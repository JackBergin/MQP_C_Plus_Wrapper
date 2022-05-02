# Install script for directory: /home/parallels/vcpkg/buildtrees/realsense2/src/v2.50.0-ec78fddb81.clean/examples

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/home/parallels/vcpkg/buildtrees/realsense2/src/v2.50.0-ec78fddb81.clean/examples/hdr/hello-realsense/cmake_install.cmake")
  include("/home/parallels/vcpkg/buildtrees/realsense2/src/v2.50.0-ec78fddb81.clean/examples/hdr/software-device/cmake_install.cmake")
  include("/home/parallels/vcpkg/buildtrees/realsense2/src/v2.50.0-ec78fddb81.clean/examples/hdr/capture/cmake_install.cmake")
  include("/home/parallels/vcpkg/buildtrees/realsense2/src/v2.50.0-ec78fddb81.clean/examples/hdr/callback/cmake_install.cmake")
  include("/home/parallels/vcpkg/buildtrees/realsense2/src/v2.50.0-ec78fddb81.clean/examples/hdr/save-to-disk/cmake_install.cmake")
  include("/home/parallels/vcpkg/buildtrees/realsense2/src/v2.50.0-ec78fddb81.clean/examples/hdr/multicam/cmake_install.cmake")
  include("/home/parallels/vcpkg/buildtrees/realsense2/src/v2.50.0-ec78fddb81.clean/examples/hdr/pointcloud/cmake_install.cmake")
  include("/home/parallels/vcpkg/buildtrees/realsense2/src/v2.50.0-ec78fddb81.clean/examples/hdr/align/cmake_install.cmake")
  include("/home/parallels/vcpkg/buildtrees/realsense2/src/v2.50.0-ec78fddb81.clean/examples/hdr/align-advanced/cmake_install.cmake")
  include("/home/parallels/vcpkg/buildtrees/realsense2/src/v2.50.0-ec78fddb81.clean/examples/hdr/sensor-control/cmake_install.cmake")
  include("/home/parallels/vcpkg/buildtrees/realsense2/src/v2.50.0-ec78fddb81.clean/examples/hdr/measure/cmake_install.cmake")
  include("/home/parallels/vcpkg/buildtrees/realsense2/src/v2.50.0-ec78fddb81.clean/examples/hdr/C/depth/cmake_install.cmake")
  include("/home/parallels/vcpkg/buildtrees/realsense2/src/v2.50.0-ec78fddb81.clean/examples/hdr/C/color/cmake_install.cmake")
  include("/home/parallels/vcpkg/buildtrees/realsense2/src/v2.50.0-ec78fddb81.clean/examples/hdr/C/distance/cmake_install.cmake")
  include("/home/parallels/vcpkg/buildtrees/realsense2/src/v2.50.0-ec78fddb81.clean/examples/hdr/post-processing/cmake_install.cmake")
  include("/home/parallels/vcpkg/buildtrees/realsense2/src/v2.50.0-ec78fddb81.clean/examples/hdr/record-playback/cmake_install.cmake")
  include("/home/parallels/vcpkg/buildtrees/realsense2/src/v2.50.0-ec78fddb81.clean/examples/hdr/motion/cmake_install.cmake")
  include("/home/parallels/vcpkg/buildtrees/realsense2/src/v2.50.0-ec78fddb81.clean/examples/hdr/gl/cmake_install.cmake")
  include("/home/parallels/vcpkg/buildtrees/realsense2/src/v2.50.0-ec78fddb81.clean/examples/hdr/pose/cmake_install.cmake")
  include("/home/parallels/vcpkg/buildtrees/realsense2/src/v2.50.0-ec78fddb81.clean/examples/hdr/pose-predict/cmake_install.cmake")
  include("/home/parallels/vcpkg/buildtrees/realsense2/src/v2.50.0-ec78fddb81.clean/examples/hdr/pose-and-image/cmake_install.cmake")
  include("/home/parallels/vcpkg/buildtrees/realsense2/src/v2.50.0-ec78fddb81.clean/examples/hdr/trajectory/cmake_install.cmake")
  include("/home/parallels/vcpkg/buildtrees/realsense2/src/v2.50.0-ec78fddb81.clean/examples/hdr/ar-basic/cmake_install.cmake")
  include("/home/parallels/vcpkg/buildtrees/realsense2/src/v2.50.0-ec78fddb81.clean/examples/hdr/ar-advanced/cmake_install.cmake")
  include("/home/parallels/vcpkg/buildtrees/realsense2/src/v2.50.0-ec78fddb81.clean/examples/hdr/pose-apriltag/cmake_install.cmake")
  include("/home/parallels/vcpkg/buildtrees/realsense2/src/v2.50.0-ec78fddb81.clean/examples/hdr/tracking-and-depth/cmake_install.cmake")
  include("/home/parallels/vcpkg/buildtrees/realsense2/src/v2.50.0-ec78fddb81.clean/examples/hdr/hdr/cmake_install.cmake")

endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "/home/parallels/vcpkg/buildtrees/realsense2/src/v2.50.0-ec78fddb81.clean/examples/hdr/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
