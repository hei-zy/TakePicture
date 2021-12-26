#pragma once
#include<vector>
#include<opencv2/opencv.hpp>

class TargetDetection
{
public:
	TargetDetection();
	virtual std::vector<cv::Point2d> getTarget(cv::Mat& src) = 0;
	virtual ~TargetDetection();
};