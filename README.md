# tensorflow-object-detection-cpp
A C++ example of running TensorFlow Object Detection model in live mode.
Inspired by [issue](https://github.com/tensorflow/models/issues/1741#issuecomment-318613222).

## Requirements (for use without Bazel):
* `TensorFlow` .so builds ([more](https://github.com/tensorflow/tensorflow/issues/2412#issuecomment-300628873), requires Bazel to build)
* `Eigen3` headers ([more](http://eigen.tuxfamily.org/index.php?title=Main_Page))
* `OpenCV` ([more](https://github.com/opencv/opencv))

## Usage:
1. Specify your own paths for necessary libs in `CmakeLists.txt`
2. Specify your own paths for `frozen_inference_graph.pb` and `labels_map.pbtxt` in `main.cpp` (lines 44-47)
3. Specify your video source (`main.cpp`, line 80)
4. Have fun

`demo/` dir contains frozen graph & labels map from [victordibia/handstracking](https://github.com/victordibia/handtracking) as an example. 

## Manual:
### 0. environment
    cuda9.0, cudnn7.3， ubuntu18.04, gcc6.5
### 1. Install Protobuf
    sudo apt-get install autoconf automake libtool curl make unzip
    tar -xvf protobuf-all-3.6.0.tar
    cd protoc-3.6
    ./autogen.sh
    ./configure
    make && sudo make install
    sudo ldconfig

### 2.Install Eigen
    # Download eigen 3.3.4 from https://github.com/eigenteam/eigen-git-mirror/releases
    tar -xvf eigen-eigen-b3f3d4950030.tar.bz2
    cd eigen-git-mirror
    mkdir build && cd build
    cmake ..
    make
    sudo make install

### 3.Install TensorFlow
    rm -fr ~/.cache/bazel*
    bazel clean
    a. ./configure
    b. build
        // build with avx2 support
        bazel build -c opt --copt=-mavx --copt=-mavx2 --copt=-mfma --copt=-mfpmath=both --copt=-msse4.2 --config=monolithic //tensorflow:libtensorflow_cc.so
        // build with no avx2 support
        bazel build -c opt --copt=-mfma --copt=-mfpmath=both --copt=-msse4.2 --config=monolithic //tensorflow:libtensorflow_cc.so
    c. Then Copy the following include headers and dynamic shared library to /usr/local/lib and /usr/local/include:
        mkdir /usr/local/include/tf
        cp -r bazel-genfiles/ /usr/local/include/tf/
        cp -r tensorflow /usr/local/include/tf/
        cp -r third_party /usr/local/include/tf/
        cp -r bazel-bin/libtensorflow_cc.so /usr/local/lib/
        
        cp tensorflow/contrib/makefile/downloads/ /usr/local/include/tf/tensorflow/contrib/makefile/ -r

    d. compile
        g++ -std=c++11 -o tLoader -I/usr/local/include/tf -I/usr/local/include/eigen3 -g -Wall -D_DEBUG -Wshadow -Wno-sign-compare -w  -L/usr/local/lib/libtensorflow_cc `pkg-config --cflags --libs protobuf`  -ltensorflow_cc loader.cpp

### 4. (Optional)Install/Update cmake
    tar xcvf cmake-3.11.1-Linux-x86_64.tar.gz
    cd cmake-3.11.1-Linux-x86_64
    sudo apt-get purge cmake
    sudo cp -r bin /usr/
    sudo cp -r share /usr/
    sudo cp -r doc /usr/share/
    sudo cp -r man /usr/share/

### 5. install Opencv
    sudo apt-get install build-essential checkinstall cmake pkg-config yasm
    sudo apt-get install git gfortran
    sudo apt-get install libjpeg8-dev libjasper-dev libpng12-dev
    sudo apt-get install libtiff5-dev
    sudo apt-get install libavcodec-dev libavformat-dev libswscale-dev libdc1394-22-dev
    sudo apt-get install libxine2-dev libv4l-dev
    sudo apt-get install libgstreamer0.10-dev libgstreamer-plugins-base0.10-dev
    sudo apt-get install qt5-default libgtk2.0-dev libtbb-dev
    sudo apt-get install libatlas-base-dev
    sudo apt-get install libfaac-dev libmp3lame-dev libtheora-dev
    sudo apt-get install libvorbis-dev libxvidcore-dev
    sudo apt-get install libopencore-amrnb-dev libopencore-amrwb-dev
    sudo apt-get install x264 v4l-utils
    sudo apt-get install python-dev python-pip python3-dev python3-pip
    sudo pip3 install numpy scipy matplotlib scikit-image scikit-learn ipython
    cd ~
    git clone https://github.com/opencv/opencv.git
    cd opencv && git checkout 3.4.1
    cd ~
    git clone https://github.com/opencv/opencv_contrib.git
    cd opencv_contrib && git checkout 3.4.1
    cd ~/opencv && mkdir build && cd build
    cmake -D CMAKE_BUILD_TYPE=RELEASE \
        -D CMAKE_INSTALL_PREFIX=/usr/local \
        -D INSTALL_C_EXAMPLES=ON \
        -D INSTALL_PYTHON_EXAMPLES=ON \
        -D WITH_TBB=ON \
        -D WITH_V4L=ON \
        -D OPENCV_EXTRA_MODULES_PATH=~/opencv_contrib/modules \
        -D BUILD_EXAMPLES=ON ..
    nproc
    # substitute 4 by output of nproc
    make -j4
    sudo make install
    sudo sh -c 'echo "/usr/local/lib" >> /etc/ld.so.conf.d/opencv.conf'
    sudo ldconfig

### 6. Testing

    a. build libDEye.so
        mkdir build
        cd build
        cmake ..
        make
        cp libDEye.so /usr/local/lib
    b. build application
        cd app
        mkdir build
        cd build 
        cmake ..
        make

### 7.Reference
    https://github.com/lysukhin/tensorflow-object-detection-cpp 
    https://tuatini.me/building-tensorflow-as-a-standalone-project/
    https://medium.com/@fanzongshaoxing/tensorflow-c-api-to-run-a-object-detection-model-4d5928893b02
    https://gist.github.com/kyrs/9adf86366e9e4f04addb
    https://medium.com/@fanzongshaoxing/tensorflow-c-api-to-run-a-object-detection-model-4d5928893b02
    windows: https://github.com/hluu11/SimpleTF-CPP
