#include "dialogForDetect.h"
#include "ui_dialogForDetect.h"

DialogForDetect::DialogForDetect(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogForDetect)
{
    ui->setupUi(this);
    this->setFixedSize(this->width(),this->height());//固定尺寸
    //将开启摄像头的部分放在构造函数中
    myKinectSim.InitKinect();
    myKinectSim.InitDepth();
    timer = new QTimer(this);
    timer->setInterval(30);   //set timer match with FPS==30
    connect(timer, SIGNAL(timeout()), this, SLOT(nextFrame()));
    myKinectSim.depthProcess2();
    frame = myKinectSim.depthMat8;
    im = myKinectSim.depthMat16;
    frame3c.create(frame.rows,frame.cols,CV_8UC(3));

    index = 0; //默认是方框1
    g_rectangle[0] = cv::Rect(-1,-1,0,0);
    g_rectangle[1] = cv::Rect(-1,-1,0,0);

    g_bDrawingBox[0] =false;
    g_bDrawingBox[1] =false;

    rec_flag[0]=false;
    rec_flag[1]=false;
}

void DialogForDetect::on_open_clicked()
{
    open_flag = true;
    timer->start();
}

DialogForDetect::~DialogForDetect()
{
    delete ui;
}

QImage DialogForDetect::Mat2QImage(cv::Mat cvImg)
{
    QImage qImg;
    if(cvImg.channels()==3)                        //3 channels color image
    {
        cv::cvtColor(cvImg,cvImg,CV_BGR2RGB);
        qImg =QImage((const unsigned char*)(cvImg.data),
                     cvImg.cols, cvImg.rows,
                     cvImg.cols*cvImg.channels(),
                     QImage::Format_RGB888);
    }
    else if(cvImg.channels()==1)                    //grayscale image
    {
        qImg =QImage((const unsigned char*)(cvImg.data),
                     cvImg.cols,cvImg.rows,
                     cvImg.cols*cvImg.channels(),
                     QImage::Format_Indexed8);
    }
    else
    {
        qImg =QImage((const unsigned char*)(cvImg.data),
                     cvImg.cols,cvImg.rows,
                     cvImg.cols*cvImg.channels(),
                     QImage::Format_RGB888);
    }

    return qImg;
}

void DialogForDetect::on_box_clicked()//应该是公用的但实际上是分开的，还需要测试
{
    frame3c = cv::Scalar::all(0);
    g_rectangle[index] = cv::Rect(-1,-1,0,0);
    rec_flag[index] = false;
}

void DialogForDetect::on_clean_clicked()
{
    if(index == 0)
    {
        ui->coords_value->clear();
        depth_sum = 0;//用于计算均值，在按下clean按钮后，置0
        clik_sum = 0;
        coords_mean = 0;
    }
    else if (index ==1)
    {
        ui->coords_value_2->clear();
    }
}

