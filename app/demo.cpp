#include "DEye.h"
#include <iostream>

int main(int argc, char** argv){
    // 1. load DEye model.
    DEyeNet dEyeNet_p;
    int ret = loadDEyeNet(&dEyeNet_p);
    if(ret == -1){
        std::cout << "load DEye model failed!"<< std::endl;
        return -1;
    }

    // 2. read image(loop read image)
    cv::Mat inMat = cv::imread("demo.png",cv::IMREAD_UNCHANGED);
    if(inMat.empty()){
        std::cout<<"Image read failed or not found!"<<std::endl;
        return -1;
    }

    // 3. do detection
    std::vector<DEFECT> defects;
    ret = detect(dEyeNet_p, inMat, defects);
    if(ret != -1){
        int size = ret;
        std::cout<<"======size: " << size <<"======"<< std::endl;
        for(auto def: defects){
            std::cout<<def.type<<std::endl;
            std::cout<<def.score<<std::endl;
            std::cout<<def.defectRect.x<<","<<def.defectRect.y<<","<<def.defectRect.width<<","<<def.defectRect.height<<std::endl;
            cv::Point tl, br;
            tl =  cv::Point(def.defectRect.x, def.defectRect.y);
            br =  cv::Point(def.defectRect.x + def.defectRect.width, def.defectRect.y +  def.defectRect.height);
            cv::rectangle(inMat, tl, br, cv::Scalar(0, 255, 255), 1);
            std::string scoreString = std::to_string(def.score).substr(0, 5);
            std::string caption = def.type + " (" + scoreString + ")";
            cv::Point textCorner = cv::Point(tl.x, tl.y + 12 * 0.9);
            cv::putText(inMat, caption, textCorner, cv::FONT_HERSHEY_SIMPLEX, 0.4, cv::Scalar(255, 0, 0));
        }
        cv::imwrite("result.png",inMat);
    }
   return 1;
}
