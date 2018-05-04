#ifndef INISETTING_H
#define INISETTING_H

#include <QSettings>
#include <string>

//用于读取和写入ini文件
//ini文件的内容
//  wifi部分
//      Mac地址
//      端口
//      阈值
//  Kinect部分
//      xml路径
//      存取照片路径
//  身份证部分
//      路径

struct iniData
{
    std::string wifiMac;
    int wifiPort;
    int wifiThd;
    std::string xmlPath;
    std::string savePath;
    std::string idCardPath;
    iniData& operator =(iniData src)
    {
        this->wifiMac = src.wifiMac;
        this->wifiPort = src.wifiPort;
        this->wifiThd = src.wifiThd;
        this->xmlPath = src.xmlPath;
        this->savePath = src.savePath;
        this->idCardPath = src.idCardPath;
        return *this;
    }
};

void readIniFile(const char *, iniData &);

void writeIniFile(const char *, iniData &);

#endif // INISETTING_H
