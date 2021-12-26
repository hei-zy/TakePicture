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
	/// 颜色分割，输入rgb，输出出
	/// </summary>
	/// <param name="src"></param>
	/// <returns></returns>
	cv::Mat colorSegmention(cv::Mat& src);
};