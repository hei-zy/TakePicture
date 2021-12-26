#pragma once
#include"TargetDetection.h"

class CircleGridDetection :public TargetDetection
{
public:
	CircleGridDetection();
	virtual ~CircleGridDetection();
	std::vector<cv::Point2d> getTarget(cv::Mat& src)override;
public:
	/// <summary>
	/// ��ɫ�ָ����rgb�������
	/// </summary>
	/// <param name="src"></param>
	/// <returns></returns>
	cv::Mat colorSegmention(cv::Mat& src);
};