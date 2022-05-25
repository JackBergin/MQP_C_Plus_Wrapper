
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













