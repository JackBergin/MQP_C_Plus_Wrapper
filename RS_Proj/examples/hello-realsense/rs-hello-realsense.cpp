// License: Apache 2.0. See LICENSE file in root directory.
// Copyright(c) 2015-2017 Intel Corporation. All Rights Reserved.

#include <librealsense2/rs.hpp> // Include RealSense Cross Platform API
#include "/home/parallels/Desktop/GitHub_Work/MQP/RS_Proj/examples/example.hpp"          // Include short list of convenience functions for rendering
#include </home/parallels/Desktop/GitHub_Work/MQP/RS_Proj/examples/rs_processing_gl.hpp> // Include GPU-Processing API

// Helper functions
void register_glfw_callbacks(window& app, glfw_state& app_state);
void prepare_matrices(glfw_state& app_state, float width, float height, float* projection, float* view);
bool handle_input(window& app, bool& use_gpu_processing);
// This helper class will be used to keep track of FPS and print instructions:
struct instructions_printer
{
    instructions_printer();
    void print_instructions(window& app, bool use_gpu_processing);

    std::chrono::high_resolution_clock::time_point last_clock;
    int rendered_frames;
    int last_fps;
};

int main(int argc, char * argv[]) try
{
    // The following toggle is going to control
    // if we will use CPU or GPU for depth data processing
    bool use_gpu_processing = true;

    // Create a simple OpenGL window for rendering:
    window app(1280, 720, "RealSense GPU-Processing Example");

    // Once we have a window, initialize GL module
    // Pass our window to enable sharing of textures between processed frames and the window
    rs2::gl::init_processing(app, use_gpu_processing);
    // Initialize rendering module:
    rs2::gl::init_rendering();

    // Construct an object to manage view state
    glfw_state app_state;
    // register callbacks to allow manipulation of the pointcloud
    register_glfw_callbacks(app, app_state);

    texture tex; // For when not using GPU frames, we will need standard texture object
    // to render pointcloud texture (similar to pointcloud example)

    instructions_printer printer;

    // Every iteration the demo will render 3D pointcloud that will be stored in points object:
    rs2::points points;
    // The pointcloud will use colorized depth as a texture:
    rs2::frame  depth_texture;

    // ---- Declare GL processing blocks ----
    rs2::gl::pointcloud pc;         // similar to rs2::pointcloud
    rs2::gl::colorizer  colorizer;  // similar to rs2::colorizer
    rs2::gl::uploader   upload;     // used to explicitly copy frame to the GPU

    // ---- Declare rendering block      ----
    rs2::gl::pointcloud_renderer pc_renderer; // Will handle rendering points object to the screen

    // We will manage two matrices - projection and view
    // to handle mouse input and perspective
    float proj_matrix[16];
    float view_matrix[16];

    // Enable pipeline
    rs2::pipeline pipe;
    rs2::config cfg; 
    // For this example, we are only interested in depth:
    cfg.enable_stream(RS2_STREAM_DEPTH, RS2_FORMAT_Z16);
    pipe.start(cfg);

    while (app) // Application still alive?
    {
        // Any new frames?
        rs2::frameset frames;
        if (pipe.poll_for_frames(&frames))
        {
            auto depth = frames.get_depth_frame();

            // Since both colorizer and pointcloud are going to use depth
            // It helps to explicitly upload it to the GPU
            // Otherwise, each block will upload the same depth to GPU separately 
            // [ This step is optional, but can speed things up on some cards ]
            // The result of this step is still a rs2::depth_frame, 
            // but now it is also extendible to rs2::gl::gpu_frame
            depth = upload.process(depth);

            // Apply color map with histogram equalization
            depth_texture = colorizer.colorize(depth);

            // Tell pointcloud object to map to this color frame
            pc.map_to(depth_texture);

            // Generate the pointcloud and texture mappings
            points = pc.calculate(depth);
        }

        // Populate projection and view matrices based on user input
        prepare_matrices(app_state, 
                         app.width(), app.height(), 
                         proj_matrix, view_matrix);

        // We need to get OpenGL texture ID for pointcloud texture
        uint32_t texture_id = 0;
        // First, check if the frame is already a GPU-frame
        if (auto gpu_frame = depth_texture.as<rs2::gl::gpu_frame>())
        {
            // If it is (and you have passed window for sharing to init_processing
            // you can just get the texture ID of the GPU frame
            texture_id = gpu_frame.get_texture_id(0);
        }
        else
        {
            // Otherwise, we need to upload texture like in all other examples
            tex.upload(depth_texture);
            texture_id = tex.get_gl_handle();
        }

        // Clear screen
        glClearColor(0.2f, 0.2f, 0.2f, 1.f);
        glClear(GL_DEPTH_BUFFER_BIT);
        
        // We need Depth-Test unless you want pointcloud to intersect with itself
        glEnable(GL_DEPTH_TEST);

        // Set texture to our selected texture ID
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texture_id);

        // Inform pointcloud-renderer of the projection and view matrices:
        pc_renderer.set_matrix(RS2_GL_MATRIX_PROJECTION, proj_matrix);
        pc_renderer.set_matrix(RS2_GL_MATRIX_TRANSFORMATION, view_matrix);

        // If we have something to render, use pointcloud-renderer to do it
        if (points) pc_renderer.process(points);

        // Disable texturing
        glDisable(GL_TEXTURE_2D);

        // Print textual information
        printer.print_instructions(app, use_gpu_processing);

        // Handle user input and check if reset is required
        if (handle_input(app, use_gpu_processing))
        {
            // First, shutdown processing and rendering
            rs2::gl::shutdown_rendering();
            rs2::gl::shutdown_processing();

            // Next, reinitialize processing and rendering with new settings
            rs2::gl::init_processing(app, use_gpu_processing);
            rs2::gl::init_rendering();
        }
    }

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

