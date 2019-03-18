/*
* Author: sundycoder@gmail.com
* Date: 20181229
*/

#ifndef TF_DETECT_H
#define TF_DETECT_H

#include "tensorflow/cc/ops/const_op.h"
//#include "tensorflow/cc/ops/standard_ops.h"
#include "tensorflow/core/framework/graph.pb.h"
#include "tensorflow/core/framework/tensor.h"
#include "tensorflow/core/graph/default_device.h"
#include "tensorflow/core/graph/graph_def_builder.h"
#include "tensorflow/core/lib/core/errors.h"
#include "tensorflow/core/lib/core/stringpiece.h"
#include "tensorflow/core/lib/core/threadpool.h"
#include "tensorflow/core/lib/io/path.h"
#include "tensorflow/core/lib/strings/stringprintf.h"
#include "tensorflow/core/platform/env.h"
#include "tensorflow/core/platform/init_main.h"
#include "tensorflow/core/platform/logging.h"
#include "tensorflow/core/platform/types.h"
#include "tensorflow/core/public/session.h"
#include "tensorflow/core/util/command_line_flags.h"
#include "image/kernels/image_ops.h"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "DEye.h"
#include <iostream>

using tensorflow::Flag;
using tensorflow::Tensor;
using tensorflow::Status;
using tensorflow::string;
using tensorflow::int32;
using namespace tensorflow;
using namespace tensorflow::ops;

// struct DEFECT{
//     std::string type;
//     float score;
//     cv::Rect defectRect;
// };

class Detector {
    std::unique_ptr<tensorflow::Session> session;
    std::map<int, std::string> labelsMap = std::map<int,std::string>();
    public:
        int mapLabel(std::string label);
        int loadModel(std::string modelPath, std::string labelMapPath);
        int freeModel();
        //int detect(Mat frame, double thresholdScore, double thresholdIOU, std::vector<std::vector<float>> &boxes, std::vector<float> &scores, std::vector<size_t> &labels);
        //int detect(Mat& frame, double thresholdScore, double thresholdIOU,std::vector<defectInfo>& defects);
        //int detect(cv::Mat& frame, std::vector<DEFECT>& defects);
        int detect(cv::Mat& frame, P_DEFECTS defects);
};

#endif //TF_DETECT_H
