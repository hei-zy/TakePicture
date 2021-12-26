#pragma once
#include<string>
#include<vector>
#include<opencv.hpp>
#include<io.h>
#include<qobject.h>

enum CAL_METHOD {
	WITH_CIRCLE,
	WITH_CORNER
};
enum CAL_ERRO {

};
class Calibration:public QObject
{
	Q_OBJECT
public:
	Calibration();
	~Calibration();
	int singleCameraCalibration(std::vector<std::string> address, cv::Size boardSize,cv::Point2f squareSize, CAL_METHOD method=WITH_CORNER);
	void getFiles(std::string path, std::vector<std::string>& files);
public:
	void dowork(std::string path);
signals:
	void camCalibFinished(const QString& str);
};
