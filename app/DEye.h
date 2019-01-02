/************************************************************************
*  Author: Sundy
*  E-mail: sundycoder@gmail.com
*  Date:  Jan 01,2019
************************************************************************/

#pragma once

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#ifdef _WIN32
#define __DLLEXPORT __declspec(dllexport)
#else
#define __DLLEXPORT __attribute__((visibility("default")))
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef void* DEyeNet;

struct DEFECT{
    std::string type;
    float score;
    cv::Rect defectRect;
};

__DLLEXPORT int loadDEyeNet(DEyeNet* dEyeNet_p);
__DLLEXPORT int detect(const DEyeNet dEyeNet_p, cv::Mat& frame, std::vector<DEFECT>& defects);

#ifdef __cplusplus
}
#endif
