#include "dialogForSettings.h"
#include "ui_dialogForSettings.h"

DialogForSettings::DialogForSettings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogForSettings)
{
    ui->setupUi(this);
    this->setFixedSize(this->width(),this->height());//固定尺寸
    myDialogForDetect =0;

    //变量初始化
    memset(&settingData,0,sizeof(settingData));
    memset(&tmpSettingData,0,sizeof(settingData));

    //设置窗口属性
    layout()->setSizeConstraint(QLayout::SetFixedSize);
    ui->groupBox_otherPath->hide();

    //界面控件的设置
    ui->lineText_box1->setEnabled(false);
    ui->lineText_box1->setFocusPolicy(Qt::NoFocus);
    ui->lineText_box2->setEnabled(false);
    ui->lineText_box2->setFocusPolicy(Qt::NoFocus);
    //    ui->remoteIP->setEnabled(false);
    //    ui->remoteIP->setFocusPolicy(Qt::NoFocus);
    //    ui->remotePort_data->setEnabled(false);
    //    ui->remotePort_data->setFocusPolicy(Qt::NoFocus);
    ui->remotePort_whiteList->setEnabled(false);
    ui->remotePort_whiteList->setFocusPolicy(Qt::NoFocus);
    ui->lineEdit_idcard->setEnabled(false);
    ui->lineEdit_idcard->setFocusPolicy(Qt::NoFocus);
    ui->lineEdit_iniPath->setEnabled(false);
    ui->lineEdit_iniPath->setFocusPolicy(Qt::NoFocus);
    ui->lineEdit_segment->setEnabled(false);
    ui->lineEdit_segment->setFocusPolicy(Qt::NoFocus);
    ui->wifiPortLineText->setEnabled(false);
    ui->wifiPortLineText->setFocusPolicy(Qt::NoFocus);
    ui->wifiThdLineText->setEnabled(false);
    ui->wifiThdLineText->setFocusPolicy(Qt::NoFocus);
    ui->MacAddress->setEnabled(false);
    ui->MacAddress->setFocusPolicy(Qt::NoFocus);
    ui->horizontalSlider_wifiThd->setMaximum(-20);
    ui->horizontalSlider_wifiThd->setMinimum(-70);

    connect(ui->horizontalSlider_wifiThd,SIGNAL(valueChanged(int)),this,SLOT(setThdLineTextValue(int)));
    connect(&theTimer,&QTimer::timeout,this,&DialogForSettings::showSettingData);
    theTimer.start(30);

}

DialogForSettings::~DialogForSettings()
{
    delete myDialogForDetect;
    myDialogForDetect =0;
    delete ui;
    ui= 0;
}

void DialogForSettings::setSliderValue(int thdVal)//用于初始的赋值
{
    ui->horizontalSlider_wifiThd->setValue(thdVal);
    ui->wifiThdLineText->setText(QString(std::to_string(thdVal).c_str()));
}

void DialogForSettings::setThdLineTextValue(int value)//与滑动条绑定
{
    int pos = ui->horizontalSlider_wifiThd->value();
    QString str = QString("%1").arg(pos);
    ui->wifiThdLineText->setText(str);
    tmpSettingData.wifiThd = pos;
}

