#pragma once
#include<opencv2/opencv.hpp>

namespace PARA {
	const cv::Scalar lowRedR(0, 150, 160);
	const cv::Scalar highRedR(10, 255.0, 255.0);
	const cv::Scalar lowRedL(170, 150, 160);
	const cv::Scalar highRedL(180, 255.0, 255.0);

	const int MAXDISTANCE = 10;
	const int MINPOINTS = 20;
	const double proportionalityCoefficient = 0.05;
	const double maxCoefficient = 0.5;
}