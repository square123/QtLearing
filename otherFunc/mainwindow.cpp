#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "iostream"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    myDialogForSettings = 0;//先设置未0
    myDialogForFind = 0;//先设置未0
    ui->setupUi(this);
    this->setFixedSize(this->width(),this->height());//固定尺寸

    //读取配置文件
    //先判断文件是否存在
    const char * tmpSrcPath = "C://Users//HP//Documents//MainProject//projectSetting.ini";
    bool tmpFlag =true;
    while(tmpFlag)//循环判断机制 //可能有点变态因为找不到就很坑 一直循环 但测试连续点关闭就好
    {
        QFile file(tmpSrcPath);
        if(!file.exists())
        {
            //QMessageBox::warning(this,tr("警告"),tr("未找到项目的配置属性文件（projectSetting.ini）！"),QMessageBox::Yes);//这句话很奇怪，不知道是什么原因会阻塞循环的发生
            QString src= QFileDialog::getOpenFileName(this,tr("导入ini文件"),"",tr("ini(*.ini)"));
            tmpSrcPath = transPath2(src.toStdString()).c_str();
        }
        QFile otherFile(tmpSrcPath);
        if(otherFile.exists())
        {
            tmpFlag = false;
        }
    }

    //读取detect文件路径 不加判断，在设置窗口中加入就好
    detectExePath = "E://标定软件v0.1.exe";

    //正式读取配置文件
    iniFilePath = tmpSrcPath;
    readIniFile(tmpSrcPath,settingData);

    //kinect部分
    mKTd = new myKinectThread(transPath(settingData.xmlPath).c_str(),transPath(settingData.savePath).c_str());//还需要修改
    clickTime =0;
    connect(&theTimer,&QTimer::timeout,this,&MainWindow::updateImage);

    //参数更新部分
    connect(&theTimerForRenewFlag,&QTimer::timeout,this,&MainWindow::renewFlag);

    //身份证部分
    myidCardReaderThread = new idCardReaderThread(transPath(settingData.idCardPath).c_str());//后面不要再加斜杠

    //wifi部分
    myWifiThread = new wifiThread(settingData.wifiMac.c_str(),settingData.wifiPort,settingData.wifiThd);

    //传输数据线程部分
    myTransDataThread = new transDataThread;

    //表格数据显示
    model = new QStandardItemModel();
    //设置模型
    model->setColumnCount(3);
    model->setHeaderData(0,Qt::Horizontal,tr("MAC地址"));
    model->setHeaderData(1,Qt::Horizontal,tr("平均RSSI"));
    model->setHeaderData(2,Qt::Horizontal,tr("个数"));
    //两者绑定
    ui->macTableView->setModel(model);//输出模型
    ui->macTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);//数据不可编辑
    ui->macTableView->setSelectionBehavior(QAbstractItemView::SelectRows);//选择只能选择一行
    //表格行距设置
    ui->macTableView->verticalHeader()->setVisible(false);
    // ui->macTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);//自动调整 但感觉不太美观
    ui->macTableView->setColumnWidth(0,124);
    ui->macTableView->setColumnWidth(1,80);
    ui->macTableView->setColumnWidth(2,45);

    //启动定时器
    theTimer.start(30);
    theTimerForRenewFlag.start(10);

}

MainWindow::~MainWindow()
{
    delete myDialogForSettings;
    myDialogForSettings=0;
    delete myDialogForFind;
    myDialogForFind = 0;
    delete ui;
    ui = 0;
    //线程指针的关闭
    delete mKTd;
    mKTd = 0;
    delete myidCardReaderThread;
    myidCardReaderThread = 0;
    delete myWifiThread;
    myWifiThread = 0;
    delete myTransDataThread;
    myTransDataThread = 0;
}

