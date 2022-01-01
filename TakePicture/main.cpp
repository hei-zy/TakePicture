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
    
    cv::Mat mm(cv::Size(5, 5), CV_8UC3, cv::Scalar(222,0, 0));
    cv::imshow("BGR", mm);
    cv::Mat imgHsv;
    cv::cvtColor(mm, imgHsv, cv::COLOR_BGR2HSV);
    cv::imshow("HSV", imgHsv);

    cv::waitKey(0);
    return 0;
}

#endif