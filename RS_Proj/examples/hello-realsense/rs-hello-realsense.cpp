
// License: Apache 2.0. See LICENSE file in root directory.
// Copyright(c) 2019 Intel Corporation. All Rights Reserved.
#include <librealsense2/rs.hpp>
#include <librealsense2/rsutil.h>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <chrono>
#include <future>
#include <math.h>

#include "/home/parallels/apriltag-3.1.1/apriltag.h"
#include "/home/parallels/apriltag-3.1.1/apriltag_pose.h"
#include "/home/parallels/apriltag-3.1.1/common/homography.h"
#include "/home/parallels/apriltag-3.1.1/tag36h11.h"

#define FORMAT_VALUE     std::fixed << std::right << std::setprecision(3) << std::setw(6)

void homography_compute2(const double c[4][4], matd_t* H);

typedef rs2_extrinsics transformation;
static transformation to_transform(const double R[9], const double t[3]) {
    transformation tf;
    for(int r=0; r<9; ++r){ tf.rotation[r] = static_cast<float>(R[r]); }
    for(int i=0; i<3; ++i){ tf.translation[i] = static_cast<float>(t[i]); }
    return tf;
}

static transformation to_transform(const rs2_quaternion& q, const rs2_vector& t) {
    transformation tf;
    tf.rotation[0] = q.w * q.w + q.x * q.x - q.y * q.y - q.z * q.z;
    tf.rotation[1] = 2 * (q.x * q.y - q.w * q.z);
    tf.rotation[2] = 2 * (q.x * q.z + q.w * q.y);
    tf.rotation[3] = 2 * (q.x * q.y + q.w * q.z);
    tf.rotation[4] = q.w * q.w - q.x * q.x + q.y * q.y - q.z * q.z;
    tf.rotation[5] = 2 * (q.y * q.z - q.w * q.x);
    tf.rotation[6] = 2 * (q.x * q.z - q.w * q.y);
    tf.rotation[7] = 2 * (q.y * q.z + q.w * q.x);
    tf.rotation[8] = q.w * q.w - q.x * q.x - q.y * q.y + q.z * q.z;
    tf.translation[0] = t.x;
    tf.translation[1] = t.y;
    tf.translation[2] = t.z;
    return tf;
}

static transformation operator*(const transformation& a, const transformation& b) {
    transformation tf;
    tf.rotation[0] = a.rotation[0] * b.rotation[0] + a.rotation[1] * b.rotation[3] + a.rotation[2] * b.rotation[6];
    tf.rotation[1] = a.rotation[0] * b.rotation[1] + a.rotation[1] * b.rotation[4] + a.rotation[2] * b.rotation[7];
    tf.rotation[2] = a.rotation[0] * b.rotation[2] + a.rotation[1] * b.rotation[5] + a.rotation[2] * b.rotation[8];
    tf.rotation[3] = a.rotation[3] * b.rotation[0] + a.rotation[4] * b.rotation[3] + a.rotation[5] * b.rotation[6];
    tf.rotation[4] = a.rotation[3] * b.rotation[1] + a.rotation[4] * b.rotation[4] + a.rotation[5] * b.rotation[7];
    tf.rotation[5] = a.rotation[3] * b.rotation[2] + a.rotation[4] * b.rotation[5] + a.rotation[5] * b.rotation[8];
    tf.rotation[6] = a.rotation[6] * b.rotation[0] + a.rotation[7] * b.rotation[3] + a.rotation[8] * b.rotation[6];
    tf.rotation[7] = a.rotation[6] * b.rotation[1] + a.rotation[7] * b.rotation[4] + a.rotation[8] * b.rotation[7];
    tf.rotation[8] = a.rotation[6] * b.rotation[2] + a.rotation[7] * b.rotation[5] + a.rotation[8] * b.rotation[8];
    
    tf.translation[0] = a.rotation[0] * b.translation[0] + a.rotation[1] * b.translation[1] + a.rotation[2] * b.translation[2] + a.translation[0];
    tf.translation[1] = a.rotation[3] * b.translation[0] + a.rotation[4] * b.translation[1] + a.rotation[5] * b.translation[2] + a.translation[1];
    tf.translation[2] = a.rotation[6] * b.translation[0] + a.rotation[7] * b.translation[1] + a.rotation[8] * b.translation[2] + a.translation[2];
    return tf;
}

