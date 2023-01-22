#include "videocapture.h"
#include <QDebug>
#include <QMessageBox>
#include <iostream>

VideoCapture::VideoCapture(QObject *parent)
    : QThread{parent}
{
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

void VideoCapture::run()
{
    mVideoCap = cv::VideoCapture(this->index_);
    mVideoWriter = cv::VideoWriter("outcpp.mp4", cv::VideoWriter::fourcc('a', 'v', 'c', '1') , 10, cv::Size_(640, 480));


    if (mVideoCap.isOpened())
    {
        while (true)
        {
            mVideoCap >> mFrame;
            if (! mFrame.empty())
            {
                mPixmap = cvMatToQPixmap(mFrame);
                // TODO:
                // make sure that you can stop and rerecord one more time

                if (mRecordingStatus)
                {
                    mVideoWriter.write(mFrame);
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
