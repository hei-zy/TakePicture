#include "Calibration.h"
#include<iostream>
#include<opencv2\core\core.hpp>
#include<opencv2\imgproc\imgproc.hpp>
#include<opencv2\calib3d\calib3d.hpp>
#include<opencv2\highgui\highgui.hpp>
using namespace cv;
using namespace std;

Calibration::Calibration()
{
}

void Calibration::getFiles(string path, vector<string>& files)
{
	//文件句柄  
	__int64   hFile = 0;
	//文件信息  
	struct _finddata_t fileinfo;
	string p;
	if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1)
	{
		do
		{
			//如果是目录,迭代之  
			//如果不是,加入列表  
			if ((fileinfo.attrib & _A_SUBDIR))
			{
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
					getFiles(p.assign(path).append("\\").append(fileinfo.name), files);
			}
			else
			{
				files.push_back(p.assign(path).append("\\").append(fileinfo.name));
			}
		} while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
	}
}

void Calibration::dowork(std::string path)
{
	std::vector<std::string> files;
	getFiles(path, files);
	int size = singleCameraCalibration(files, cv::Size(8, 7), cv::Point2f(18.5, 18.5));
	QString str = "<font size = 3>CALIBRATE DONE! " + QString::number(size) + "</font>";
	emit camCalibFinished(str);
}


int Calibration::singleCameraCalibration(std::vector<std::string> address, cv::Size boardSize, cv::Point2f squareSize, CAL_METHOD method)
{
	Mat img;
	int imageCount = 0; 
	vector<vector<Point2f>> imagePointsSeqL;//所有的角点
	vector<vector<Point3f>> objectPoints;//所有标定板坐标系下的点
	vector<Point2f> corners;//单个图像角点
	Size imageSize;
	for (auto & path : address) {
		Mat imageInput = imread(path);
		if (imageInput.empty()) {
			cout << "cannnot read the image" << endl;
			continue;
		}
		Mat imgGray,imgThresh;
		cvtColor(imageInput, imgGray, COLOR_BGR2GRAY);
		threshold(imgGray, imgThresh, 100, 255, THRESH_TRUNC);

		if (imageCount == 0) {
			imageSize.width = imageInput.cols;
			imageSize.height = imageInput.rows;
			cout << "imageSize.width = " << imageSize.width << endl;
			cout << "imageSize.height" << imageSize.height << endl;
		}
		
		//寻找角点
		bool isFind = false;
		if (method == WITH_CORNER) {
			isFind = findChessboardCorners(imgGray, boardSize, corners, CALIB_CB_ADAPTIVE_THRESH | CALIB_CB_FAST_CHECK | CALIB_CB_NORMALIZE_IMAGE);
		}
		else if (method == WITH_CIRCLE) {
			isFind = findCirclesGrid(imgThresh, boardSize, corners, CALIB_CB_SYMMETRIC_GRID);
		}
		if (isFind == false) {
			cout << "can not find chessboard corners!" << endl; //找不到角点
			cout << "the adress of failed picture" << " " << path << endl;
		}
		else {
			imageCount++;
			cornerSubPix(imgGray, corners, Size(5, 5), Size(-1, -1), TermCriteria(TermCriteria::EPS + TermCriteria::COUNT, 30, 0.001));
			drawChessboardCorners(imageInput, boardSize, corners, isFind);
			namedWindow("chessboardL", WINDOW_NORMAL);
			imshow("chessboardL", imageInput);
			waitKey(1);
			imagePointsSeqL.push_back(corners);
		}
	}
	if (imagePointsSeqL.empty()) {
		return -1;
	}
	cout << "开始标定………………" << endl;
	int i, j, t;
	for (t = 0; t<imagePointsSeqL.size(); t++)//此处image_count图片index值
	{
		vector<Point3f> tempPointSet;
		for (i = 0; i<boardSize.height; i++)
		{
			for (j = 0; j<boardSize.width; j++)
			{
				Point3f realPoint;
				/* 假设标定板放在世界坐标系中z=0的平面上 */
				realPoint.x = (float)i*(float)(squareSize.x);
				realPoint.y = (float)j*(float)(squareSize.y);
				realPoint.z = 0;
				tempPointSet.push_back(realPoint);
			}
		}
		objectPoints.push_back(tempPointSet);
	}
	Mat cameraMatrix, distCoeffs;
	vector<Mat>rvecsMat;
	vector<Mat>tvecsMat;
	double esp = calibrateCamera(objectPoints, imagePointsSeqL, imageSize, cameraMatrix, distCoeffs, rvecsMat, tvecsMat);
	cout << "esp = " << esp << endl;
	FileStorage fs("intrinsics.yml", FileStorage::WRITE);
	if (fs.isOpened()) {
		fs << "M" << cameraMatrix << "D" << distCoeffs << "R" << rvecsMat[0] << "T" << tvecsMat[0];
		fs.release();
	}
	cout << "标定完成！\n";
	cout << "相机内参数矩阵"  << endl << cameraMatrix << endl;
	cout << "畸变系数"  << endl << distCoeffs << endl;
	return imagePointsSeqL.size();
}


Calibration::~Calibration()
{
}
