#ifndef DIALOGFORSETTINGS_H
#define DIALOGFORSETTINGS_H

#include <QDialog>
#include "inisetting.h"
#include <cstring>//memset用
#include <QTimer> //用于绘制界面
#include <string>

#include <QFile>//文件存在
#include <QFileDialog>//文件存在
#include <QString>//文件存在
#include "transDataForm.h"//用于统一路径
#include "dialogForDetect.h" //打开画框程序

namespace Ui {
class DialogForSettings;
}

class DialogForSettings : public QDialog
{
    Q_OBJECT

public:
    explicit DialogForSettings(QWidget *parent = 0);
    ~DialogForSettings();
    void setSliderValue(int val);//为了在初始的时候将指赋好，不用循环更新

    //用于和MainWindow传输变量
    iniData settingData;
    std::string iniFilePath;
    //临时变量（用于确认是否修改数据，如果修改数据再将临时变量传递个长期变量）
    iniData tmpSettingData;
    std::string tmpIniFilePath;

public slots:
    void showSettingData();//用来再启动的时候将参数显示出来
    void setThdLineTextValue(int value);

signals:
    void emitSettingsDataToMainWindow(iniData sData,std::string iPath);//用于将设置数据传输给主窗口

public slots:
    void getSettingDataFormMainWindow(iniData sData,std::string iPath);//用于从主窗口接受数据
    void getDetectDataFormDialogForDetect(std::vector<int> arg1,std::vector<int> arg2);//用于接受从检测域得到的数据 并且要加以判断


private slots:
    void on_pushButton_detect_clicked();

    void on_pushButton_save_clicked();

    void on_pushButton_segment_clicked();

    void on_pushButton_idcard_clicked();

    void on_pushButton_iniChoose_clicked();

    void on_pushButton_iniSave_clicked();

    void on_spinBox_interval_valueChanged(int arg1);

private:
    Ui::DialogForSettings *ui;
    DialogForDetect *myDialogForDetect;
    QTimer theTimer;//用于显示

};

#endif // DIALOGFORSETTINGS_H
