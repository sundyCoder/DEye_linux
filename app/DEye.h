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
typedef unsigned int UINT32;

/* defect struct*/
typedef struct _tag_Defect{
    UINT32 type;
    float score;
    cv::Rect defectRect;
}DEFECT, *P_DEFECT;

/* defect info for each image*/
typedef struct _tag_Defects{
    UINT32 size;
    DEFECT defects[100];
}DEFECTS,*P_DEFECTS;

__DLLEXPORT int loadDEyeNet(DEyeNet* dEyeNet_p);
__DLLEXPORT int detect(const DEyeNet dEyeNet_p, cv::Mat& frame, P_DEFECTS p_defects);

#ifdef __cplusplus
}
#endif
