
/*
Within this script, we will be able to inject the realsense coordinate information (hopefully)
and then from there be able to localize a projection within our anybeam projector on to the wall
localized to the realsense's reference frame
*/

#include <GL/glut.h>
#include <math.h>
#include <iostream>
#include <librealsense2/rs.hpp>


void displayMe(void)
{
    

    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0f, 0.0f, 0.0f);

    for(int i = 0; i < 8; i++){
        for (int j = 0; j < 9; j++){
            glBegin(GL_POLYGON);   
            double radius = 0.01;
            double ori_x = 0.0+0.051*j;                    
            double ori_y = 0.1+0.1*i;
            for (int i = 0; i <= 300; i++) {
                double angle = 2 * 3.14159265 * i / 300;
                double x = cos(angle) * radius;
                double y = sin(angle) * radius *1280/720;
                glVertex2d(ori_x + x, ori_y + y);
            }
            glEnd();

        }
    }
    glFlush();
}

int main(int argc, char** argv)
{


    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE);
    glutInitWindowSize(1280, 720);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("Test 1: Display Triangle");
    glutDisplayFunc(displayMe);
    glutMainLoop();

    
    return 0;
}