void MainWindow::updateImage()//Kinect部分 用于大小转换
{
    //video部分
    if(mKTd->myKinect->bodyIndexMat.data)
    {
        cvtColor(mKTd->myKinect->bodyIndexMat,bodyVideo,CV_BGR2RGB);//由于openCV是BGR，因此需要转换
        cv::resize(bodyVideo,bodyVideo,cv::Size(ui->bodyVideo->size().width(),ui->bodyVideo->size().height()));
    }
    if(mKTd->myKinect->color.data)
    {
        cvtColor(mKTd->myKinect->color,colorVideo,CV_BGR2RGB);//由于openCV是BGR，因此需要转换
        cv::resize(colorVideo,colorVideo,cv::Size(ui->colorVideo->size().width(),ui->colorVideo->size().height()));
    }
    if(mKTd->myKinect->depthMat8.data)
    {
        cv::resize(mKTd->myKinect->depthMat8,depthVideo,cv::Size(ui->depthVideo->size().width(),ui->depthVideo->size().height()));
    }

    //image部分 11个部分   显示部分要采用别的方式试下

    //全身
    if(mKTd->myKinect->selectImageArray[0].data)
    {
        cvtColor(mKTd->myKinect->selectImageArray[0],allPartsImage,CV_BGR2RGB);//由于openCV是BGR，因此需要转换
        cv::resize(allPartsImage,allPartsImage,cv::Size(ui->allPartsImg->size().width(),ui->allPartsImg->size().height()));
    }
    //头部
    if(mKTd->myKinect->selectImageArray[1].data)
    {
        cvtColor(mKTd->myKinect->selectImageArray[1],headImage,CV_BGR2RGB);//由于openCV是BGR，因此需要转换
        cv::resize(headImage,headImage,cv::Size(ui->headImg->size().width(),ui->headImg->size().height()));
    }
    //身体
    if(mKTd->myKinect->selectImageArray[2].data)
    {
        cvtColor(mKTd->myKinect->selectImageArray[2],bodyImage,CV_BGR2RGB);//由于openCV是BGR，因此需要转换
        cv::resize(bodyImage,bodyImage,cv::Size(ui->bodyImg->size().width(),ui->bodyImg->size().height()));
    }
    //右大臂
    if(mKTd->myKinect->selectImageArray[3].data)
    {
        cvtColor(mKTd->myKinect->selectImageArray[3],rightBigArmImage,CV_BGR2RGB);//由于openCV是BGR，因此需要转换
        cv::resize(rightBigArmImage,rightBigArmImage,cv::Size(ui->rightBigArmImg->size().width(),ui->rightBigArmImg->size().height()));
    }
    //右小臂
    if(mKTd->myKinect->selectImageArray[4].data)
    {
        cvtColor(mKTd->myKinect->selectImageArray[4],rightSmallArmImage,CV_BGR2RGB);//由于openCV是BGR，因此需要转换
        cv::resize(rightSmallArmImage,rightSmallArmImage,cv::Size(ui->rightSmallArmImg->size().width(),ui->rightSmallArmImg->size().height()));
    }
    //左大臂
    if(mKTd->myKinect->selectImageArray[5].data)
    {
        cvtColor(mKTd->myKinect->selectImageArray[5],leftBigArmImage,CV_BGR2RGB);//由于openCV是BGR，因此需要转换
        cv::resize(leftBigArmImage,leftBigArmImage,cv::Size(ui->leftBigArmImg->size().width(),ui->leftBigArmImg->size().height()));
    }
    //左小臂
    if(mKTd->myKinect->selectImageArray[6].data)
    {
        cvtColor(mKTd->myKinect->selectImageArray[6],leftSmallArmImage,CV_BGR2RGB);//由于openCV是BGR，因此需要转换
        cv::resize(leftSmallArmImage,leftSmallArmImage,cv::Size(ui->leftSmallArmImg->size().width(),ui->leftSmallArmImg->size().height()));
    }
    //右大腿
    if(mKTd->myKinect->selectImageArray[7].data)
    {
        cvtColor(mKTd->myKinect->selectImageArray[7],rightBigLegImage,CV_BGR2RGB);//由于openCV是BGR，因此需要转换
        cv::resize(rightBigLegImage,rightBigLegImage,cv::Size(ui->rightBigLegImg->size().width(),ui->rightBigLegImg->size().height()));
    }
    //右小腿
    if(mKTd->myKinect->selectImageArray[8].data)
    {
        cvtColor(mKTd->myKinect->selectImageArray[8],rightSmallLegImage,CV_BGR2RGB);//由于openCV是BGR，因此需要转换
        cv::resize(rightSmallLegImage,rightSmallLegImage,cv::Size(ui->rightSmallLegImg->size().width(),ui->rightSmallLegImg->size().height()));
    }
    //左大腿
    if(mKTd->myKinect->selectImageArray[9].data)
    {
        cvtColor(mKTd->myKinect->selectImageArray[9],leftBigLegImage,CV_BGR2RGB);//由于openCV是BGR，因此需要转换
        cv::resize(leftBigLegImage,leftBigLegImage,cv::Size(ui->rightBigLegImg->size().width(),ui->rightBigLegImg->size().height()));
    }
    //左小腿
    if(mKTd->myKinect->selectImageArray[10].data)
    {
        cvtColor(mKTd->myKinect->selectImageArray[10],leftSmallLegImage,CV_BGR2RGB);//由于openCV是BGR，因此需要转换
        cv::resize(leftSmallLegImage,leftSmallLegImage,cv::Size(ui->leftSmallLegImg->size().width(),ui->leftSmallLegImg->size().height()));
    }

    //身份证idCardImg部分
    if(myidCardReaderThread->myIdCardReader->idCardImg.data)
    {
        cvtColor(myidCardReaderThread->myIdCardReader->idCardImg,idCardImg,CV_BGR2RGB);
        cv::resize(idCardImg,idCardImg,cv::Size(ui->idCardImg->size().width(),ui->idCardImg->height()));
    }

    this->update();
}

