# Preliminary OpenGL Test
This folder and test was created along side the hello-realsense experience to prove viability of the OpenGL software for rendering graphics and creating graphics. This test was the first illustration done by myself and then projected through the Anybeam projector. The following was the animation.

![Screen Shot 2022-05-17 at 7 18 12 AM](https://user-images.githubusercontent.com/81708456/168799115-8937bc4c-ae55-4215-b141-170dca88cd97.png)

It is a red dot in the center of the screen which will be extended to creating an array. From there, the rs camera and the OpenGL code will be integrated for camera, projector, workspace calibration. The following is the current mathematical conceptualization behind creating these transforms for the calibration along with general discussion about how to solve this problem.

![IMG_2BE960F67919-1](https://user-images.githubusercontent.com/81708456/168799454-2291c6a7-34a2-4a5e-a5e4-570e85f9e380.jpeg)

For a better explaination of the calibration, the pinhole method is a good way to visualize what is going to be done in the camera and projector system. The following image showcases a pinhole method being done on a camera. "The pinhole camera model describes a camera that projects scene 3D points into the image plane by means of a perspective transformation." (https://docs.nvidia.com/vpi/appendix_pinhole_camera.html) 

![Screen Shot 2022-05-18 at 5 50 45 AM](https://user-images.githubusercontent.com/81708456/169012324-80b54555-61cf-4a22-95e2-4a79bca35922.png)

With this already being a capability on the realsense as a function, the two other things needed are; the transformation from camera to projector, the translation of the pixel point in the realsense frame to the corresponding pixel point of the projector. To implement these methodoligies of calibration, this grid of 9x8 1 inch checkerboard was created along with dots that correspond to the same spacing and are each a half inch in diameter from a  distance of 1.5 meters. The setup of the system was also changed from a static one to more of a dynamic one with the integration of a tripod.
