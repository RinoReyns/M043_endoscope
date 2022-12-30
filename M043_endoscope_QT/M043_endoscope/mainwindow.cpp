#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "videocapture.h"
#include "deviceenumerator.h"
#include "QDebug"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Web Camera Capture App");
    mOpenCV_videoCapture = new VideoCapture(this);
    this->ListVideoEndpoints();
    connect(mOpenCV_videoCapture, &VideoCapture::newPixmapCaptured, this, [&]()
    {
        // TODO:
        // 1. list resolution for given endpoints and allow to seleced by user.
        //    by default set the highest
        // 2. save capture to file.
        ui->opencvFrame->setPixmap(mOpenCV_videoCapture->pixmap().scaled(640, 480)); // set proper size
    });

}

MainWindow::~MainWindow()
{
    delete ui;
    mOpenCV_videoCapture->terminate();
}

void MainWindow::ListVideoEndpoints()
{
    DeviceEnumerator de;
    std::map<int, Device> devices = de.getVideoDevicesMap();
    for (auto const &device : devices)
    {
        ui->endpointsListcomboBox->addItem(QString::fromLocal8Bit(device.second.deviceName.c_str()));
    }
//    const QList<QCameraDevice> cameras = QMediaDevices::videoInputs();
//    for (const QCameraDevice &cameraDevice : cameras)
//        qDebug() << cameraDevice.description();
    // https://doc.qt.io/qt-6/qcameradevice.html
}

void MainWindow::on_InitOpenCV_button_clicked()
{
    if(mOpenCV_videoCapture->isRunning())
    {
        mOpenCV_videoCapture->terminate();
    }
    mOpenCV_videoCapture->SetCameraIndex(ui->endpointsListcomboBox->currentIndex());
    mOpenCV_videoCapture->start(QThread::HighestPriority);
}
