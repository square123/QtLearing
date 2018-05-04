#include "wifithread.h"

wifiThread::wifiThread(const char* setMac,int setPort, int setTHD)
{
    //新建类
    myWifi = new Wifi(setMac,setPort,setTHD);
}

wifiThread::~wifiThread()
{
    delete myWifi;
}

void wifiThread::run() //写机制 受两个因素影响
{
    while(1)
    {
        if (myWifi->waitFlag==true&&myWifi->fileFlag==true)   //"啥都不干"
        {
           // Sleep(100);//释放cpu
        }
        else if(myWifi->waitFlag==false&&myWifi->fileFlag==false) //wifi信息记录
        {
            myWifi->wifiProcess();
        }
        else if (myWifi->waitFlag==false&&myWifi->fileFlag==true)//wifi开始记录
        {
            myWifi->fileFlag=false;
            myWifi->wifiSaved=0;//重新归零   //为传送数据服务 传输服务那边也需要将wifiSaved置零
        }
        else if (myWifi->waitFlag==true&&myWifi->fileFlag==false)//结束后要完成的操作
        {
            myWifi->clearOutDataDing();//先将数据清空《--现在清空是为了给传输数据保留时间
            myWifi->wifiProcessedSimplify(myWifi->outDataDing,myWifi->ipSTR);//将数据处理后放入outdata
            //清空和一些结束操作
            myWifi->reSelMacRssi();//将数据清零 便于下一次操作
            myWifi->fileFlag=true;
            myWifi->wifiSaved=1;//表示记录好数据了
        }
    }
}