static std::string print(const transformation& tf) {
    std::stringstream ss; ss << "R:";
    for(const auto& r : tf.rotation){ ss << FORMAT_VALUE << r << ","; }
    ss << "|t:";
    for(const auto& t : tf.translation){ ss << FORMAT_VALUE << t << ","; }
    return ss.str();
}

class apriltag_manager {
public:
    apriltag_manager(const rs2_intrinsics& _intr,  const rs2_extrinsics _extr_b2f, double tagsize)
    : intr(_intr), tf_body_to_fisheye(_extr_b2f) {
        tf = tag36h11_create();
        td = apriltag_detector_create();
        apriltag_detector_add_family(td, tf);
        
        td->quad_decimate = 1.0;
        td->quad_sigma    = 0.0;
        td->nthreads      = 1;
        td->debug         = 0;
        td->refine_edges  = 1;
        
        info.tagsize      = tagsize;
        info.fx = info.fy = 1;       //undistorted image with focal length = 1
        info.cx = info.cy = 0;       //undistorted image with principal point at (0,0)
    }
    ~apriltag_manager() {
        apriltag_detector_destroy(td);
        tag36h11_destroy(tf);
    }
    
    struct apriltag_array_t {
        std::shared_ptr<zarray_t>                     det;
        std::vector<std::shared_ptr<apriltag_pose_t>> pose_raw;       //tag pose from library
        std::vector<transformation>                   pose_in_camera; //tag pose in camera coordinate
        std::vector<transformation>                   pose_in_world;  //tag pose in world coordinate
        
        apriltag_detection_t* get(int t) const { apriltag_detection_t* ptr; zarray_get(det.get(), t, &ptr); return ptr; }
        int get_id(int t) const { return get(t)->id; }
        int size() const { return pose_in_camera.size(); }
    };
    
    static void apriltag_pose_destroy(apriltag_pose_t* p){ matd_destroy(p->R); matd_destroy(p->t); delete p;}
    
    apriltag_array_t detect(unsigned char* gray, const rs2_pose* camera_pose) const {
        image_u8_t img{ intr.width, intr.height, intr.width, gray};
        
        apriltag_array_t tags;
        tags.det = std::shared_ptr<zarray_t>(apriltag_detector_detect(td, &img), apriltag_detections_destroy);
        tags.pose_in_camera.resize(zarray_size(tags.det.get()));
        tags.pose_raw.resize(tags.size());

        auto info_ = info;
        for(int t=0, num_of_tags=(int)tags.size(); t<num_of_tags; ++t)
        {
            tags.pose_raw[t] = std::shared_ptr<apriltag_pose_t>(new apriltag_pose_t(), apriltag_pose_destroy);

            undistort(*(info_.det = tags.get(t)), intr);                      //recompute tag corners on an undistorted image focal length = 1
            //estimate_tag_pose(&info_, tags.pose_raw[t].get());              //(alternative) estimate tag pose in camera coordinate
            estimate_pose_for_tag_homography(&info_, tags.pose_raw[t].get()); //estimate tag pose in camera coordinate
            for(auto c : {1,2,4,5,7,8}){ tags.pose_raw[t]->R->data[c] *= -1; }
            
            tags.pose_in_camera[t] = to_transform(tags.pose_raw[t]->R->data, tags.pose_raw[t]->t->data);
        }
        
        if(camera_pose){ compute_tag_pose_in_world(tags, *camera_pose); }
        return tags;
    }
    
protected:
    apriltag_family_t        *tf;
    apriltag_detector_t      *td;
    apriltag_detection_info_t info;
    rs2_intrinsics            intr;
    transformation            tf_body_to_fisheye;
    
