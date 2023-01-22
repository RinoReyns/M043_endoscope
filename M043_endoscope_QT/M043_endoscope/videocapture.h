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
    void InitSaveImage();
    void SaveImage();

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
    void InitVideoWriter();
    void GetCurrentDate(std::string* data);

private:
    QPixmap mPixmap;
    cv::Mat mFrame;
    cv::VideoCapture mVideoCap;
    cv::VideoWriter mVideoWriter;
    bool mIsVideoDummperInit = false;
    bool mSaveImageFlag = false;
    uint8_t index_ = 0;
    QImage cvMatToQImage(const cv::Mat &inMat);
    QPixmap cvMatToQPixmap(const cv::Mat &inMat);
    bool mRecordingStatus = false;
};

#endif // VIDEOCAPTURE_H
