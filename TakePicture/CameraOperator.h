#pragma once
#include"GalaxyIncludes.h"
#include"CSampleCaptureHandler.h"
#include<iostream>
#include<QObject>


enum class CamereParameters {
	BALANCE_WHITE_NONE = 0,
	BALANCE_WHITE_AUTO = 1,
	SHUTTER_VALUE_50000 = 50000,
	SHUTTER_VALUE_40000 = 40000,
	SHUTTER_VALUE_30000 = 30000,
	SHUTTER_VALUE_20000 = 20000,
	SHUTTER_VALUE_10000 = 10000
};

//----------------------------------------------------------------------------------
/**
\brief     判断是否兼容
\param     pBmpInfo BITMAPINFO指针
\param     nWidth 图像宽
\param     nHeight 图像高
\return    true为一样，false不一样
*/
//----------------------------------------------------------------------------------
bool __IsCompatible(BITMAPINFO* pBmpInfo, uint64_t nWidth, uint64_t nHeight);
//用户继承掉线事件处理类
class CSampleDeviceOfflineEventHandler : public IDeviceOfflineEventHandler
{
public:
	void DoOnDeviceOfflineEvent(void* pUserParam)
	{
		std::cout << "收到设备掉线事件!" << std::endl;
	}
};



//用户继承属性更新事件处理类
class CSampleFeatureEventHandler : public IFeatureEventHandler
{
public:
	void DoOnFeatureEvent(const GxIAPICPP::gxstring& strFeatureName, void* pUserParam)
	{
		std::cout << "收到曝光结束事件!" << std::endl;
	}
};



//用户继承采集事件处理类
class CameraOperator:public QObject
{
	Q_OBJECT
public:
	CameraOperator(QObject*parent=nullptr);
	~CameraOperator();
public:
	//打开
	struct cameraData
	{
		cameraData() = delete;
		cameraData(double d) :EPSTime(d) {}
		double EPSTime;
	};
	cameraData openCamera();
	//关闭
	void closeCamera();
	//处理函数
	void setBalance();
	bool takePicture();
	void setExposure(double exposureTime);

	bool isFinished = false;
public:
	CSampleCaptureHandler* pCaptureEventHandler;///<采集回调对象
private:
	IDeviceOfflineEventHandler* pDeviceOfflineEventHandler;///<掉线事件回调对象
	IFeatureEventHandler* pFeatureEventHandler;///<远端设备事件回调对象
												  //初始化
	CGXDevicePointer ObjDevicePtr;
	CGXStreamPointer ObjStreamPtr;
	CGXFeatureControlPointer ObjFeatureControlPtr;
	CGXFeatureControlPointer ObjStreamFeatureControlPtr;

	//GX_FEATURE_CALLBACK_HANDLE hFeatureEvent;
	GX_DEVICE_OFFLINE_CALLBACK_HANDLE hDeviceOffline;
	bool isOpenWhiteBalance = false;
	bool m_bTriggerMode;
};

