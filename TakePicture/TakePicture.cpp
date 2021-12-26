#include "TakePicture.h"

TakePicture::TakePicture(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);

    initCamOpThread();
    initCamCalibThread();
    connectSignalsSlots();
}

void TakePicture::initCamOpThread()
{
    cameraOperator = new CameraOperator;
    thCameraOperator = new QThread(this);
    cameraOperator->moveToThread(thCameraOperator);
}

void TakePicture::initCamCalibThread()
{
    QThread* thCamCalib = new QThread;
    Calibration* camCal = new Calibration;
    camCal->moveToThread(thCamCalib);
    connect(ui.CalibCamButton, &QPushButton::clicked, this, [=]()
        {
            ui.CalibCamButton->setEnabled(false);
            std::string path = "./GxSingleCamImages";
            emit camImagePath(path);
            thCamCalib->start();
        });
    connect(this, &TakePicture::camImagePath, camCal, &Calibration::dowork);
    connect(camCal, &Calibration::camCalibFinished, this, [=](const QString& str)
        {
            ui.CalibCamButton->setEnabled(true);
            QMessageBox::information(this, "DONE", str);
        });
    //release
    connect(this, &QWidget::destroyed, this, [=]()
        {
            thCamCalib->quit();
            thCamCalib->exit();
            thCamCalib->deleteLater();

            camCal->deleteLater();
        });
}


void TakePicture::openCamera()
{
    CameraOperator::cameraData cData = cameraOperator->openCamera();
    ui.ExposureTimeEdit->setText(QString::number(cData.EPSTime));
    ui.ExposureTimeEdit->setReadOnly(false);
    ui.ExposureTimeEdit->setEnabled(true);
    connect((cameraOperator->pCaptureEventHandler), &CSampleCaptureHandler::sendImage, this, &TakePicture::displayPicture);
}

void TakePicture::connectSignalsSlots()
{
    connect(ui.OpenCameraButton, &QPushButton::clicked, this, &TakePicture::openCamera);
    connect(ui.CloseCameraButton, &QPushButton::clicked, this, &TakePicture::closeCamera);
    connect(ui.TakePictureButton, &QPushButton::clicked, this, &TakePicture::getPicture);
    connect(ui.ExposureTimeEdit, &QLineEdit::editingFinished, this, &TakePicture::setExposure);
}

void TakePicture::setExposure()
{
    QString str = ui.ExposureTimeEdit->text();
    if (str.isEmpty())
        return;
    double exposureTime = str.toDouble();
    if (cameraOperator->pCaptureEventHandler != nullptr)
        cameraOperator->setExposure(exposureTime);
}

void TakePicture::getPicture()
{
    if (cameraOperator->pCaptureEventHandler != nullptr)
        cameraOperator->takePicture();
}

void TakePicture::closeCamera()
{
    disconnect(cameraOperator->pCaptureEventHandler, &CSampleCaptureHandler::sendImage, this, &TakePicture::displayPicture);
    cameraOperator->closeCamera();
    ui.ExposureTimeEdit->setReadOnly(true);
    ui.ExposureTimeEdit->setEnabled(false);
}

void TakePicture::closeEvent(QCloseEvent*event)
{
    if (cameraOperator->pCaptureEventHandler != nullptr)
        cameraOperator->closeCamera();
}

void TakePicture::displayPicture(QImage input)
{
    QPixmap map = QPixmap::fromImage(input);
    int width = ui.pictureLabel->width();
    int height = ui.pictureLabel->height();
    QPixmap fitpixmap = map.scaled(width, height, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui.pictureLabel->setPixmap(fitpixmap);
}