void DialogForDetect::on_save_clicked() //保存应该是两个同时都保存的--------------------还需修改
{
    //方框1部分
    QString coords, rectangle_coords_tl, rectangle_coords_br;
    //设置点击的有效范围，即滑动绘制的方框所在区域的坐标范围
    if( (g_x >= g_rectangle[0].tl().x && g_x <= g_rectangle[0].br().x )
            && (g_y >= g_rectangle[0].tl().y && g_y <= g_rectangle[0].br().y) )
    {
        coords = tr("%1,%2,%3").arg(coords_mean).arg(coords_mean-depth_min).arg(coords_mean+depth_max);
        rectangle_coords_tl = tr("%1,%2").arg(g_rectangle[0].tl().x).arg(g_rectangle[0].tl().y);
        rectangle_coords_br = tr("%1,%2").arg(g_rectangle[0].br().x).arg(g_rectangle[0].br().y);
    }

    //将所设置的参数在界面上显示
    QString saved={"depth_mean_min_max:" + coords+"\n"
                   +"rectangle_coords_tl:" + rectangle_coords_tl+"\n"
                   +"rectangle_coords_br:" + rectangle_coords_br+"\n"
                  };
    ui->textBrowser_saved->setText(saved);//输出的是七个值，中间/前/后，矩阵的两个点 是一个数组 -----------------------------------
    std::vector<int> tmpVec1;
    tmpVec1.push_back(coords_mean>0?coords_mean:0);
    tmpVec1.push_back((coords_mean-depth_min)>0?(coords_mean-depth_min):0);
    tmpVec1.push_back(coords_mean+depth_max);
    tmpVec1.push_back(g_rectangle[0].tl().x);
    tmpVec1.push_back(g_rectangle[0].tl().y);
    tmpVec1.push_back(g_rectangle[0].br().x);
    tmpVec1.push_back(g_rectangle[0].br().y);
    //方框2的保存 输出的应该是5个值，矩阵的两个点和中点------------------------------------------------------
    QString saved2 = {"final output:"+tr("%1").arg(ui->spinBox->value())+"\n"};
    ui->textBrowser_saved_2->setText(saved2);
    std::vector<int> tmpVec2;
    tmpVec2.push_back(ui->spinBox->value());
    tmpVec2.push_back(g_rectangle[1].tl().x);
    tmpVec2.push_back(g_rectangle[1].tl().y);
    tmpVec2.push_back(g_rectangle[1].br().x);
    tmpVec2.push_back(g_rectangle[1].br().y);

    //传送数据
    emit emitSaveValueToSettingDialog(tmpVec1,tmpVec2);
}

void DialogForDetect::on_spinBox_min_valueChanged(int arg1)
{
    depth_min = arg1;
}

void DialogForDetect::on_spinBox_max_valueChanged(int arg1)
{
    depth_max = arg1;
}

void DialogForDetect::DrawRectangle(cv::Mat& img, cv::Rect box)
{
    cv::rectangle(img,box.tl(),box.br(),cv::Scalar(255,255,255),2);//固定颜色
}

//读取下一帧
void DialogForDetect::nextFrame()
{
    std::vector<cv::Mat>frame3cpart(frame3c.channels());//生成与通道数数目相等的图像容器
    myKinectSim.depthProcess2();
    cv::split(frame3c,frame3cpart);
    frame3cpart[1] = myKinectSim.depthMat8;//用于显示
    cv::merge(frame3cpart,frame3c);//合成与通道数数目相等的图像容器
    if(rec_flag[0])
    {
        rectangle(frame3c,g_rectangle[0].tl(),g_rectangle[0].br(),cv::Scalar(255,255,255),2);
    }
    if(rec_flag[1])
    {
        rectangle(frame3c,g_rectangle[1].tl(),g_rectangle[1].br(),cv::Scalar(255,0,255),2);
    }
    im = myKinectSim.depthMat16;//用于读取源图像的值，从而不受划线影响像素值
    image = Mat2QImage(frame3c);
    ui->video_show->setPixmap(QPixmap::fromImage(image));
}


void DialogForDetect::mouseReleaseEvent(QMouseEvent *event)//释放鼠标
{
    if(event->button() == Qt::RightButton)
    {
        g_bDrawingBox[index] = false;//标识符置为false
        //对宽和高小于0的处理
        if(g_rectangle[index].width < 0)
        {
            g_rectangle[index].x += g_rectangle[index].width;
            g_rectangle[index].width *= -1;
        }

        if(g_rectangle[index].height < 0)
        {
            g_rectangle[index].y += g_rectangle[index].height;
            g_rectangle[index].height *= -1;
        }
        //调用函数进行绘制
        DrawRectangle(frame3c,g_rectangle[index]);
        rec_flag[index] = true;
        if(index == 1)//如果是方框2直接输出
        {
            ui->spinBox->setValue(myKinectSim.detPeopleDepth3(g_rectangle[1].tl(),g_rectangle[1].br()));
        }
    }
}