void MainWindow::openMonitor()//开启监视器
{
    //Kinect部分
    mKTd->start();
    //身份证部分
    myidCardReaderThread->start();
    //wifi部分
    myWifiThread->start();
    //传输部分
    myTransDataThread->start();
}

void MainWindow::closeMonitor()//关闭监视器
{
    //线程等待
    //Kinect部分
    mKTd->terminate();
    mKTd->wait();
    //身份证部分
    myidCardReaderThread->terminate();
    myidCardReaderThread->wait();
    //wifi部分
    myWifiThread->terminate();
    myWifiThread->wait();
    //传输部分
    myTransDataThread->terminate();
    myTransDataThread->wait();

    //界面重置
    //video部分
    mKTd->myKinect->bodyIndexMat = cv::Mat::zeros(mKTd->myKinect->bodyIndexMat.size().height,mKTd->myKinect->bodyIndexMat.size().width,CV_8UC(3));
    mKTd->myKinect->color = cv::Mat::zeros(mKTd->myKinect->color.size().height,mKTd->myKinect->color.size().width,CV_8UC(3));
    mKTd->myKinect->depthMat8 = cv::Mat::zeros(mKTd->myKinect->depthMat8.size().height,mKTd->myKinect->depthMat8.size().width,CV_8UC(1));
    //分割部分
    mKTd->myKinect->blackTheSelectImage();
    //身份证部分
    myidCardReaderThread->myIdCardReader->idCardDataClear();
    //wifi部分
    myWifiThread->myWifi->wifiSaved = 0;
    //传输部分
    myTransDataThread->initData();
}