void DialogForSettings::showSettingData()//用于将传递的数据显示出来
{
    //用于不变的显示
    ui->MacAddress->setText(QString(tmpSettingData.wifiMac.c_str()));
    ui->spinBox_interval->setValue(tmpSettingData.wifiTimeInterval);
    ui->lineEdit_idcard->setText(QString(tmpSettingData.idCardPath.c_str()));
    ui->lineEdit_iniPath->setText(QString(tmpIniFilePath.c_str()));
    ui->lineEdit_segment->setText(QString(tmpSettingData.savePath.c_str()));
    ui->wifiPortLineText->setText(QString(std::to_string(tmpSettingData.wifiPort).c_str()));
    ui->remoteIP->setText(QString(tmpSettingData.remoteIP.c_str()));
    ui->remotePort_data->setText(QString(std::to_string(tmpSettingData.remotePort).c_str()));
    ui->remotePort_whiteList->setText(QString(std::to_string(tmpSettingData.whitePort).c_str()));
    ui->lineText_box1->setText(tr("(%1,%2,%3,%4,%5,%6,%7)").arg(tmpSettingData.area1[0]).arg(tmpSettingData.area1[1]).arg(tmpSettingData.area1[2]).arg(tmpSettingData.area1[3]).arg(tmpSettingData.area1[4]).arg(tmpSettingData.area1[5]).arg(tmpSettingData.area1[6]));
    ui->lineText_box2->setText(tr("(%1,%2,%3,%4,%5)").arg(tmpSettingData.area2[0]).arg(tmpSettingData.area2[1]).arg(tmpSettingData.area2[2]).arg(tmpSettingData.area2[3]).arg(tmpSettingData.area2[4]));
}

void DialogForSettings::getSettingDataFormMainWindow(iniData sData, std::string iPath)
{
    //接受到的变量
    settingData = sData;
    iniFilePath = iPath;
    //存储到临时变量中
    tmpSettingData = sData;
    tmpIniFilePath = iPath;
}

void DialogForSettings::on_pushButton_detect_clicked()
{
    if(!myDialogForDetect)
    {
        myDialogForDetect = new DialogForDetect;
    }
    //建立联系
    connect(myDialogForDetect,&DialogForDetect::emitSaveValueToSettingDialog,this,&DialogForSettings::getDetectDataFormDialogForDetect);
    //显示窗口
    myDialogForDetect->show();
    myDialogForDetect->raise();
    myDialogForDetect->activateWindow();
}

//接受对话框的数据
void DialogForSettings::getDetectDataFormDialogForDetect(std::vector<int> arg1, std::vector<int> arg2)
{
    //对读取到的数据进行判断-------------------先对ini 文件进行重新修改
    if((arg1[0]!=0)&&(arg1[5]!=0)&&(arg1[6]!=0))
    {
        memcpy(tmpSettingData.area1,&arg1[0],sizeof(int)*7);
    }
    if((arg2[0]!=0)&&(arg2[3]!=0)&&(arg2[4]!=0))
    {
        memcpy(tmpSettingData.area2,&arg2[0],sizeof(int)*5);
    }
}

void DialogForSettings::on_pushButton_save_clicked()//保存并写入到ini文件中
{
    //将tmp转正
    settingData = tmpSettingData;
    //写入到ini文件中
    writeIniFile(iniFilePath.c_str(),settingData);
    //发射信号给主窗口
    emit emitSettingsDataToMainWindow(settingData,iniFilePath);
    this->accept();//退出对话框
}

void DialogForSettings::on_pushButton_segment_clicked()//修改分割的文件夹路径
{
    QString dir = QFileDialog::getExistingDirectory(this,tr("打开文件夹"),"",QFileDialog::ShowDirsOnly|QFileDialog::DontResolveSymlinks);
    tmpSettingData.savePath = transPath2(dir.toStdString());
}

void DialogForSettings::on_pushButton_idcard_clicked()//修改身份证照片的文件夹路径
{
    QString dir = QFileDialog::getExistingDirectory(this,tr("打开文件夹"),"",QFileDialog::ShowDirsOnly|QFileDialog::DontResolveSymlinks);
    tmpSettingData.idCardPath = transPath2(dir.toStdString());
}

void DialogForSettings::on_pushButton_iniChoose_clicked()//选择ini文件的路径
{
    QString src= QFileDialog::getOpenFileName(this,tr("导入ini文件"),"",tr("ini(*.ini)"));
    tmpIniFilePath = transPath2(src.toStdString());
}

void DialogForSettings::on_pushButton_iniSave_clicked()//保存文件路径
{
    iniFilePath = tmpIniFilePath;
}

void DialogForSettings::on_spinBox_interval_valueChanged(int arg1)
{
    tmpSettingData.wifiTimeInterval = arg1;
}
