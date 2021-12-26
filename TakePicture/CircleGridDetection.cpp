#include "CircleGridDetection.h"
#include"Para.h"

CircleGridDetection::CircleGridDetection()
{
}

CircleGridDetection::~CircleGridDetection()
{
}

std::vector<cv::Point2d> CircleGridDetection::getTarget(cv::Mat& src)
{
	return std::vector<cv::Point2d>();
}

cv::Mat CircleGridDetection::colorSegmention(cv::Mat& src)
{
	//判断图像是否正确读取
	if (src.empty())
	{
		std::cout << "image load failed" << std::endl;
		return cv::Mat();
	}
	//高斯模糊去除噪点
	cv::Mat imgProcess, imgHSV;
	cv::GaussianBlur(src, imgProcess, cv::Size(3, 3), 1);
	//灰度
	cv::Mat imgGray;
	cv::cvtColor(imgProcess, imgGray, cv::COLOR_BGR2GRAY);

	//色彩变幻
	cv::cvtColor(imgProcess, imgHSV, cv::COLOR_BGR2HSV);
	//分割
	cv::Mat maskR, maskL, mask;
	cv::inRange(imgHSV, PARA::lowRedR, PARA::highRedR, maskR);
	cv::inRange(imgHSV, PARA::lowRedL, PARA::highRedL, maskL);
	cv::add(maskL, maskR, mask);

	//形态学处理
	cv::Mat kernel = cv::getStructuringElement(cv::MORPH_CROSS, cv::Size(3, 3));
	cv::Mat kernel2 = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(11, 11));
	cv::Mat kernel3 = cv::getStructuringElement(cv::MORPH_CROSS, cv::Size(5, 5));

	cv::Mat imgThre, imgDial;
	cv::erode(mask, imgThre, kernel);
	//cv::erode(imgThre, imgThre, kernel3);
	cv::dilate(imgThre, imgDial, kernel2);
	for (int i = 0; i < 3; ++i) {
		cv::dilate(imgDial, imgDial, kernel2);
	}
	//拷贝
	cv::Mat maskImg;
	src.copyTo(maskImg, imgDial);
	cv::namedWindow("maskImage", cv::WINDOW_NORMAL);
	cv::imshow("maskImage", imgDial);
	cv::waitKey(1);
	return imgDial;
}
