
#include "include/detect.h"
#include "include/utils.h"
#include <iostream>
using namespace std;
using namespace cv;


int Detector::loadModel(std::string modelPath, std::string labelMapPath){
    LOG(INFO) << "Loading mode..." << endl;
    Status loadGraphStatus = loadGraph(modelPath, &session);
    if (!loadGraphStatus.ok()) {
        LOG(ERROR) << "loadModel(): ERROR" << loadGraphStatus;
        return 0;
    } else{
        LOG(INFO) << "DEye model loaded" << endl;
    }

    // Load labels map from .pbtxt file
    Status readLabelsMapStatus = readLabelsMapFile(labelMapPath,labelsMap);
    if (!readLabelsMapStatus.ok()) {
        LOG(ERROR) << "readLabelsMapFile(): ERROR" << loadGraphStatus;
        return 0;
    }

    return 1;
}

int Detector::detect(cv::Mat& frame,std::vector<DEFECT>& defects){
    double thresholdScore = 0.5;
    double thresholdIOU = 0.7;
    if ( frame.empty() ){
     LOG(INFO) << "Read image error or not found!" << endl;
    }
    int height = frame.rows;
    int width = frame.cols;
    int channels = frame.channels();
    LOG(INFO) << frame.rows << ","<<frame.cols<<","<<frame.channels() <<","<<frame.type()<< endl;

    cvtColor(frame, frame, COLOR_BGR2RGB);
    std::vector<Tensor> outputs;
    tensorflow::Tensor tensor;
    tensor = Tensor(tensorflow::DT_FLOAT, TensorShape({1,height, width, channels })); //3, height, width, depth
    // Convert mat to tensor
    tensor =  readTensorFromMat(frame);
    // Run the graph on tensor
    outputs.clear();
    string inputLayer = "image_tensor:0";
    vector<string> outputLayer = {"detection_boxes:0", "detection_scores:0", "detection_classes:0", "num_detections:0"};
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
    //LOG(INFO) << "detected: "<< goodIdxs.size() <<endl;
    int numDef = goodIdxs.size();
    for (size_t i = 0; i < goodIdxs.size(); i++){
        // LOG(INFO) << "score:" << scores(goodIdxs.at(i)) << ",class:" << labelsMap[classes(goodIdxs.at(i))]
        //             << " (" << classes(goodIdxs.at(i)) << "), box:" << "," << boxes(0, goodIdxs.at(i), 0) << ","
        //             << boxes(0, goodIdxs.at(i), 1) << "," << boxes(0, goodIdxs.at(i), 2) << ","
        //             << boxes(0, goodIdxs.at(i), 3);
        DEFECT defect;
        defect.type =  labelsMap[classes(goodIdxs.at(i))];
        defect.score = floorf(scores(goodIdxs.at(i)) * 1000) / 1000;

        double yMin = boxes(0,goodIdxs.at(i),0);
        double xMin = boxes(0,goodIdxs.at(i),1);
        double yMax =  boxes(0,goodIdxs.at(i),2);
        double xMax =  boxes(0,goodIdxs.at(i),3);
        double score = scores(goodIdxs.at(i));

        int x = (int) (xMin * frame.cols);
        int y = (int) (yMin * frame.rows);
        int w = (int) (xMax * frame.cols) - x;
        int h = (int) (yMax * frame.rows) - y;

        cv::Rect rect(x, y, w , h);
        defect.defectRect = rect;
        defects.push_back(defect);
    }
    return numDef;
}