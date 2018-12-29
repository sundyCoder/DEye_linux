/*
* Author: sundycoder@gmail.com
* Date: 20181229
*/

#ifndef TF_DETECTOR_EXAMPLE_UTILS_H
#define TF_DETECTOR_EXAMPLE_UTILS_H


#include <vector>
#include <string>
#include "tensorflow/core/framework/tensor.h"
#include "tensorflow/core/public/session.h"
#include <opencv2/core/mat.hpp>


using tensorflow::Tensor;
using tensorflow::Status;
using tensorflow::string;


Status readLabelsMapFile(const string &fileName, std::map<int, string> &labelsMap);

Status loadGraph(const string &graph_file_name,
                 std::unique_ptr<tensorflow::Session> *session);

Status readTensorFromMat(const cv::Mat &mat, Tensor &outTensor);
Tensor readTensorFromMat(const cv::Mat &mat);


Status mat2Tensor(const cv::Mat& mat, Tensor& outTensor);

void drawBoundingBoxOnImage(cv::Mat &image, double xMin, double yMin, double xMax, double yMax, double score, std::string label, bool scaled);

void drawBoundingBoxesOnImage(cv::Mat &image,
                              tensorflow::TTypes<float>::Flat &scores,
                              tensorflow::TTypes<float>::Flat &classes,
                              tensorflow::TTypes<float,3>::Tensor &boxes,
                              std::map<int, string> &labelsMap,
                              std::vector<size_t> &idxs);

double IOU(cv::Rect box1, cv::Rect box2);

std::vector<size_t> filterBoxes(tensorflow::TTypes<float>::Flat &scores,
                                tensorflow::TTypes<float, 3>::Tensor &boxes,
                                double thresholdIOU, double thresholdScore);
                        

std::string myTrim(const std::string& str, std::string& whitespace);

void mySplit(const std::string& s, char delim, std::vector<std::string>& elems);

#endif //TF_DETECTOR_EXAMPLE_UTILS_H
