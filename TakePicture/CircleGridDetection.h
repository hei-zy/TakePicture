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
	/// ��ɫ�ָ����rgb�������
	/// </summary>
	/// <param name="src"></param>
	/// <returns></returns>
	cv::Mat colorSegmention(cv::Mat& src);

	/// <summary>
	/// ����Բ��ϻ�ðе�����
	/// </summary>
	/// <param name="maskImg"></param>
	/// <param name="src"></param>
	/// <param name="centers"></param>
	void findTargetByEllipse(cv::Mat& maskImg, cv::Mat& src, std::vector<cv::Point2d>& centers);

	/// <summary>
	/// ��ͼ��ػ�ðе�����
	/// </summary>
	/// <param name="maskImg"></param>
	/// <param name="src"></param>
	/// <param name="centers"></param>
	void findTargetByMomnent(cv::Mat& maskImg, cv::Mat& src, std::vector<cv::Point2d>& centers);

	/// <summary>
	/// ��ǿ�ǵ�����
	/// </summary>
	void pointEnhance(cv::Mat& src, cv::Mat& dst);

	/// <summary>
	/// findCirclesCrid�ķ�װ
	/// </summary>
	/// <param name="src">����ͼ��</param>
	/// <param name="centers">���Ŀ���</param>
	void findCirclesGrid(cv::Mat& src, std::vector<cv::Point2d>& centers);
};