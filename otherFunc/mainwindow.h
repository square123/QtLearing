#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <opencv2/opencv.hpp>

#include<QPaintEvent>//图像
#include<QTimer>//图像
#include<QPainter>//图像
#include<QPixmap>//图像
#include<QLabel>//图像
#include<QImage>//图像
#include<QStandardItem>//表格用
#include <QFile>//文件存在
#include <QFileDialog>//文件存在
#include <QString>//文件存在
#include <QMessageBox>

#include "mykinectthread.h"
#include "idcardreaderthread.h"
#include "wifithread.h"
#include "dialogForSettings.h"
#include "inisetting.h"
#include "transDataForm.h"
#include "dialogForFind.h"
#include "transdatathread.h"



namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:
    void emitSettingsToSetDialog(iniData sData,std::string dPath,std::string iPath);//用于给设置窗口传递信息

public slots:
    void updateImage();//更新图片信息
    void openMonitor();//开启监视
    void closeMonitor();//关闭监视器
    void renewFlag();//用于更新参数
    void getSettingsFormSetDialog(iniData sData,std::string dPath,std::string iPath);//用于从设置窗口中获取参数

private:
    Ui::MainWindow *ui;
    DialogForSettings *myDialogForSettings;//设置界面对话框
    DialogForFind *myDialogForFind;//查询界面对话框
    QTimer theTimer;//用于显示
    QTimer theTimerForRenewFlag;//用于将Kinect的索引传递给其他的函数
    short clickTime;//按钮点击次数 用于切换功能

    //Mat变量->显示部分
    //video部分
    cv::Mat colorVideo;
    cv::Mat depthVideo;
    cv::Mat bodyVideo;
    //image分割部分显示 (还需实际测试，因为可能指的是反的)
    cv::Mat allPartsImage;          //0
    cv::Mat headImage;              //1
    cv::Mat bodyImage;              //2
    cv::Mat leftSmallArmImage;      //6
    cv::Mat leftBigArmImage;        //5
    cv::Mat rightSmallArmImage;     //4
    cv::Mat rightBigArmImage;       //3
    cv::Mat leftSmallLegImage;      //10
    cv::Mat leftBigLegImage;        //9
    cv::Mat rightSmallLegImage;     //8
    cv::Mat rightBigLegImage;       //7
    //idCard部分
    cv::Mat idCardImg;

    //Mac地址展示部分
    QStandardItemModel *model;//表格模型数据


    //各类线程
    myKinectThread *mKTd;
    idCardReaderThread *myidCardReaderThread;
    wifiThread *myWifiThread;
    transDataThread *myTransDataThread;

    //一些辅助函数
    QPixmap Mat2QPixmap(cv::Mat cvImg);

    //用于和设置界面传递变量
    iniData settingData;
    std::string detectExePath;
    std::string iniFilePath;

protected:
    void paintEvent(QPaintEvent *event);
    void closeEvent(QCloseEvent *event);//关闭时要注意的

private slots:
    void on_startButton_clicked();//用于打开/关闭监控
    void on_settingButton_clicked();
    void on_historyButton_clicked();
};

#endif // MAINWINDOW_H
