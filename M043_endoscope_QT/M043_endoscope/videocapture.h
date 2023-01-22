#ifndef VIDEOCAPTURE_H
#define VIDEOCAPTURE_H

#include <QPixmap>
#include <QImage>
#include <QThread>
#include <opencv2/opencv.hpp>

class VideoCapture : public QThread
{
    Q_OBJECT
public:
    VideoCapture(QObject *parent);
    void SetCameraIndex(int index);
    void SetRecordingMode(bool status);
    bool GetRecordingStatus();

    QPixmap pixmap() const
    {
        return mPixmap;
    }

signals:
    void newPixmapCaptured();
    void openDeviceError();

protected:
    void run() override;

private:
    QPixmap mPixmap;
    cv::Mat mFrame;
    cv::VideoCapture mVideoCap;
    cv::VideoWriter mVideoWriter;
    uint8_t index_ = 0;
    QImage cvMatToQImage(const cv::Mat &inMat);
    QPixmap cvMatToQPixmap(const cv::Mat &inMat);
    bool mRecordingStatus = false;
};

#endif // VIDEOCAPTURE_H
