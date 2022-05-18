# MQP - Lung Ultrasound 3D Projection Mapping and Body Segmentation
## Setup of the Repository
Storage for all of the work done on MQP including research and current solutions
To access the current class being developed for the project follow the following directory:
MQP/RS_Proj/examples/hello-realsense

![Screen Shot 2022-05-17 at 7 04 49 AM](https://user-images.githubusercontent.com/81708456/168797223-9599c692-7b67-40b1-bd78-5062914c8e84.png)

In this directory you will find the .cpp file that runs the realsense along with the projector.
To include the projector dependencies of OpenGL, you will navigate from the above directory to the 
following:

MQP/RS_Proj/examples/hello-realsense2/hello-realsense2/CMakeFiles/rs-hello-realsense.dir/link.txt

 ![Screen Shot 2022-05-17 at 7 04 36 AM](https://user-images.githubusercontent.com/81708456/168797127-1dc3847f-3c24-4cc5-bf6f-8c2f143e537c.png)

In this file insert the following script on the end of the txt file:
 -lGL -lGLU -lglut

Once completed, the file should look similar to the following:
/usr/bin/c++    -rdynamic CMakeFiles/rs-hello-realsense.dir/rs-hello-realsense.cpp.o  -o rs-hello-realsense  -lrealsense2 -lGL -lGLU -lglut

Now, all that needs to be done is to execute the "make" function in terminal/terminator in the following directories:
MQP/RS_Proj/examples/hello-realsense2 
MQP/RS_Proj/examples/hello-realsense2/hello-realsense2 

Once this step is executed, the repository is set to run. Navigate to the following directory:
MQP/RS_Proj/examples/hello-realsense2/hello-realsense2 
And run this command: "./rs-hello-realsense"
This in the above directory is an executable file that will run all of the c++ script with the needed realsense dependencies.

![Screen Shot 2022-05-17 at 7 05 06 AM](https://user-images.githubusercontent.com/81708456/168797287-789ac050-b691-482a-beb2-16f5c2e889ad.png)
