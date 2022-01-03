#include "CircleGridDetection.h"
#include"Para.h"

CircleGridDetection::CircleGridDetection()
{
}

CircleGridDetection::~CircleGridDetection()
{
}

std::vector<cv::Point2d> CircleGridDetection::getPoints(cv::Mat& src)
{
	auto mask = imgSegmention(src);
	cv::imshow("imgSegmention", mask);
	std::vector<cv::Point2d> centers;
	findTargetByEllipse(mask, src, centers);
	return centers;
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
	cv::Mat imgGauss, imgHSV;
	cv::GaussianBlur(src, imgGauss, cv::Size(3, 3), 1);

	cv::Mat dst_kernel;
	pointEnhance(imgGauss, dst_kernel);
	//灰度
	cv::Mat imgGray;
	cv::cvtColor(dst_kernel, imgGray, cv::COLOR_BGR2GRAY);

	//色彩变幻
	cv::cvtColor(dst_kernel, imgHSV, cv::COLOR_BGR2HSV);
	cv::Mat imgHLS;
	cv::cvtColor(dst_kernel, imgHLS, cv::COLOR_BGR2HLS);
	//分割
	cv::Mat maskR, maskL, mask;
	cv::inRange(imgHSV, PARA::lowRedR, PARA::highRedR, maskR);
	cv::inRange(imgHSV, PARA::lowRedL, PARA::highRedL, maskL);
	cv::add(maskL, maskR, mask);

	//形态学处理
	cv::Mat kernel1 = cv::getStructuringElement(cv::MORPH_CROSS, cv::Size(3, 3));
	cv::Mat kernel2 = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(11, 11));
	cv::Mat kernel3 = cv::getStructuringElement(cv::MORPH_CROSS, cv::Size(5, 5));

	cv::Mat imgThre, imgDial;
	cv::erode(mask, imgThre, kernel1);
	cv::dilate(imgThre, imgThre, kernel1);
	cv::dilate(imgThre, imgDial, kernel2);
	cv::erode(imgDial, imgDial, kernel2);

	//拷贝
	cv::Mat maskImg;
	src.copyTo(maskImg, imgDial);
	cv::namedWindow("maskImage", cv::WINDOW_NORMAL);
	cv::imshow("maskImage", imgDial);
	cv::waitKey(1);
	return imgDial;
}



