#if 0
#include "TakePicture.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qRegisterMetaType<std::string>("std::string");
    TakePicture w;
    w.show();
    return a.exec();
}

#endif

#if 1
#include<opencv2/opencv.hpp>
#include<iostream>
#include"CircleGridDetection.h"
using namespace std;

using namespace cv;

int main(int argc, char* argv[])
{
    string address = "./image/9.bmp";
    cv::Mat src = cv::imread(address);
    CircleGridDetection* cd = new CircleGridDetection;
    auto dst = cd->getPoints(src);

    
    return 0;
}
 
#endif