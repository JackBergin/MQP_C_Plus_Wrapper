# Point Cloud Visualization

## Purpose Statement
We are graphing the point cloud points to see how the point cloud looks in regards to points. 
From there, we will factor in the cropping of the realsense point cloud as the projector resizes 
from adjusting the z axis. This would result in calibration of the projector, camera, and 
workspace simultaniously upon running the projection mapping experience.

The following url will direct you to the paper that covers this methodolgy. I want to pursue this method
over the existing one because it is much more effecient in process and can be tied directly into the live 
rendering of the projection based on moving the camera in the x, y, and z axis.
https://uwaterloo.ca/vision-image-processing-lab/sites/ca.vision-image-processing-lab/files/uploads/files/201506_auto-calibration_for_screen_correction_and_point_cloud_generation_paper_2.pdf

The following is the output of the point cloud graphing. From here, we will use the realsense to detect specific points projected on the wall to create the bounds for the realsense point cloud in our desired work space. 
![Screen Shot 2022-05-30 at 8 17 34 PM](https://user-images.githubusercontent.com/81708456/171070448-380c02b9-25b3-4dfb-a72a-51ac23c07a44.png)