    void compute_tag_pose_in_world(apriltag_array_t& tags, const rs2_pose& camera_world_pose) const {
        tags.pose_in_world.resize(tags.size());
        for(int t=0, num_of_tags=tags.size(); t<num_of_tags; ++t){
            auto tf_fisheye_to_tag = tags.pose_in_camera[t];
            auto tf_world_to_body = to_transform(camera_world_pose.rotation, camera_world_pose.translation);
            tags.pose_in_world[t] = tf_world_to_body * tf_body_to_fisheye * tf_fisheye_to_tag;
        }
    }
    
    static void undistort(apriltag_detection_t& src, const rs2_intrinsics& intr) {
        deproject(src.c, intr, src.c);
        
        double corr_arr[4][4];
        for(int c=0; c<4; ++c){
            deproject(src.p[c], intr, src.p[c]);
            
            corr_arr[c][0] = (c==0 || c==3) ? -1 : 1; // tag corners in an ideal image
            corr_arr[c][1] = (c==0 || c==1) ? -1 : 1; // tag corners in an ideal image
            corr_arr[c][2] = src.p[c][0];             // tag corners in undistorted image focal length = 1
            corr_arr[c][3] = src.p[c][1];             // tag corners in undistorted image focal length = 1
        }
        if(src.H == nullptr) { src.H = matd_create(3, 3); }
        homography_compute2(corr_arr, src.H);
    }
    
    static void deproject(double pt[2], const rs2_intrinsics& intr, const double px[2]) {
        float fpt[3], fpx[2] = { (float)px[0], (float)px[1] };
        rs2_deproject_pixel_to_point(fpt, &intr, fpx, 1.0f);
        pt[0] = fpt[0];
        pt[1] = fpt[1];
    }
};

int main(int argc, char * argv[]) try
{
    // Declare RealSense pipeline, encapsulating the actual device and sensors
    rs2::pipeline pipe;
    // Create a configuration for configuring the pipeline with a non default profile
    rs2::config cfg;
    // Add pose stream
    cfg.enable_stream(RS2_STREAM_POSE, RS2_FORMAT_6DOF);
    // Enable both image streams
    // Note: It is not currently possible to enable only one
    cfg.enable_stream(RS2_STREAM_FISHEYE, 1, RS2_FORMAT_Y8);
    cfg.enable_stream(RS2_STREAM_FISHEYE, 2, RS2_FORMAT_Y8);
    
    // replay
    if(argc > 1){ cfg.enable_device_from_file(argv[1]); }
    
    // Start pipe and get camera calibrations
    const int fisheye_sensor_idx = 1;  //for the left fisheye lens of T265
    auto pipe_profile       = pipe.start(cfg);
    auto fisheye_stream     = pipe_profile.get_stream(RS2_STREAM_FISHEYE, fisheye_sensor_idx);
    auto fisheye_intrinsics = fisheye_stream.as<rs2::video_stream_profile>().get_intrinsics();
    auto body_fisheye_extr  = fisheye_stream.get_extrinsics_to(pipe_profile.get_stream(RS2_STREAM_POSE));
    const double tag_size_m = 0.144; // The expected size of the tag in meters. This is required to get the relative pose

    // Create an Apriltag detection manager
    apriltag_manager tag_manager(fisheye_intrinsics, body_fisheye_extr, tag_size_m);
    
    // Main loop
    while (true)
    {
        // Wait for the next set of frames from the camera
        auto frames        = pipe.wait_for_frames();
        auto fisheye_frame = frames.get_fisheye_frame(fisheye_sensor_idx);
        auto frame_number  = fisheye_frame.get_frame_number();
        auto camera_pose   = frames.get_pose_frame().get_pose_data();
        
        if(frame_number % 6 == 0)
        {
            fisheye_frame.keep();
            
            std::async(std::launch::async, std::bind([&tag_manager](rs2::frame img, int fn, rs2_pose pose){
                auto tags = tag_manager.detect((unsigned char*)img.get_data(), &pose);

                if(tags.pose_in_camera.size() == 0) {
                    std::cout << "frame " << fn << "|no Apriltag detections" << std::endl;
                }
                for(int t=0; t<tags.pose_in_camera.size(); ++t){
                    std::stringstream ss; ss << "frame " << fn << "|tag id: " << tags.get_id(t) << "|";
                    std::cout << ss.str() << "camera " << print(tags.pose_in_camera[t]) << std::endl;
                    std::cout << std::setw(ss.str().size()) << " " << "world  " <<
                                (pose.tracker_confidence == 3 ? print(tags.pose_in_world[t]) : " NA ") << std::endl << std::endl;
                }
            }, fisheye_frame, frame_number, camera_pose));
        }
    }
    
    return EXIT_SUCCESS;
}
catch (const rs2::error & e)
{
    std::cerr << "RealSense error calling " << e.get_failed_function() << "(" << e.get_failed_args() << "):\n    " << e.what() << std::endl;
    return EXIT_FAILURE;
}
catch (const std::exception& e)
{
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
}

