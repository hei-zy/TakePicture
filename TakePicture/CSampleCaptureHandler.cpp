#include "CSampleCaptureHandler.h"
#include<QDebug>
#include<QWidget>
#include<QImage>
#include<QString>

using namespace std;
#define LengthOfAruco 3

void CSampleCaptureHandler::DoOnImageCaptured(CImageDataPointer& objImageDataPointer, void* pUserParam)
{
	QImage image;
	if (GX_FRAME_STATUS_SUCCESS == objImageDataPointer->GetStatus())
	{
		STOP_BIT = false;
		uint64_t nWidth = objImageDataPointer->GetWidth();
		uint64_t nHeight = objImageDataPointer->GetHeight();
		//GX_VALID_BIT_LIST emValidBits = GX_BIT_0_7;
		BYTE* pBuffer = NULL;
		if (objImageDataPointer.IsNull())
		{
			throw std::runtime_error("NULL pointer dereferenced");
		}
		auto emValidBits = CSampleCaptureHandler::
			GetBestValudBit(objImageDataPointer->GetPixelFormat());

		pBuffer = (BYTE*)objImageDataPointer->ConvertToRGB24(emValidBits, GX_RAW2RGB_NEIGHBOUR, true);

		QImage qimage(pBuffer, nWidth, nHeight, QImage::Format_RGB888);
		image = qimage;
		if (!image.isNull()) {
			if (isSaved) {
				string adress = "./image/" + to_string(index++) + ".bmp";
				QString qadress = QString::fromStdString(adress);
				image.save(qadress);
				isSaved = false;
			}
			count--;
			//if (count == 0) {
				emit sendImage(image);
				count = 5;
			//}
		}
		STOP_BIT = true;
	}
	STOP_BIT = true;
}

GX_VALID_BIT_LIST CSampleCaptureHandler::GetBestValudBit(GX_PIXEL_FORMAT_ENTRY emPixelFormatEntry)
{
	GX_VALID_BIT_LIST emValidBits = GX_BIT_0_7;
	switch (emPixelFormatEntry)
	{
		case GX_PIXEL_FORMAT_MONO8:
		case GX_PIXEL_FORMAT_BAYER_GR8:
		case GX_PIXEL_FORMAT_BAYER_RG8:
		case GX_PIXEL_FORMAT_BAYER_GB8:
		case GX_PIXEL_FORMAT_BAYER_BG8:
		{
			emValidBits = GX_BIT_0_7;
			break;
		}
		case GX_PIXEL_FORMAT_MONO10:
		case GX_PIXEL_FORMAT_BAYER_GR10:
		case GX_PIXEL_FORMAT_BAYER_RG10:
		case GX_PIXEL_FORMAT_BAYER_GB10:
		case GX_PIXEL_FORMAT_BAYER_BG10:
		{
			emValidBits = GX_BIT_2_9;
			break;
		}
		case GX_PIXEL_FORMAT_MONO12:
		case GX_PIXEL_FORMAT_BAYER_GR12:
		case GX_PIXEL_FORMAT_BAYER_RG12:
		case GX_PIXEL_FORMAT_BAYER_GB12:
		case GX_PIXEL_FORMAT_BAYER_BG12:
		{
			emValidBits = GX_BIT_4_11;
			break;
		}
		case GX_PIXEL_FORMAT_MONO14:
		{
			//暂时没有这样的数据格式待升级
			break;
		}
		case GX_PIXEL_FORMAT_MONO16:
		case GX_PIXEL_FORMAT_BAYER_GR16:
		case GX_PIXEL_FORMAT_BAYER_RG16:
		case GX_PIXEL_FORMAT_BAYER_GB16:
		case GX_PIXEL_FORMAT_BAYER_BG16:
		{
			//暂时没有这样的数据格式待升级
			break;
		}
		default:
			break;
	}
	return emValidBits;
}



void CSampleCaptureHandler::saveImage()
{
	isSaved = true;
}



