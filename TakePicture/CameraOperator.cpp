#include "CameraOperator.h"
#include"CSampleCaptureHandler.h"
#include<QWidget>
#include<qmessagebox.h>
#include<thread>
#include<qdebug.h>

using namespace std;
CameraOperator::CameraOperator(QObject*parent):QObject(parent)
{
	//声明事件回调对象指针
	pDeviceOfflineEventHandler = nullptr;///<掉线事件回调对象
	pFeatureEventHandler = nullptr;///<远端设备事件回调对象
	pCaptureEventHandler = nullptr;///<采集回调对象
	hDeviceOffline = nullptr;
}

CameraOperator::~CameraOperator()
{
}

CameraOperator::cameraData CameraOperator::openCamera()
{
	IGXFactory::GetInstance().Init();
	isFinished = false;
	double EPSTime = 0.0;

	// TODO: Add your control notification handler code here
	bool bIsDeviceOpen = false;         ///< 设备是否打开标志
	bool bIsStreamOpen = false;         ///< 设备流是否打开标志
	try
	{
		do
		{
			//枚举设备
			gxdeviceinfo_vector vectorDeviceInfo;
			IGXFactory::GetInstance().UpdateDeviceList(1000, vectorDeviceInfo);
			if (0 == vectorDeviceInfo.size())
			{
				qDebug() << "无可用设备!";
				break;
			}

			//打开第一台设备以及设备下面第一个流
			ObjDevicePtr = IGXFactory::GetInstance().OpenDeviceBySN(
				vectorDeviceInfo[0].GetSN(),
				GX_ACCESS_EXCLUSIVE);
			bIsDeviceOpen = true;
			//获取远端设备属性控制器
			ObjFeatureControlPtr = ObjDevicePtr->GetRemoteFeatureControl();

			//判断设备流是否大于零，如果大于零则打开流
			int nStreamCount = ObjDevicePtr->GetStreamCount();

			if (nStreamCount > 0)
			{
				ObjStreamPtr = ObjDevicePtr->OpenStream(0);
				bIsStreamOpen = true;
			}
			else
			{
				throw exception("未发现设备流!");
			}

			// 建议用户在打开网络相机之后，根据当前网络环境设置相机的流通道包长值，
			// 以提高网络相机的采集性能,设置方法参考以下代码。
			GX_DEVICE_CLASS_LIST objDeviceClass = ObjDevicePtr->GetDeviceInfo().GetDeviceClass();
			if (GX_DEVICE_CLASS_GEV == objDeviceClass)
			{
				// 判断设备是否支持流通道数据包功能
				if (true == ObjFeatureControlPtr->IsImplemented("GevSCPSPacketSize"))
				{
					// 获取当前网络环境的最优包长值
					int nPacketSize = ObjStreamPtr->GetOptimalPacketSize();
					// 将最优包长值设置为当前设备的流通道包长值
					ObjFeatureControlPtr->GetIntFeature("GevSCPSPacketSize")->SetValue(nPacketSize);
				}
			}
			bool bBalanceWhiteAutoRead = false;         ///< 白平衡是否可读

			//设置采集模式为连续采集模式
			ObjFeatureControlPtr->GetEnumFeature("AcquisitionMode")->SetValue("Continuous");
			//是否支持触发模式选择
			m_bTriggerMode = ObjFeatureControlPtr->IsImplemented("TriggerMode");
			if (m_bTriggerMode)
			{
				//设置触发模式关
				ObjFeatureControlPtr->GetEnumFeature("TriggerMode")->SetValue("Off");
			}

			//注册设备掉线事件【目前只有千兆网系列相机支持此事件通知】
			hDeviceOffline = NULL;
			pDeviceOfflineEventHandler = new CSampleDeviceOfflineEventHandler();
			hDeviceOffline = ObjDevicePtr->RegisterDeviceOfflineCallback(pDeviceOfflineEventHandler, NULL);
			//获取曝光时间
			EPSTime = ObjFeatureControlPtr->GetFloatFeature("ExposureTime")->GetValue();

			//注册回调采集
			pCaptureEventHandler = new CSampleCaptureHandler();
			ObjStreamPtr->RegisterCaptureCallback(pCaptureEventHandler, NULL);

			//设置采集buffer个数
			//ObjStreamPtr->SetAcqusitionBufferNumber(10);
			//发送开采命令
			ObjStreamPtr->StartGrab();
			ObjFeatureControlPtr->GetCommandFeature("AcquisitionStart")->Execute();
			isFinished = true;
			setBalance();
		} while (0);
	}

	catch (CGalaxyException& e)
	{
		isFinished = false;
		cout << "错误码: " << e.GetErrorCode() << endl;
		cout << "错误描述信息: " << e.what() << endl;
	}

	catch (std::exception& e)
	{
		isFinished = false;
		cout << "错误描述信息: " << e.what() << endl;
	}
	return cameraData(EPSTime);
}