//
// Re-compute homography between ideal standard tag image and undistorted tag corners for estimage_tag_pose().
//
// @param[in]  c is 4 pairs of tag corners on ideal image and undistorted input image.
// @param[out] H is the output homography between ideal and undistorted input image.
// @see        static void apriltag_manager::undistort(...)
//
void homography_compute2(const double c[4][4], matd_t* H) {
    double A[] =  {
        c[0][0], c[0][1], 1,       0,       0, 0, -c[0][0]*c[0][2], -c[0][1]*c[0][2], c[0][2],
        0,       0, 0, c[0][0], c[0][1], 1, -c[0][0]*c[0][3], -c[0][1]*c[0][3], c[0][3],
        c[1][0], c[1][1], 1,       0,       0, 0, -c[1][0]*c[1][2], -c[1][1]*c[1][2], c[1][2],
        0,       0, 0, c[1][0], c[1][1], 1, -c[1][0]*c[1][3], -c[1][1]*c[1][3], c[1][3],
        c[2][0], c[2][1], 1,       0,       0, 0, -c[2][0]*c[2][2], -c[2][1]*c[2][2], c[2][2],
        0,       0, 0, c[2][0], c[2][1], 1, -c[2][0]*c[2][3], -c[2][1]*c[2][3], c[2][3],
        c[3][0], c[3][1], 1,       0,       0, 0, -c[3][0]*c[3][2], -c[3][1]*c[3][2], c[3][2],
        0,       0, 0, c[3][0], c[3][1], 1, -c[3][0]*c[3][3], -c[3][1]*c[3][3], c[3][3],
    };
    
    double epsilon = 1e-10;
    
    // Eliminate.
    for (int col = 0; col < 8; col++) {
        // Find best row to swap with.
        double max_val = 0;
        int max_val_idx = -1;
        for (int row = col; row < 8; row++) {
            double val = fabs(A[row*9 + col]);
            if (val > max_val) {
                max_val = val;
                max_val_idx = row;
            }
        }
        
        if (max_val < epsilon) {
            fprintf(stderr, "WRN: Matrix is singular.\n");
        }
        
        // Swap to get best row.
        if (max_val_idx != col) {
            for (int i = col; i < 9; i++) {
                double tmp = A[col*9 + i];
                A[col*9 + i] = A[max_val_idx*9 + i];
                A[max_val_idx*9 + i] = tmp;
            }
        }
        
        // Do eliminate.
        for (int i = col + 1; i < 8; i++) {
            double f = A[i*9 + col]/A[col*9 + col];
            A[i*9 + col] = 0;
            for (int j = col + 1; j < 9; j++) {
                A[i*9 + j] -= f*A[col*9 + j];
            }
        }
    }
    
    // Back solve.
    for (int col = 7; col >=0; col--) {
        double sum = 0;
        for (int i = col + 1; i < 8; i++) {
            sum += A[col*9 + i]*A[i*9 + 8];
        }
        A[col*9 + 8] = (A[col*9 + 8] - sum)/A[col*9 + col];
    }
    H->data[0] = A[8];
    H->data[1] = A[17];
    H->data[2] = A[26];
    H->data[3] = A[35];
    H->data[4] = A[44];
    H->data[5] = A[53];
    H->data[6] = A[62];
    H->data[7] = A[71];
    H->data[8] = 1;
}



