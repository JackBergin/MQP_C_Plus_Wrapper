/*
Within this script, we will be able to inject the realsense coordinate information (hopefully)
and then from there be able to localize a projection within our anybeam projector on to the wall
localized to the realsense's reference frame
*/

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
    double ori_x = -0.11;                         // the origin or center of circle
    double ori_y = -0.46;

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
        
        
       //---------------Attrivial of the projector control-------------
       // Query the distance from the camera to the object in the center of the image
    
       float dist_to_center = depth.get_distance(width / 2, height / 2);
       
       glutInit(&argc, argv);
       glutInitDisplayMode(GLUT_SINGLE);
       glutInitWindowSize(1280, 720);
       glutInitWindowPosition(0, 0);
       glutCreateWindow("Calibration");
       glutDisplayFunc(displayMe);
       glutMainLoopEvent();
              
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
 














