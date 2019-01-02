
#include "include/DEye.h"
#include "include/detect.hpp"


__DLLEXPORT int loadDEyeNet(DEyeNet* dEyeNet_p){
    std::string modelPath = "model.so";
    std::string labelMapPath = "model.map";
    auto defector = new Detector();
    if(defector){
        defector->loadModel(modelPath,labelMapPath);
        *dEyeNet_p = defector;
    }else{
        return -1;
    }

    return 1;
}

__DLLEXPORT int detect(const DEyeNet dEyeNet_p, cv::Mat& frame, std::vector<DEFECT>& defects){
    if (dEyeNet_p) {
        auto detector = static_cast<Detector*>(dEyeNet_p);
        int ret = detector->detect(frame, defects);
        return ret;
    }
    return -1;
}
