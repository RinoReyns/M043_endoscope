#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class VideoCapture;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void ListVideoEndpoints();

private slots:
    void on_InitOpenCV_button_clicked();
    void on_startRecording_clicked();

    void on_saveSingleImage_clicked();

private:
    Ui::MainWindow *ui;
    VideoCapture *mOpenCV_videoCapture;
};
#endif // MAINWINDOW_H