/*
// License: Apache 2.0. See LICENSE file in root directory.
// Copyright(c) 2015-2017 Intel Corporation. All Rights Reserved.

#include <librealsense2/rs.hpp> // Include RealSense Cross Platform API
#include "/home/parallels/Desktop/GitHub_Work/MQP/RS_Proj/examples/example.hpp"          // Include short list of convenience functions for rendering
#include <iostream>
#include <fstream>
#include <algorithm>            // std::min, std::max

// Helper functions
void register_glfw_callbacks(window& app, glfw_state& app_state);

int main(int argc, char * argv[]) try
{
    // Create a simple OpenGL window for rendering:
    window app(1280, 720, "RealSense Pointcloud Example");
    

    // Construct an object to manage view state
    glfw_state app_state;
    // register callbacks to allow manipulation of the pointcloud
    register_glfw_callbacks(app, app_state);

    // Declare pointcloud object, for calculating pointclouds and texture mappings
    rs2::pointcloud pc;
    // We want the points object to be persistent so we can display the last cloud when a frame drops
    rs2::points points;

    // Declare RealSense pipeline, encapsulating the actual device and sensors
    rs2::pipeline pipe;
    // Start streaming with default recommended configuration
    pipe.start();

    //while (app) // Application still alive?
    //{
        // Wait for the next set of frames from the camera
        auto frames = pipe.wait_for_frames();

        auto color = frames.get_color_frame();

        // For cameras that don't have RGB sensor, we'll map the pointcloud to infrared instead of color
        if (!color)
            color = frames.get_infrared_frame();

        // Tell pointcloud object to map to this color frame
        pc.map_to(color);

        auto depth = frames.get_depth_frame();

        // Generate the pointcloud and texture mappings
        points = pc.calculate(depth);
        
        //std::cout << "SIZE: " << points.size();
        //std::cout << "POINTS: " << points;
        
        auto vertices = points.get_vertices();              // get vertices
        auto tex_coords = points.get_texture_coordinates(); // and texture coordinates
        std::vector< double > verticieX;
        std::vector< double > verticieY;        
        std::vector< double > verticieZ;
        

        for (int i = 0; i < points.size(); i++) {
            if (vertices[i].z){
                //Gets all of the point cloud data
                verticieX.push_back(vertices[i].x);
                verticieY.push_back(vertices[i].y);                
                verticieZ.push_back(vertices[i].z);
           }
        }
        
        // Preliminary visualization of the points in the output window
        std::cout << "X Pt: " << verticieX[1] << "Length X: " << verticieX.size();
        std::cout << "Y Pt: " << verticieY[1] << "Length Y: " << verticieY.size();
        std::cout << "Z Pt: " << verticieZ[1] << "Length Z: " << verticieZ.size(); 

        // Csv storage of the point cloud for x, y, and z
        std::ofstream myfile;
        myfile.open ("/home/parallels/Desktop/GitHub_Work/MQP/RS_Proj/examples/hello-realsense/example.csv");
        myfile << "Verticie X, Verticie Y, Verticie Z, \n";



        for(int j = 0; j < points.size(); j++){
           //myfile << verticieX[j];
           //myfile << verticieY[j];           
           //myfile << verticieZ[j];
           //myfile << "\n";
            myfile << verticieX[j] << "," << verticieY[j] << "," << verticieZ[j] << std::endl;


        }

        myfile.close();

        // Upload the color frame to OpenGL
        app_state.tex.upload(color);

        // Draw the pointcloud
        draw_pointcloud(app.width(), app.height(), app_state, points);
    //}
    


    return EXIT_SUCCESS;
}
catch (const rs2::error & e)
{
    std::cerr << "RealSense error calling " << e.get_failed_function() << "(" << e.get_failed_args() << "):\n    " << e.what() << std::endl;
    return EXIT_FAILURE;
}
catch (const std::exception & e)
{
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
}
*/