//CameraOperator::cameraData CameraOperator::openCamera()
//{
//	IGXFactory::GetInstance().Init();
//	isFinished = false;
//	double EPSTime = 0.0;
//	try
//	{
//		do
//		{
//			//枚举设备
//			gxdeviceinfo_vector vectorDeviceInfo;
//			IGXFactory::GetInstance().UpdateDeviceList(1000, vectorDeviceInfo);
//			if (0 == vectorDeviceInfo.size())
//			{
//				cout << "无可用设备!" << endl;
//				break;
//			}
//			cout << vectorDeviceInfo[0].GetVendorName() << endl;
//			cout << vectorDeviceInfo[0].GetSN() << endl;
//			
//			//打开第一台设备以及设备下面第一个流
//			ObjDevicePtr = IGXFactory::GetInstance().OpenDeviceBySN(
//				vectorDeviceInfo[0].GetSN(),
//				GX_ACCESS_EXCLUSIVE);
//			ObjStreamPtr = ObjDevicePtr->OpenStream(0);
//
//			//注册设备掉线事件【目前只有千兆网系列相机支持此事件通知】
//			hDeviceOffline = NULL;
//			pDeviceOfflineEventHandler = new CSampleDeviceOfflineEventHandler();
//			hDeviceOffline = ObjDevicePtr->RegisterDeviceOfflineCallback(pDeviceOfflineEventHandler, NULL);
//			//获取远端设备属性控制器
//			ObjFeatureControlPtr = ObjDevicePtr->GetRemoteFeatureControl();
//			//获取曝光时间
//			EPSTime = ObjFeatureControlPtr->GetFloatFeature("ExposureTime")->GetValue();
//			ObjFeatureControlPtr->GetEnumFeature("StreamBufferHandlingMode")->SetValue("OldestFirst");
//			//注册回调采集
//			pCaptureEventHandler = new CSampleCaptureHandler();
//			ObjStreamPtr->RegisterCaptureCallback(pCaptureEventHandler, NULL);
//
//			//设置采集buffer个数
//			//ObjStreamPtr->SetAcqusitionBufferNumber(10);
//			//发送开采命令
//			ObjStreamPtr->StartGrab();
//			ObjFeatureControlPtr->GetCommandFeature("AcquisitionStart")->Execute();
//			isFinished = true;
//			setBalance();
//		} while (0);
//	}
//
//	catch (CGalaxyException& e)
//	{
//		isFinished = false;
//		cout << "错误码: " << e.GetErrorCode() << endl;
//		cout << "错误描述信息: " << e.what() << endl;
//	}
//
//	catch (std::exception& e)
//	{
//		isFinished = false;
//		cout << "错误描述信息: " << e.what() << endl;
//	}
//	return cameraData(EPSTime);
//}

void CameraOperator::closeCamera()
{
	if (isFinished == false) {
		return;
	}

	//发送停采命令
	ObjFeatureControlPtr->GetCommandFeature("AcquisitionStop")->Execute();
	ObjStreamPtr->StopGrab();

	//注销采集回调
	ObjStreamPtr->UnregisterCaptureCallback();

	////注销远端设备事件
	//ObjFeatureControlPtr->UnregisterFeatureCallback(hFeatureEvent);

	////注销设备掉线事件
	ObjDevicePtr->UnregisterDeviceOfflineCallback(hDeviceOffline);
	//释放资源
	ObjStreamPtr->Close();
	ObjDevicePtr->Close();
	//反初始化库
	IGXFactory::GetInstance().Uninit();
	isFinished = false;

	//销毁事件回调指针
	if (NULL != pCaptureEventHandler)
	{
		delete pCaptureEventHandler;
		pCaptureEventHandler = NULL;
	}

	if (NULL != pDeviceOfflineEventHandler)
	{
		delete pDeviceOfflineEventHandler;
		pDeviceOfflineEventHandler = NULL;
	}

	//if (NULL != pFeatureEventHandler)
	//{
	//	delete pFeatureEventHandler;
	//	pFeatureEventHandler = NULL;
	//}
	Sleep(1000);
}

void CameraOperator::setBalance()
{
	bool m_strBalanceWhiteAutoMode = true;
	if (isOpenWhiteBalance == true) {
		m_strBalanceWhiteAutoMode = (bool)CamereParameters::BALANCE_WHITE_NONE;
	}
	else {
		m_strBalanceWhiteAutoMode = (bool)CamereParameters::BALANCE_WHITE_AUTO;
	}
	const char* ContinuousModel = "Continuous";
	if (m_strBalanceWhiteAutoMode) {
		ObjFeatureControlPtr->GetEnumFeature("BalanceWhiteAuto")->SetValue(ContinuousModel);
		ObjFeatureControlPtr->GetEnumFeature("GainAuto")->SetValue(ContinuousModel);
	}
}

bool CameraOperator::takePicture()
{
	if (this->isFinished == false) {
		return false;
	}
	pCaptureEventHandler->saveImage();
	return true;
}

void CameraOperator::setExposure(double exposureTime)
{
	if (this->isFinished == false)
		return;
	//设置曝光时间
	ObjFeatureControlPtr->GetEnumFeature("ExposureMode")->SetValue("Timed");
	ObjFeatureControlPtr->GetFloatFeature("ExposureTime")->SetValue(exposureTime);
}



bool __IsCompatible(BITMAPINFO* pBmpInfo, uint64_t nWidth, uint64_t nHeight)
{
	if (pBmpInfo == NULL
		|| pBmpInfo->bmiHeader.biHeight != nHeight
		|| pBmpInfo->bmiHeader.biWidth != nWidth
		)
	{
		return false;
	}
	return true;
}