//移动鼠标
void DialogForDetect::mouseMoveEvent(QMouseEvent *event)
{
    if(true==g_bDrawingBox[index])//如何进行绘制的标识符为true，则记录下长和宽到RECT型变量中
    {
        //获取全局位置
        QPoint lPoint = event->globalPos();
        lPoint = ui->video_show->mapFromGlobal(lPoint);
        int x = lPoint.x();
        int y = lPoint.y();

        g_rectangle[index].width = x - g_rectangle[index].x;
        g_rectangle[index].height = y - g_rectangle[index].y;
    }
}

void DialogForDetect::mousePressEvent(QMouseEvent *e)
{
    //open_flag==true:只有当摄像头开启后，才读取像素，
    //否则会由于所点击的区域未被初始化而使程序崩溃
    if(true==open_flag)
    {
        //获取全局位置
        QPoint lPoint = e->globalPos();
        lPoint = ui->video_show->mapFromGlobal(lPoint);
        int x = lPoint.x();
        int y = lPoint.y();
        g_x = x;
        g_y = y;
        unsigned int depth = static_cast<unsigned int>(im.at<short>(cv::Point(x,y)));

        if( (x <= 512) && (y <= 424) )//设置点击的有效范围，即图像所在区域的坐标范围
        {
            //-----------------------------------------------------方框1-------------------------
            if(index == 0)
            {
                //设置点击的有效范围，即滑动绘制的方框所在区域的坐标范围
                if( (x >= g_rectangle[0].tl().x && x <= g_rectangle[0].br().x )
                        && (y >= g_rectangle[0].tl().y && y <= g_rectangle[0].br().y) )
                {
                    if(e->button()==Qt::LeftButton)//左键按下，用于深度值的选取
                    {
                        //**********这里后面再将选取区域选定在绘制的矩形当中

                        ui->coords_value->insertPlainText(tr("(%1,%2,%3)").arg(x).arg(y).arg(depth));
                        if(0 != depth)
                        {
                            depth_sum += depth;//在按下clean按钮后，置0
                            clik_sum += 1;
                            coords_mean = depth_sum/clik_sum;
                            ui->mean->setText(tr("%1").arg(coords_mean));
                        }
                    }
                }
                else if(e->button() == Qt::RightButton)//右键按下，用于绘制矩形框
                {
                    if(!rec_flag[0])//只能绘制一次
                    {
                        g_bDrawingBox[0] = true;
                        g_rectangle[0] = cv::Rect(x,y,0,0);//记录起始点
                    }
                }
            }//----------------------方框2==================================
            else if (index == 1)
            {
                if(e->button()==Qt::LeftButton)//左键按下，用于深度值的选取
                {
                    //显示数据到2号框上 获取单点的值
                    if(0 != depth)
                    {
                        ui->coords_value_2->insertPlainText(tr("(%1,%2,%3)").arg(x).arg(y).arg(depth));
                    }
                }
                else if(e->button() == Qt::RightButton)//右键按下，用于绘制矩形框
                {
                    if(!rec_flag[1])//只能绘制一次
                    {
                        g_bDrawingBox[1] = true;
                        g_rectangle[1] = cv::Rect(x,y,0,0);//记录起始点
                    }
                }
            }
        }
    }
    else
    {
        QMessageBox message(QMessageBox::NoIcon,"Warning","请先打开摄像机", \
                            QMessageBox::Yes | QMessageBox::No, NULL);
        if(message.exec() == QMessageBox::Yes)
        {
            on_open_clicked();
        }
    }
}


void DialogForDetect::on_pushButton_turn_clicked() //用于切换方框1和方框2
{
    if(index == 0)
    {
        ui->pushButton_turn->setText(tr("方框2"));
        index = 1;
    }else if(index == 1)
    {
        ui->pushButton_turn->setText(tr("方框1"));
        index = 0;
    }
}
