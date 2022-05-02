<p align="center"><img src="doc/img/realsense.png" width="70%" /><br><br></p>

----------------
## Overview
**Intel® RealSense™ SDK 2.0** is a cross-platform library for Intel® RealSense™ depth cameras (D400 & L500 series and the SR300) and the [T265 tracking camera](./doc/t265.md).

The SDK allows depth and color streaming, and provides intrinsic and extrinsic calibration information.
The library also offers synthetic streams (pointcloud, depth aligned to color and vise-versa), and a built-in support for [record and playback](./doc/record-and-playback.md) of streaming sessions.

## Building librealsense - Using vcpkg

You can download and install librealsense using the [vcpkg](https://github.com/Microsoft/vcpkg) dependency manager:

    git clone https://github.com/Microsoft/vcpkg.git
    cd vcpkg
    ./bootstrap-vcpkg.sh
    ./vcpkg integrate install
    ./vcpkg install realsense2

## Download and Install
* **Download** - The latest releases including the Intel RealSense SDK, Viewer and Depth Quality tools are available at: [**latest releases**](https://github.com/IntelRealSense/librealsense/releases). Please check the [**release notes**](https://github.com/IntelRealSense/librealsense/wiki/Release-Notes) for the supported platforms, new features and capabilities, known issues, how to upgrade the Firmware and more.

* **Install** - You can also install or build from source the SDK (on [Linux](./doc/distribution_linux.md) \ [Windows](./doc/distribution_windows.md) \ [Mac OS](doc/installation_osx.md) \ [Android](./doc/android.md) \ [Docker](./scripts/Docker/readme.md)), connect your D400 depth camera and you are ready to start writing your first application.

> **Support & Issues**: If you need product support (e.g. ask a question about / are having problems with the device), please check the [FAQ & Troubleshooting](https://github.com/IntelRealSense/librealsense/wiki/Troubleshooting-Q%26A) section.
> If not covered there, please search our [Closed GitHub Issues](https://github.com/IntelRealSense/librealsense/issues?utf8=%E2%9C%93&q=is%3Aclosed) page,  [Community](https://communities.intel.com/community/tech/realsense) and [Support](https://www.intel.com/content/www/us/en/support/emerging-technologies/intel-realsense-technology.html) sites.
> If you still cannot find an answer to your question, please [open a new issue](https://github.com/IntelRealSense/librealsense/issues/new).

## License
This project is licensed under the [Apache License, Version 2.0](LICENSE).
Copyright 2018 Intel Corporation