/*
Within this script, we will be able to inject the realsense coordinate information (hopefully)
and then from there be able to localize a projection within our anybeam projector on to the wall
localized to the realsense's reference frame
*/

/*
#include <GL/glut.h>
#include <GL/freeglut.h>
#include <math.h>
#include <iostream>
#include <librealsense2/rs.hpp>
#include <unistd.h>


void displayMe(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0f, 0.0f, 0.0f);
    glBegin(GL_POLYGON);                        // Middle circle
    double radius = 0.01;
    double ori_x = 0.0;                         // the origin or center of circle
    double ori_y = 0.0;
    //double ori_x = -0.11;                         // the origin or center of circle
    //double ori_y = -0.46;
    //I want to insert the measurements to be here. can and should
    for (int i = 0; i <= 300; i++) {
        double angle = 2 * 3.14159265 * i / 300;
        double x = cos(angle) * radius;
        double y = sin(angle) * radius *1280/720;
        glVertex2d(ori_x + x, ori_y + y);
    }
    //Ends the drawing and then redraws the dot.
    glEnd();
    glFlush();
}


// Hello RealSense example demonstrates the basics of connecting to a RealSense device
// and taking advantage of depth data
int main(int argc, char * argv[]) try
{
   // Create a Pipeline - this serves as a top-level API for streaming and processing frames
   rs2::pipeline p;
   glutInit(&argc, argv);

   // Configure and start the pipeline
   p.start();
 
   while (true)
   {
       // Block program until frames arrive
       rs2::frameset frames = p.wait_for_frames();
 
       // Try to get a frame of a depth image
       rs2::depth_frame depth = frames.get_depth_frame();
 
       // Get the depth frame's dimensions
       auto width = depth.get_width();
       auto height = depth.get_height();
        
       //auto test = rs2_project_point_to_pixel(width, height);
       
       //---------------Attrivial of the projector control-------------
       // Query the distance from the camera to the object in the center of the image
    
       float dist_to_center = depth.get_distance(width / 2, height / 2);
       
       glutInitDisplayMode(GLUT_SINGLE);
       glutInitWindowSize(1280, 720);
       glutInitWindowPosition(0, 0);
       glutCreateWindow("Calibration");
       glutDisplayFunc(displayMe);
    
       glutMainLoopEvent();
       sleep(1);
       //exit(true);

       //delay(10);
       //glutLeaveMainLoop();
       //NOTE: Will test viability of the drawing loop.
       //---------------Attrivial of the projector control-------------
       
       // Print the distance
       std::cout << "The camera is facing an object " << dist_to_center << " meters away \r";

   }
   return EXIT_SUCCESS;
}
catch (const rs2::error & e)
{
   std::cerr << "RealSense error calling " << e.get_failed_function() << "(" << e.get_failed_args() << "):\n    " << e.what() << std::endl;
   return EXIT_FAILURE;
}
catch (const std::exception& e)
{
   std::cerr << e.what() << std::endl;
   return EXIT_FAILURE;
}
*/













