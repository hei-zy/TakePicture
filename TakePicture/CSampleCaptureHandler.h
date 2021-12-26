#pragma once
#include"GalaxyIncludes.h"
#include<QWidget>
#include<QImage>
#include<QObject>

enum TypeOfPoint {
	LaserPoint,FixPoint,ThreSeg
};

class CSampleCaptureHandler : public QObject, public ICaptureEventHandler
{
	Q_OBJECT

public:

	virtual void DoOnImageCaptured(CImageDataPointer& objImageDataPointer, void* pUserParam);
	static GX_VALID_BIT_LIST GetBestValudBit(GX_PIXEL_FORMAT_ENTRY emPixelFormatEntry);

	void saveImage();

public:
	bool STOP_BIT = true;

signals:
	void sendImage(QImage);

private:
	bool isSaved = false;
	int index = 1;
	int count = 5;
};