void MainWindow::paintEvent(QPaintEvent *event)//主界面更新
{
    //Kinect部分
    //视频部分（实时更新）
    ui->bodyVideo->setPixmap(Mat2QPixmap(bodyVideo));
    ui->colorVideo->setPixmap(Mat2QPixmap(colorVideo));
    ui->depthVideo->setPixmap(Mat2QPixmap(depthVideo));

    //分割部分 目前是和Kinect自身的机制绑定，实现方式和video部分是一样的，这里采用的是离开的时候更新，直到下一次出现为止，但如果要随时更新的话，需要修改Kinect内部的函数（目前是实时更新，可以一次更新）

    ui->allPartsImg->setPixmap(Mat2QPixmap(allPartsImage));
    ui->headImg->setPixmap(Mat2QPixmap(headImage));
    ui->bodyImg->setPixmap(Mat2QPixmap(bodyImage));
    ui->rightBigArmImg->setPixmap(Mat2QPixmap(rightBigArmImage));
    ui->rightSmallArmImg->setPixmap(Mat2QPixmap(rightSmallArmImage));
    ui->leftBigArmImg->setPixmap(Mat2QPixmap(leftBigArmImage));
    ui->leftSmallArmImg->setPixmap(Mat2QPixmap(leftSmallArmImage));
    ui->rightBigLegImg->setPixmap(Mat2QPixmap(rightBigLegImage));
    ui->rightSmallLegImg->setPixmap(Mat2QPixmap(rightSmallLegImage));
    ui->leftBigLegImg->setPixmap(Mat2QPixmap(leftBigLegImage));
    ui->leftSmallLegImg->setPixmap(Mat2QPixmap(leftSmallLegImage));

    //身份证部分（需要实时更新，因为其是在Kinect进入区域之后进行更新，无法但通过Kinect来完成绑定更新）
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("GBK"));//这句很关键
    ui->idCardImg->setPixmap(Mat2QPixmap(idCardImg));
    ui->nameText->setText(QString::fromLocal8Bit(myidCardReaderThread->myIdCardReader->idcardData.Name));
    ui->sexText->setText(QString::fromLocal8Bit(myidCardReaderThread->myIdCardReader->idcardData.Sex));
    ui->birthText->setText(QString::fromLocal8Bit(myidCardReaderThread->myIdCardReader->idcardData.Born));
    ui->peopleText->setText(QString::fromLocal8Bit(myidCardReaderThread->myIdCardReader->idcardData.Nation));
    ui->idText->setText(QString::fromLocal8Bit(myidCardReaderThread->myIdCardReader->idcardData.IDCardNo));
    ui->addressText->setText(QString::fromLocal8Bit(myidCardReaderThread->myIdCardReader->idcardData.Address));
    ui->govenText->setText(QString::fromLocal8Bit(myidCardReaderThread->myIdCardReader->idcardData.GrantDept));
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));//这句很关键
    //Mac地址结果显示(实时更新) 采集部分 可以一次更新但机制比较复杂，暂时图省事吧
    if(myWifiThread->myWifi->wifiSaved == 1)
    {
        for(int i = 0; i < myWifiThread->myWifi->outDataDing.size(); ++i )
        {
            model->setItem(i,0,new QStandardItem(myWifiThread->myWifi->outDataDing[i].macName.c_str()));

            model->item(i,0)->setTextAlignment(Qt::AlignCenter);
            string tmp1 = to_string(myWifiThread->myWifi->outDataDing[i].avgRssi);
            string tmp2 = to_string(myWifiThread->myWifi->outDataDing[i].num);

            model->setItem(i,1,new QStandardItem(tmp1.c_str()));
            model->item(i,1)->setTextAlignment(Qt::AlignCenter);
            model->setItem(i,2,new QStandardItem(tmp2.c_str()));
            model->item(i,2)->setTextAlignment(Qt::AlignCenter);
        }
    }
    else
    {
        model->removeRows(0,model->rowCount());//清空所有行
    }

}

