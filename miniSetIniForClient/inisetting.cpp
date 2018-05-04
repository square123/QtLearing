#include "inisetting.h"

void readIniFile(const char *srcPath, iniData &data)
{

    QSettings *configIniRead = new QSettings(srcPath,QSettings::IniFormat);

    data.area1[0] = configIniRead->value("/area1/med").toInt();
    data.area1[1] = configIniRead->value("/area1/front").toInt();
    data.area1[2] = configIniRead->value("/area1/back").toInt();
    data.area1[3] = configIniRead->value("/area1/tlx").toInt();
    data.area1[4] = configIniRead->value("/area1/tly").toInt();
    data.area1[5] = configIniRead->value("/area1/brx").toInt();
    data.area1[6] = configIniRead->value("/area1/bry").toInt();

    data.area2[0] = configIniRead->value("/area2/med").toInt();
    data.area2[1] = configIniRead->value("/area2/tlx").toInt();
    data.area2[2] = configIniRead->value("/area2/tly").toInt();
    data.area2[3] = configIniRead->value("/area2/brx").toInt();
    data.area2[4] = configIniRead->value("/area2/bry").toInt();

    data.remoteIP = configIniRead->value("/remote/ip").toString().toStdString();
    data.remotePort = configIniRead->value("/remote/dataPort").toInt();
    data.whitePort = configIniRead->value("/remote/whitePort").toInt();

    data.wifiMac = configIniRead->value("/wifi/MacAddress").toString().toStdString();
    data.wifiPort = configIniRead->value("/wifi/serverPort").toInt();
    data.wifiThd = configIniRead->value("/wifi/threshold").toInt();
    data.wifiTimeInterval = configIniRead->value("/wifi/timeInterval").toInt();

    data.savePath = configIniRead->value("/kinect/savePath").toString().toStdString();

    data.idCardPath = configIniRead->value("/idCard/idCardImgSavePath").toString().toStdString();

    delete configIniRead;
}

void writeIniFile(const char *srcPath, iniData &data)
{
    QSettings *configIniWrite = new QSettings(srcPath,QSettings::IniFormat);

    configIniWrite->setValue("/area1/med",data.area1[0]);
    configIniWrite->setValue("/area1/front",data.area1[1]);
    configIniWrite->setValue("/area1/back",data.area1[2]);
    configIniWrite->setValue("/area1/tlx",data.area1[3]);
    configIniWrite->setValue("/area1/tly",data.area1[4]);
    configIniWrite->setValue("/area1/brx",data.area1[5]);
    configIniWrite->setValue("/area1/bry",data.area1[6]);

    configIniWrite->setValue("/area2/med",data.area2[0]);
    configIniWrite->setValue("/area2/tlx",data.area2[1]);
    configIniWrite->setValue("/area2/tly",data.area2[2]);
    configIniWrite->setValue("/area2/brx",data.area2[3]);
    configIniWrite->setValue("/area2/bry",data.area2[4]);

    configIniWrite->setValue("/remote/ip",data.remoteIP.c_str());
    configIniWrite->setValue("/remote/dataPort",data.remotePort);
    configIniWrite->setValue("/remote/whitePort",data.whitePort);

    configIniWrite->setValue("/wifi/MacAddress",data.wifiMac.c_str());
    configIniWrite->setValue("/wifi/serverPort",data.wifiPort);
    configIniWrite->setValue("/wifi/threshold",data.wifiThd);
    configIniWrite->setValue("/wifi/timeInterval",data.wifiTimeInterval);

    configIniWrite->setValue("/kinect/savePath",data.savePath.c_str());

    configIniWrite->setValue("/idCard/idCardImgSavePath",data.idCardPath.c_str());
    delete configIniWrite;
}
