#ifndef INISETTING_H
#define INISETTING_H

#include <QSettings>
#include <string>
#include <vector>

//用于读取和写入ini文件
//ini文件的内容
//  方框1
//      中
//      前--》为了和之前的类型对接
//      后
//      左上x
//      左上y
//      右下x
//      右下y
//  方框2
//      中
//      左上x
//      左上y
//      右下x
//      右下y
//  远程服务器
//      IP地址
//      数据端口
//      白名单端口
//  wifi部分
//      Mac地址
//      端口
//      阈值
//      时间间隔
//  Kinect部分
//      存取照片路径
//  身份证部分
//      路径

struct iniData
{
    int area1[7];
    int area2[5];
    std::string remoteIP;
    int remotePort;
    int whitePort;
    std::string wifiMac;
    int wifiPort;
    int wifiThd;
    int wifiTimeInterval;
    std::string savePath;
    std::string idCardPath;
    iniData& operator =(iniData src)
    {
        memcpy(this->area1,src.area1,sizeof(int)*7);
        memcpy(this->area2,src.area2,sizeof(int)*5);
        this->remoteIP = src.remoteIP;
        this->whitePort = src.whitePort;
        this->remotePort = src.remotePort;
        this->wifiMac = src.wifiMac;
        this->wifiPort = src.wifiPort;
        this->wifiThd = src.wifiThd;
        this->wifiTimeInterval = src.wifiTimeInterval;
        this->savePath = src.savePath;
        this->idCardPath = src.idCardPath;
        return *this;
    }
};

void readIniFile(const char *, iniData &);

void writeIniFile(const char *, iniData &);

#endif // INISETTING_H
