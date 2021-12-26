#pragma once
#include"TargetDetection.h"

class CircleGridDetection :public TargetDetection
{
public:
	CircleGridDetection();
	virtual ~CircleGridDetection();
	std::vector<cv::Point2d> getPoints(cv::Mat& src)override;
private:
	/// <summary>
	/// 颜色分割，输入rgb，输出出
	/// </summary>
	/// <param name="src"></param>
	/// <returns></returns>
	cv::Mat colorSegmention(cv::Mat& src);

	/// <summary>
	/// 由椭圆拟合获得靶点坐标
	/// </summary>
	/// <param name="maskImg"></param>
	/// <param name="src"></param>
	/// <param name="centers"></param>
	void findTargetByEllipse(cv::Mat& maskImg, cv::Mat& src, std::vector<cv::Point2d>& centers);

	/// <summary>
	/// 由图像矩获得靶点坐标
	/// </summary>
	/// <param name="maskImg"></param>
	/// <param name="src"></param>
	/// <param name="centers"></param>
	void findTargetByMomnent(cv::Mat& maskImg, cv::Mat& src, std::vector<cv::Point2d>& centers);

	/// <summary>
	/// 增强角点亮度
	/// </summary>
	void pointEnhance(cv::Mat& src, cv::Mat& dst);

	/// <summary>
	/// findCirclesCrid的封装
	/// </summary>
	/// <param name="src">输入图像</param>
	/// <param name="centers">输出目标点</param>
	void findCirclesGrid(cv::Mat& src, std::vector<cv::Point2d>& centers);
};