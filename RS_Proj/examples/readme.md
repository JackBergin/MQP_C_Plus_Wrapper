
# Sample Code for Intel® RealSense™ cameras
**Code Examples to start prototyping quickly:** These simple examples demonstrate how to easily use the SDK to include code snippets that access the camera into your applications.  

## List of Examples:

|Name | Language | Description | Experience Level | Technology |
|---|---|---|---|---|
|[Hello-RealSense](./hello-realsense)| C++ | Demonstrates the basics of connecting to a RealSense device and using depth data | :star: | [![Depth Sensing - Structured Light, Stereo and L500](https://img.shields.io/badge/-Depth-5bc3ff.svg)](./depth.md) |

|[Pointcloud](./pointcloud)| C++ | Showcase Projection API while generating and rendering 3D pointcloud | :star: | [![Depth Sensing - Structured Light, Stereo and L500](https://img.shields.io/badge/-Depth-5bc3ff.svg)](./depth.md) |

|[ImShow](../wrappers/opencv/imshow) | C++ & [OpenCV](https://github.com/IntelRealSense/librealsense/tree/master/wrappers/opencv#getting-started) | Minimal OpenCV application for visualizing depth data | :star: | [![Depth Sensing - Structured Light, Stereo and L500](https://img.shields.io/badge/-Depth-5bc3ff.svg)](./depth.md)|

|[Post Processing](./post-processing)| C++ | Demonstrating usage of post processing filters for depth images | :star::star: | [![Depth Sensing - Structured Light, Stereo and L500](https://img.shields.io/badge/-Depth-5bc3ff.svg)](./depth.md) |

|[Apriltag Pose](./pose-apriltag)|C++|Demonstrates how to compute [Apriltag](https://github.com/AprilRobotics/apriltag/tree/3.1.1) pose from T265 fisheye image stream. | :star::star: |[![Motion Tracking - T260 and SLAM](https://img.shields.io/badge/-Tracking-0e2356.svg)](../doc/t265.md#examples-and-tools)|

### Community Projects:
1. [OpenCV threaded depth cleaner](https://github.com/juniorxsound/ThreadedDepthCleaner) - RealSense depth-map cleaning and inpainting using OpenCV
2. [Unofficial OpenVino example + D400](https://github.com/gbr1/ros_openvino) - example of using OpenVino with RealSense and ROS for object detection
