#ifndef DIALOGFORDETECT_H
#define DIALOGFORDETECT_H

#include <QDialog>
#include "myKinectSim.h"
#include <QTimer>
#include <QMouseEvent>
#include <QMessageBox>

//该程序主体框架来源于李玉鹏师弟，原有的功能没有变，只是在此基础上加入新的功能
//同时，由于xml在解析时比较麻烦，因此在保存时去掉了该方法，采用了新的思路来完成
//注意，该程序绝对不能后于主窗口的监控关闭

namespace Ui {
class DialogForDetect;
}

class DialogForDetect : public QDialog
{
    Q_OBJECT

public:
    explicit DialogForDetect(QWidget *parent = 0);
    ~DialogForDetect();
    QImage Mat2QImage(cv::Mat cvImg);

signals:
    void emitSaveValueToSettingDialog(std::vector<int> detectArea1,std::vector<int> detectArea2);

private:
    Ui::DialogForDetect *ui;
    KinectSim myKinectSim;

    cv::Mat frame;
    cv::Mat frame3c;
    cv::Mat im;
    QImage  image;
    QTimer *timer;

    //其他变量
    bool open_flag = false;
    //对图像的修正
    bool rec_flag[2];

    unsigned int depth_sum = 0;//彩色框内非零深度的均值
    unsigned int depth_max = 200;//人工设定的深度值范围的增大量
    unsigned int depth_min = 200;//人工设定的深度值范围的减小量

    unsigned int clik_sum = 0;
    unsigned int coords_mean = 0;
    int g_x;
    int g_y;
    bool g_bDrawingBox[2];//是否画框标志
    cv::Rect g_rectangle[2];
    short index;//用于切换方框1和方框2

protected:
    void mousePressEvent(QMouseEvent *);//单击
    void mouseMoveEvent(QMouseEvent *event);//移动
    void mouseReleaseEvent(QMouseEvent *event);//释放

private slots:
    void on_open_clicked();
    void on_box_clicked();
    void on_clean_clicked();
    void on_save_clicked();
    void nextFrame();
    void DrawRectangle(cv::Mat& img, cv::Rect box);
    void on_spinBox_min_valueChanged(int arg1);
    void on_spinBox_max_valueChanged(int arg1);

    //新增的功能和选项（用于切换和传回数据）
    void on_pushButton_turn_clicked();
};

#endif // DIALOGFORDETECT_H