QPixmap MainWindow::Mat2QPixmap(Mat cvImg) //用于Mat到QPixmap的转换(不提供色域转换)
{
    QImage qImg;
    if(cvImg.channels()==3)                        //3 channels color image
    {
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
    return QPixmap::fromImage(qImg);
}

void MainWindow::closeEvent(QCloseEvent *event)//退出按钮触发的指令
{
    //先关闭线程
    //Kinect部分
    mKTd->terminate();
    mKTd->wait();
    //身份证部分
    myidCardReaderThread->terminate();
    myidCardReaderThread->wait();
    //wifi部分
    myWifiThread->terminate();
    myWifiThread->wait();
    //传输部分
    myTransDataThread->terminate();
    myTransDataThread->wait();
    //最后调用原始的关闭操作
    QMainWindow::closeEvent(event);
}

void MainWindow::on_startButton_clicked() //用于开/关监控
{
    if(clickTime % 2 == 0)
    {
        openMonitor();
        ui->startButton->setText(tr("关闭监控"));
        clickTime++;
    }else
    {
        closeMonitor();
        ui->startButton->setText(tr("开始监控"));
        clickTime = 0;
    }
}

void MainWindow::renewFlag()//参数更新以及数据更新 放在定时器里
{
    myidCardReaderThread->myIdCardReader->enterFlag = mKTd->myKinect->T_enter_flag;//把kinect的标志更新到身份证
    myWifiThread->myWifi->waitFlag = !mKTd->myKinect->T_enter_flag;//把kinect的标志更新到wifi

    //数据更新 方便转存数据到发送端
    if(!myTransDataThread->sendOnceFlag)
    {
        myTransDataThread->wifiSaved = myWifiThread->myWifi->wifiSaved;
        myTransDataThread->ipSTR = myWifiThread->myWifi->ipSTR;
        myTransDataThread->T_enter_time = mKTd->myKinect->T_enter;
        myTransDataThread->T_leave_time = mKTd->myKinect->T_leave;
        memcpy(myTransDataThread->recvBuf,&(myidCardReaderThread->myIdCardReader->idcardData),sizeof(IDCardData));
        myTransDataThread->outdataDing = myWifiThread->myWifi->outDataDing;
        myTransDataThread->segmentStr = std::string(mKTd->myKinect->showImageCharArray[0]);
    }
    if(mKTd->myKinect->T_enter_flag==1)
    {
        myTransDataThread->sendOnceFlag=0;
    }
}

void MainWindow::on_settingButton_clicked()//设置按钮
{
    //ui->statusBar->showMessage("打开了新的对话框",3000); //状态栏显示（以后再完善）
    if(!myDialogForSettings)
    {
        myDialogForSettings = new DialogForSettings(this);
    }

    //传递ini数据 两个路径
    connect(myDialogForSettings,&DialogForSettings::emitSettingsDataToMainWindow,this,&MainWindow::getSettingsFormSetDialog);//建立通信
    connect(this,&MainWindow::emitSettingsToSetDialog,myDialogForSettings,&DialogForSettings::getSeetingDataFormMainWindow);//建立通信

    //传递数据
    emit emitSettingsToSetDialog(settingData,detectExePath,iniFilePath);

    //先在一开始的时候将进度条赋值（只更新一次）
    myDialogForSettings->setSliderValue(settingData.wifiThd);

    //显示窗口
    myDialogForSettings->show();
    myDialogForSettings->raise();
    myDialogForSettings->activateWindow();

}

void MainWindow::getSettingsFormSetDialog(iniData sData, string dPath, string iPath)//将数据返回并且重新设置参数
{
    settingData = sData;
    detectExePath = dPath;
    iniFilePath = iPath;
    //然后一堆重新设置的指令
    //重新设置Kinect
    //xml路径修改xml参数
    std::cout<<transPath(settingData.xmlPath).c_str()<<std::endl;
    mKTd->myKinect->setTheSetting(transPath(settingData.xmlPath).c_str());
    //修改图片存取路径 新建文件夹
    mKTd->myKinect->newFolder(transPath(settingData.savePath).c_str());
    mKTd->myKinect->saveFileFolderPathStr = settingData.savePath;
    //修改身份证路径
    myidCardReaderThread->myIdCardReader->setSavePhotoPath(transPath(settingData.idCardPath).c_str());
    //重新设置探针端口(只能修改端口)
    myWifiThread->myWifi->setTHDFunc(settingData.wifiThd);
}

void MainWindow::on_historyButton_clicked()//开启查询界面(与监控界面是不相关的)
{
    if(!myDialogForFind)
    {
        myDialogForFind = new DialogForFind;
    }
    myDialogForFind->show();
    myDialogForFind->raise();
    myDialogForFind->activateWindow();
}
