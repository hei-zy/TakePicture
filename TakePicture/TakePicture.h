#pragma once

////在头文件前面加上下面几行代码
//#ifdef WIN32  
//#pragma execution_character_set("utf-8")  
//#endif

#include <QtWidgets/QWidget>
#include "ui_TakePicture.h"
#include "CameraOperator.h"
#include<qstring.h>
#include<vector>
#include"Calibration.h"
#include<qmessagebox.h>
#include<qthread.h>

class TakePicture : public QWidget
{
    Q_OBJECT

public:
    TakePicture(QWidget *parent = Q_NULLPTR);
private:
    //初始化线程
    void initCamOpThread();
    void initCamCalibThread();
signals:
    void camImagePath(std::string path);
    void camCalibFinished(const QString&);
public slots:
    void openCamera();
    void getPicture();
    void setExposure();
    void closeCamera();
    void displayPicture(QImage input);
private:
    void connectSignalsSlots();
    void closeEvent(QCloseEvent* event);
private:
    Ui::TakePictureClass ui;
    CameraOperator* cameraOperator;
    QThread* thCameraOperator;
};