cv::Mat CircleGridDetection::imgSegmention(cv::Mat& src)
{
	//判断图像是否正确读取
	if (src.empty())
	{
		std::cout << "image load failed" << std::endl;
		return cv::Mat();
	}
	//高斯模糊去除噪点
	cv::Mat imgGauss;
	cv::medianBlur(src, imgGauss, 3);
	cv::GaussianBlur(imgGauss, imgGauss, cv::Size(3, 3), 0, 0);
	//灰度
	cv::Mat imgGray;
	cv::cvtColor(imgGauss, imgGray, cv::COLOR_BGR2GRAY);
	//归一化
	cv::normalize(imgGray, imgGray, 0, 255, cv::NORM_MINMAX);
	//阈值分割
	cv::Mat imgBi;
	cv::threshold(imgGray, imgBi, 100, 255, cv::THRESH_BINARY);
	
	//边缘检测
	cv::Mat imgMargin;
	int cannyThresh = 255;
	cv::Canny(imgGray, imgMargin, cannyThresh * 2 / 3, cannyThresh, 3, false);
	cv::Mat kernel = cv::Mat::ones(5, 5, CV_8UC1);
	cv::dilate(imgMargin, imgMargin, kernel);
	cv::erode(imgMargin, imgMargin, kernel);
	//轮廓查找
	std::vector<std::vector<cv::Point>> contours;
	cv::findContours(imgMargin, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
	cv::Mat markers = cv::Mat::zeros(src.size(), CV_32SC1);
	for (int i = 0; i < contours.size(); ++i) {
		cv::drawContours(markers, contours, i, cv::Scalar::all(255), -1);
	}
	markers.convertTo(markers, CV_8UC1);


	return markers;
}

void CircleGridDetection::findTargetByEllipse(cv::Mat& maskImg, cv::Mat& src, std::vector<cv::Point2d>& centers)
{
	if (maskImg.empty() || src.empty())
		return;
	//改成灰度图
	cv::Mat imgGray;
	if (maskImg.channels() == 3) {
		cv::cvtColor(maskImg, imgGray, cv::COLOR_BGR2GRAY);
	}
	else {
		imgGray = maskImg;
	}
	//canny--findcontours
	//寻找边缘
	std::vector<std::vector<cv::Point> > contours;
	cv::findContours(imgGray, contours, cv::RETR_LIST, cv::CHAIN_APPROX_SIMPLE);
	////绘制边缘
	cv::Mat dst;
	dst = src.clone();
	for (size_t i = 0; i < contours.size(); i++)
	{
		//Scalar color(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
		cv::Scalar color(255, 0, 0);
		drawContours(dst, contours, i, color, 1, cv::LINE_AA);
	}
	imshow("drawContours", dst);

	//椭圆拟合
	std::vector<cv::RotatedRect> ellipses;
	for (auto& contour : contours) {
		if (contours.size() > 7) {
			auto ellipse = cv::fitEllipseAMS(contour);
			auto major = std::max(ellipse.size.width, ellipse.size.height);
			auto minor = std::min(ellipse.size.width, ellipse.size.height);
			if (major / minor < 3.0f) {
				centers.push_back(ellipse.center);
				ellipses.push_back(ellipse);
			}
		}
	}

	//draw
	cv::Mat dst2 = src.clone();
	//cv::Mat canvas = cv::Mat::zeros(src.size(), src.type());
	int i = 0;
	for (auto& ellipse : ellipses) {
		cv::ellipse(dst2, ellipse, cv::Scalar(0, 255, 0));
		cv::circle(dst2, centers[i], 1, cv::Scalar(255, 0, 0), -1);
		++i;
	}
	cv::imshow("centers_ellipse", dst2);
}

void CircleGridDetection::findTargetByMomnent(cv::Mat& maskImg, cv::Mat& src, std::vector<cv::Point2d>& centers)
{
	if (maskImg.empty() || src.empty())
		return;
	//改成灰度图
	cv::Mat imgGray;
	if (maskImg.channels() == 3) {
		cv::cvtColor(maskImg, imgGray, cv::COLOR_BGR2GRAY);
	}
	else {
		imgGray = maskImg;
	}
	//canny--findcontours
	//寻找边缘
	std::vector<std::vector<cv::Point> > contours;
	cv::findContours(imgGray, contours, cv::RETR_LIST, cv::CHAIN_APPROX_SIMPLE);
	////绘制边缘
	cv::Mat dst;
	dst = src.clone();
	for (size_t i = 0; i < contours.size(); i++)
	{
		//Scalar color(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
		cv::Scalar color(255, 255, 255);
		drawContours(dst, contours, i, color, 1, cv::LINE_AA);
	}
	imshow("drawContours", dst);

	//图像矩
	centers = std::vector<cv::Point2d>(contours.size());
	std::vector<cv::Moments> contours_moments(contours.size());
	for (size_t i = 0; i < contours.size(); ++i) {
		contours_moments[i] = cv::moments(contours[i]);
		centers[i] = cv::Point2d(static_cast<double>(contours_moments[i].m10 / contours_moments[i].m00),
			static_cast<double>(contours_moments[i].m01 / contours_moments[i].m00));
	}

	//draw
	cv::Mat dst2 = src.clone();
	int i = 0;
	for (size_t i = 0; i < contours.size();++i) {
		cv::circle(dst2, centers[i], 1, cv::Scalar(255, 0, 0), -1);
		++i;
	}
	cv::imshow("centers_moments", dst2);
}

void CircleGridDetection::pointEnhance(cv::Mat& src, cv::Mat& dst)
{
	cv::Mat kernel = (cv::Mat_<char>(3, 3) << 0, -1, 0, -1, 5, -1, 0, -1, 0);
	filter2D(src, dst, src.depth(), kernel);
}

void CircleGridDetection::findCirclesGrid(cv::Mat& src, std::vector<cv::Point2d>& centers)
{
	cv::SimpleBlobDetector::Params params;
	params.minThreshold = 10;		//***二值化的起始阈值，即公式1的T1
	params.maxThreshold = 150;		//最大二值化值
	params.thresholdStep = 20;		//阈值越小检测越精确，计算速度变慢
	params.filterByInertia = false;	//斑点圆度的限制变量，默认是不限制
	params.filterByColor = true;	//斑点颜色的限制变量
	params.blobColor = 255;			//255表示只提取白色斑点，0表示只提取黑色斑点
	params.filterByArea = false;		//斑点面积的限制变量
	params.minArea = 20;			//斑点的最小面积最大取到18000
	params.maxArea = 15000;
	//最小的斑点距离，不同二值图像的斑点间距离小于该值时，
	//被认为是同一个位置的斑点，否则是不同位置上的斑点
	params.minDistBetweenBlobs = 15;//最大22左右,15比较合适
	cv::Ptr<cv::FeatureDetector> blobDetector = cv::SimpleBlobDetector::create(params);
	cv::Size patternSize(5, 5);
	bool found = cv::findCirclesGrid(src, patternSize, centers, cv::CALIB_CB_SYMMETRIC_GRID | cv::CALIB_CB_CLUSTERING, blobDetector);
}