instructions_printer::instructions_printer()
{
    // Initialize running clock to keep track of time (for FPS calculation)
    auto last_clock = std::chrono::high_resolution_clock::now();
    int rendered_frames = 0;    // Counts number of frames since last update
    int last_fps = 0;           // Stores last calculated FPS
    glfwSwapInterval(0);        // This is functionally disabling V-Sync, 
                                // allowing application FPS to go beyond monitor refresh rate
}

void instructions_printer::print_instructions(window& app, bool use_gpu_processing)
{
    glLoadIdentity();
    glMatrixMode(GL_PROJECTION);
    glOrtho(0, app.width(), app.height(), 0, -1, +1);

    glColor3f(1.f, 1.f, 1.f);
    std::stringstream ss;
    ss << "Performing Point-Cloud and Histogram-Equalization (Colorize) on the " << (use_gpu_processing ? "GPU" : "CPU");
    draw_text(20, 20, ss.str().c_str());

    ss.str("");
    ss << "( Press " << (use_gpu_processing ? "[C] - Switch to processing on the CPU )" : "[G] - Switch to processing on the GPU )");
    draw_text(20, 36, ss.str().c_str());

    ss.str("");
    ss << "This demo is being rendered at " << last_fps << " frames per second, ~" << (1000 / (last_fps + 1)) << "ms for single rendered frame";
    draw_text(20, 52, ss.str().c_str());

    rendered_frames++;

    auto since_last_clock = std::chrono::high_resolution_clock::now() - last_clock;
    auto sec_elapsed = std::chrono::duration_cast<std::chrono::seconds>(since_last_clock).count();
    if (sec_elapsed > 0)
    {
        last_clock = std::chrono::high_resolution_clock::now();
        last_fps = rendered_frames;
        rendered_frames = 0;
    }
}

bool handle_input(window& app, bool& use_gpu_processing)
{
    bool reset = false;

    if (glfwGetKey(app, GLFW_KEY_C) == GLFW_PRESS)
    {
        reset = use_gpu_processing;
        use_gpu_processing = false;
    }
    if (glfwGetKey(app, GLFW_KEY_G) == GLFW_PRESS)
    {
        reset = !use_gpu_processing;
        use_gpu_processing = true;
    }

    return reset;
}

void prepare_matrices(glfw_state& app_state, float width, float height, float* projection, float* view)
{
    glLoadIdentity();

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    gluPerspective(60, width / height, 0.01f, 10.0f);
    glGetFloatv(GL_PROJECTION_MATRIX, projection);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    gluLookAt(0, 0, 0, 0, 0, 1, 0, -1, 0);
    glTranslatef(0, 0, 0.5f + app_state.offset_y * 0.05f);
    glRotated(app_state.pitch, 1, 0, 0);
    glRotated(app_state.yaw, 0, 1, 0);
    glTranslatef(0, 0, -0.5f);
    glGetFloatv(GL_MODELVIEW_MATRIX, view);

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
}




/*
// License: Apache 2.0. See LICENSE file in root directory.
// Copyright(c) 2015-2017 Intel Corporation. All Rights Reserved.

#include <librealsense2/rs.hpp> // Include RealSense Cross Platform API
#include "/home/parallels/Desktop/GitHub_Work/MQP/RS_Proj/examples/example.hpp"          // Include short list of convenience functions for rendering

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
        
        
        std::cout << "X Pt: " << verticieX[1] << "Length X: " << verticieX.size();
        std::cout << "Y Pt: " << verticieY[1] << "Length Y: " << verticieY.size();
        std::cout << "Z Pt: " << verticieZ[1] << "Length Z: " << verticieZ.size();
        
                
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













