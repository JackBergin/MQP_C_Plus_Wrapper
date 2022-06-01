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

The following image below depics the x and y axis of the point cloud to which we can modify the cloud in total in order to have it map on to the projector's frame of view:

![Figure_2](https://user-images.githubusercontent.com/81708456/171459332-36021202-2f8d-4bf4-a2f9-09d966ac0bcd.png)

For examples sake, say we wanted to make the point cloud visible only for when x is between -1 meters to -0.25 meters and y is between -0.75 meters to 0 meters. In this case, using a simple loop with a nested if statement, we are able to trim the pointcloud for its visiblity only to be in the frame of this range. The same will be done when the projector projects april tags on its corners, creates a range and then this range is used to cut down the visibility of the pointcloud for the realsense. The following is a result from the proposed test stated above:

![Figure_3](https://user-images.githubusercontent.com/81708456/171461027-def933fd-37da-46b2-a293-e1a142f82c26.png)

