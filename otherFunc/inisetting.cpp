#include "inisetting.h"

void readIniFile(const char *srcPath, iniData &data)
{
    QSettings *configIniRead = new QSettings(srcPath,QSettings::IniFormat);
    data.wifiMac = configIniRead->value("/wifi/MacAddress").toString().toStdString();
    data.wifiPort = configIniRead->value("/wifi/serverPort").toInt();
    data.wifiThd = configIniRead->value("/wifi/threshold").toInt();
    data.xmlPath = configIniRead->value("/kinect/xmlPath").toString().toStdString();
    data.savePath = configIniRead->value("/kinect/savePath").toString().toStdString();
    data.idCardPath = configIniRead->value("/idCard/idCardImgSavePath").toString().toStdString();

    delete configIniRead;
}

void writeIniFile(const char *srcPath, iniData &data)
{
    QSettings *configIniWrite = new QSettings(srcPath,QSettings::IniFormat);
    configIniWrite->setValue("/wifi/MacAddress",data.wifiMac.c_str());
    configIniWrite->setValue("/wifi/serverPort",data.wifiPort);
    configIniWrite->setValue("/wifi/threshold",data.wifiThd);
    configIniWrite->setValue("/kinect/xmlPath",data.xmlPath.c_str());
    configIniWrite->setValue("/kinect/savePath",data.savePath.c_str());
    configIniWrite->setValue("/idCard/idCardImgSavePath",data.idCardPath.c_str());
    delete configIniWrite;
}
