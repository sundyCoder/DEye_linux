/*
* Author: sundycoder@gmail.com
* Date: 20181229
*/

#include <fstream>
#include <utility>
#include <vector>
#include <iostream>

#include "tensorflow/cc/ops/const_op.h"
#include "tensorflow/cc/ops/image_ops.h"
#include "tensorflow/cc/ops/standard_ops.h"
#include "tensorflow/core/framework/graph.pb.h"
#include "tensorflow/core/graph/default_device.h"
#include "tensorflow/core/graph/graph_def_builder.h"
#include "tensorflow/core/lib/core/threadpool.h"
#include "tensorflow/core/lib/io/path.h"
#include "tensorflow/core/lib/strings/stringprintf.h"
#include "tensorflow/core/platform/init_main.h"
#include "tensorflow/core/public/session.h"
#include "tensorflow/core/util/command_line_flags.h"

#include <opencv2/core/mat.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>


#include <time.h>
#include "include/utils.h"

using tensorflow::Flag;
using tensorflow::Tensor;
using tensorflow::Status;
using tensorflow::string;
using tensorflow::int32;
using namespace tensorflow;
using namespace tensorflow::ops;

using namespace std;
using namespace cv;

int main(int argc, char* argv[]) {

    // Set dirs variables
    string ROOTDIR = "../model/fasterRCNN/";
    string LABELS = "labels_map.pbtxt";
    string GRAPH = "frozen_inference_graph.pb";

    // Set input & output nodes names
    string inputLayer = "image_tensor:0";
    vector<string> outputLayer = {"detection_boxes:0", "detection_scores:0", "detection_classes:0", "num_detections:0"};

    // Load and initialize the model from .pb file
    std::unique_ptr<tensorflow::Session> session;
    string graphPath = tensorflow::io::JoinPath(ROOTDIR, GRAPH);
    LOG(INFO) << "graphPath:" << graphPath;
    Status loadGraphStatus = loadGraph(graphPath, &session);
    if (!loadGraphStatus.ok()) {
        LOG(ERROR) << "loadGraph(): ERROR" << loadGraphStatus;
        return -1;
    } else
        LOG(INFO) << "loadGraph(): frozen graph loaded" << endl;


    // Load labels map from .pbtxt file
    std::map<int, std::string> labelsMap = std::map<int,std::string>();
    Status readLabelsMapStatus = readLabelsMapFile(tensorflow::io::JoinPath(ROOTDIR, LABELS), labelsMap);
    if (!readLabelsMapStatus.ok()) {
        LOG(ERROR) << "readLabelsMapFile(): ERROR" << loadGraphStatus;
        return -1;
    } else{
        LOG(INFO) << "readLabelsMapFile(): labels map loaded with " << labelsMap.size() << " label(s)" << endl;
    }

    Mat frame;
    //Tensor tensor;
    std::vector<Tensor> outputs;
    double thresholdScore = 0.5;
    double thresholdIOU = 0.8;

    // FPS count
    double fps = 0.;
    time_t start, end;
    time(&start);

    frame = cv::imread("../demo.png",IMREAD_UNCHANGED);
    int height = frame.rows;
    int width = frame.cols;
    int channels = frame.channels();
    if ( frame.empty() ){
     LOG(INFO) << "Read image error or not found!" << endl;
    }
    LOG(INFO) << frame.rows << ","<<frame.cols<<","<<frame.channels() <<","<<frame.type()<< endl;
    //cvtColor(frame, frame, COLOR_BGR2RGB);

    //tensorflow::TensorShape shape = tensorflow::TensorShape();
    tensorflow::Tensor tensor;
    tensor = Tensor(tensorflow::DT_FLOAT, TensorShape({ 1,height,width,channels })); //3, height, width, dept
   tensor =  readTensorFromMat(frame);

    // Run the graph on tensor
    outputs.clear();
    Status runStatus = session->Run({{inputLayer, tensor}}, outputLayer, {}, &outputs);
    if (!runStatus.ok()) {
        LOG(ERROR) << "Running model failed: " << runStatus;
        return -1;
    }else{
        LOG(INFO) << "Running model successfully. " << runStatus;
    }

    // Extract results from the outputs vector
    tensorflow::TTypes<float>::Flat scores = outputs[1].flat<float>();
    tensorflow::TTypes<float>::Flat classes = outputs[2].flat<float>();
    tensorflow::TTypes<float>::Flat numDetections = outputs[3].flat<float>();
    tensorflow::TTypes<float, 3>::Tensor boxes = outputs[0].flat_outer_dims<float,3>();

    vector<size_t> goodIdxs = filterBoxes(scores, boxes, thresholdIOU, thresholdScore);
    LOG(INFO) << "detected: "<< goodIdxs.size() <<endl;
    for (size_t i = 0; i < goodIdxs.size(); i++)
        LOG(INFO) << "score:" << scores(goodIdxs.at(i)) << ",class:" << labelsMap[classes(goodIdxs.at(i))]
                    << " (" << classes(goodIdxs.at(i)) << "), box:" << "," << boxes(0, goodIdxs.at(i), 0) << ","
                    << boxes(0, goodIdxs.at(i), 1) << "," << boxes(0, goodIdxs.at(i), 2) << ","
                    << boxes(0, goodIdxs.at(i), 3);

    // Draw bboxes and captions
    cvtColor(frame, frame, COLOR_BGR2RGB);
    drawBoundingBoxesOnImage(frame, scores, classes, boxes, labelsMap, goodIdxs);

    putText(frame, to_string(fps).substr(0, 5), Point(0, frame.rows), FONT_HERSHEY_SIMPLEX, 0.7, Scalar(255, 255, 255));
    //cv::imwrite("result.png",frame);
    return 0;
}

