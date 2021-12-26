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
	//�ж�ͼ���Ƿ���ȷ��ȡ
	if (src.empty())
	{
		std::cout << "image load failed" << std::endl;
		return cv::Mat();
	}
	//��˹ģ��ȥ�����
	cv::Mat imgProcess, imgHSV;
	cv::GaussianBlur(src, imgProcess, cv::Size(3, 3), 1);
	//�Ҷ�
	cv::Mat imgGray;
	cv::cvtColor(imgProcess, imgGray, cv::COLOR_BGR2GRAY);

	//ɫ�ʱ��
	cv::cvtColor(imgProcess, imgHSV, cv::COLOR_BGR2HSV);
	//�ָ�
	cv::Mat maskR, maskL, mask;
	cv::inRange(imgHSV, PARA::lowRedR, PARA::highRedR, maskR);
	cv::inRange(imgHSV, PARA::lowRedL, PARA::highRedL, maskL);
	cv::add(maskL, maskR, mask);

	//��̬ѧ����
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
	//����
	cv::Mat maskImg;
	src.copyTo(maskImg, imgDial);
	cv::namedWindow("maskImage", cv::WINDOW_NORMAL);
	cv::imshow("maskImage", imgDial);
	cv::waitKey(1);
	return imgDial;
}
