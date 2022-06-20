# Tensorflow Object Detection
A C++ example of running TensorFlow Object Detection model.
<br>
<p>
DEye 🚀 please find the source code of windows version from the link: https://github.com/sundyCoder/DEye
</p>

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
    Step 1: 
        vim tensorflow/BUILD
        

	462 
	463 tf_cc_shared_object(
	464     #name = "libtensorflow.so",
	465     name = "libCore.so",
	466     linkopts = select({
	467         "//tensorflow:darwin": [
	468             "-Wl,-exported_symbols_list",  # This line must be directly followed by the exported_symbols.lds file
	469             "$(location //tensorflow/c:exported_symbols.lds)",
	470             "-Wl,-install_name,@rpath/libtensorflow.so",
	471         ],
	472         "//tensorflow:windows": [],
	473         "//conditions:default": [
	474             "-z defs",
	475             "-Wl,--version-script",  #  This line must be directly followed by the version_script.lds file
	476             "$(location //tensorflow/c:version_script.lds)",
	477         ],
	478     }),
	479     visibility = ["//visibility:public"],
                 
    Step 2: 
    
    rm -fr ~/.cache/bazel*
    bazel clean
    a. ./configure
    b. build
	bazel build -c opt --config=monolithic //tensorflow:libCore.so

    c. Then Copy the following include headers and dynamic shared library to /usr/local/lib and /usr/local/include:
        mkdir /usr/local/include/tf
        sudo cp -r bazel-genfiles/ /usr/local/include/tf/
        sudo cp -r tensorflow /usr/local/include/tf/
        sudo cp -r third_party /usr/local/include/tf/
        sudo cp -r bazel-bin/libCore.so /usr/local/lib/
        ./tensorflow/contrib/makefile/download_dependencies.sh
        sudo cp -r tensorflow/contrib/makefile/downloads /usr/local/include/tf/tensorflow/contrib/makefile/
	

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

    cmake -DCMAKE_CXX_FLAGS=-std=c++11 \
        -DCMAKE_BUILD_TYPE=RELEASE \
        -DCMAKE_INSTALL_PREFIX=./install \
        -DBUILD_EXAMPLES=OFF \
        -DBUILD_DOCS=OFF \
        -DBUILD_PERF_TESTS=OFF \
        -DBUILD_TESTS=OFF \
        -DINSTALL_C_EXAMPLES=OFF \
        -DENABLE_PRECOMPILED_HEADERS=OFF \
        -DWITH_OPENMP=ON \
        -DWITH_V4L=OFF \
        -DWITH_TBB=ON \
        -DWITH_QT=OFF \
        -DWITH_OPENGL=ON \
        -DWITH_JPEG=ON \
        -DWITH_FFMPEG=ON \
        -DWITH_GSTREAMER=ON \
        -DWITH_OPENCL=ON \
        -DWITH_GPHOTO2=OFF \
        -DWITH_LIBV4L=OFF \
        -DINSTALL_PYTHON_EXAMPLES=OFF \
        -DBUILD_SHARED_LIBS=ON \
        -DENABLE_CXX11=ON \
        -DOPENCV_EXTRA_MODULES_PATH=../../opencv_contrib/modules ..

        vim ~/.bashrc
        // add the content to .bashrc
        export OpenCV_DIR=/path/to/opencv4.0
        export OpenCV_INCLUDE_DIRS=/path/to/include/opencv4
        export OpenCV_LIBS=/path/to/opencv4.0/lib

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
	
    c. run app    
        cd app/build
	./demo


### 7. model encryption
    sh export.sh
    cp train_log/pb/frozen_inference_graph.pb TFSecured/python/
    python3 encrypt_model.py ./demo/models/frozen_inference_graph.pb ./model.so df6c8cd6696cfe35a6ea8dc14722132420181230
    
    'model.so' is generated.

### 8. Library
    Model file: model.so  model.map
    Core library: libDEye.so, libCore.so ( please contact with me if you want)

### 9.Reference
	@article{li2022eid,
	  title={EID-GAN: Generative Adversarial Nets for Extremely Imbalanced Data Augmentation},
	  author={Li, Wei and Chen, Jinlin and Cao, Jiannong and Ma, Chao and Wang, Jia and Cui, Xiaohui and Chen, Ping},
	  journal={IEEE Transactions on Industrial Informatics},
	  year={2022},
	  publisher={IEEE}
	}

    https://github.com/lysukhin/tensorflow-object-detection-cpp 
    https://tuatini.me/building-tensorflow-as-a-standalone-project/
    https://medium.com/@fanzongshaoxing/tensorflow-c-api-to-run-a-object-detection-model-4d5928893b02
    https://gist.github.com/kyrs/9adf86366e9e4f04addb
    https://medium.com/@fanzongshaoxing/tensorflow-c-api-to-run-a-object-detection-model-4d5928893b02
    windows: https://github.com/hluu11/SimpleTF-CPP
    https://github.com/sundyCoder/or/tree/master/models/research/tensorrt


