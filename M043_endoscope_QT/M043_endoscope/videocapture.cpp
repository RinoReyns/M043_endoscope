#include "videocapture.h"
#include <QDebug>
#include <QMessageBox>
#include <iostream>
#include <QDateTime>
#include <QFile>
#include <QString>
#include <QDir>

VideoCapture::VideoCapture(QObject *parent)
    : QThread{parent}
{
    if (!QDir().exists(mDumpFolder))
    {
        QDir().mkdir(mDumpFolder);
    }
}

void VideoCapture::SetCameraIndex(int index)
{
    this->index_ = static_cast<uint8_t>(index);
}

bool VideoCapture::GetRecordingStatus()
{
    return mRecordingStatus;
}

void VideoCapture::SetRecordingMode(bool status)
{
    mRecordingStatus = status;
}

void VideoCapture::InitSaveImage()
{
    mSaveImageFlag = true;
}

void VideoCapture::SaveImage()
{
    mSaveImageFlag = false;
    std::string data;
    GetCurrentDate(&data);
    QString data_qt = QString::fromUtf8(data.data(), int(data.size()));
    QFile file(mDumpFolder + "\\Endoscope_image_" + data_qt + ".jpg");
    file.open(QIODevice::WriteOnly);
    mPixmap.save(&file, "JPG");
    file.close();
}

void VideoCapture::GetCurrentDate(std::string* data)
{
    QDateTime date = QDateTime::currentDateTime();
    QString formattedTime = date.toString("dd_MM_yyyy_hh_mm_ss");
    *data = formattedTime.toUtf8().constData();
}

void VideoCapture::InitVideoWriter()
{
    std::string data;
    GetCurrentDate(&data);
    mVideoWriter = cv::VideoWriter(mDumpFolder.toStdString() + "\\Endoscope_video" + data + ".mp4",
                                   cv::VideoWriter::fourcc('a', 'v', 'c', '1'),
                                   10,
                                   cv::Size_(640, 480));

}

void VideoCapture::run()
{

    mVideoCap.release();
    mVideoWriter.release();

    mVideoCap = cv::VideoCapture(this->index_);

    if (mVideoCap.isOpened())
    {
        while (true)
        {
            mVideoCap >> mFrame;
            if (! mFrame.empty())
            {
                mPixmap = cvMatToQPixmap(mFrame);
                if (mSaveImageFlag)
                {
                   SaveImage();
                }

                if (mRecordingStatus)
                {
                    if (!mIsVideoDummperInit)
                    {
                        InitVideoWriter();
                        mIsVideoDummperInit = true;
                    }
                    mVideoWriter.write(mFrame);
                }
                // TODO:
                // optimize this else
                else
                {
                    mVideoWriter.release();
                    mIsVideoDummperInit = false;
                }
                emit newPixmapCaptured();
            }
        }
    }
    else
    {
        emit openDeviceError();
    }
}

QImage VideoCapture::cvMatToQImage(const cv::Mat &inMat)
{
    switch (inMat.type())
    {
    // 8-bit, 4 channel
    case CV_8UC4:
    {
        QImage image( inMat.data,
                      inMat.cols, inMat.rows,
                      static_cast<int>(inMat.step),
                      QImage::Format_ARGB32 );

        return image;
    }

    // 8-bit, 3 channel
    case CV_8UC3:
    {
        QImage image( inMat.data,
                      inMat.cols, inMat.rows,
                      static_cast<int>(inMat.step),
                      QImage::Format_RGB888 );

        return image.rgbSwapped();
    }

    // 8-bit, 1 channel
    case CV_8UC1:
    {

        QImage image( inMat.data,
                      inMat.cols, inMat.rows,
                      static_cast<int>(inMat.step),
                      QImage::Format_Grayscale8 );

        return image;
    }

    default:
        qWarning() << "ASM::cvMatToQImage() - cv::Mat image type not handled in switch:" << inMat.type();
        break;
    }

    return QImage();
}

QPixmap VideoCapture::cvMatToQPixmap(const cv::Mat &inMat)
{
    return QPixmap::fromImage(cvMatToQImage(inMat));
}
