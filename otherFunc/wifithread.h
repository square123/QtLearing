#ifndef WIFITHREAD_H
#define WIFITHREAD_H
#include "wifi.h"
#include <QThread>
class wifiThread :public QThread
{
    Q_OBJECT
public:
    wifiThread(const char* setMac,int setPort, int setTHD);
    ~wifiThread();
    Wifi *myWifi;
protected:
    void run();
};
#endif // WIFITHREAD_H
