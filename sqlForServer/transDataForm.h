#pragma once
//通用的协议包和通用处理函数
#include<string>
#include<fstream>
#include<sstream>
#include<vector>
#include <ctime>

using namespace std;

//路径转换函数(将'\'转换成"//")
string transPath(string src);//no ref can not change the src
//     '/'to'//'
string transPath2(string src);

//time_t转换成timestamp 
string timeIntToTimestampString(time_t input);

//数据收集和排序用到的结构
struct mapUsed  //在排序时要使用的格式
{
	string macName;
	int avgRssi;
	int num;
	double score; //后面要计算的分数

    mapUsed & operator =(mapUsed src)//  = func not test
    {
        macName = src.macName;
        avgRssi = src.avgRssi;
        num = src.num;
        score = src.score;
        return *this;
    }
};

//字符数组转mapUsed
void charArrayToMapUsed(char *src,unsigned char &srcNum, vector<mapUsed> &dst);

//mapUsed转字符数组
void mapUsedToCharArray(vector<mapUsed> &src, char *dst, unsigned char &dstNum);

//传输协议数据帧结构
struct transDataStruct //一个数据帧的大小不用设置的很大，50K已经足够大了 共char[50000]   
{
	char IDImg[38862]; //身份证读卡器读取的图片数据  注意所有的长度一定要固定
	char IDCardDat[1024];//从身份证读卡器读到的文本数据 （1024是身份证读卡器默认使用的buffer大小）
	char performancePath[100];//人体外貌信息存储的路径
	char ClientIP[20];//客户端的IP地址 (便于去反向查找存储的最近人物的外貌细信息)
	char enterTime[15];//旅客进入检测域时间
	char leaveTime[15];//旅客离开检测域时间
	unsigned char macTextNum; //用1个字节来作为统计MAC地址数量，方便后续转换 最大可以表示为255，但在那么短短的时间里，不可能会在15秒内统计255种MAC地址，因此不可能 
	char macText[9963];//MAC地址的文本大小，相当于有9K的存储空间，在温州测试过，一个人在机场办理手续最多才1K ，一条记录经过统计一般是35字节，这里设置一条信息的字长为38 || 255*38=9690<9964 因此空间远远的够
}; 

//将数据整合成TransDataStruct
void becomeTransDataStruct(transDataStruct *dst,char *srcIdImg, char *idData, const char* performPath, const char *IP, const char *eTime, const char *lTime, vector<mapUsed> &srcMap);

//格式化处理帧结构 & 保存图片
class transDataFormed 
{
public:	
	//文本信息
	string idStr;
	string nameStr;
	string sexStr;
	string peopleStr;
	string birthDateStr;
	string addressStr;
	string performanceStr;
	string IPstr;
	string enterTimeStr;
	string leaveTimeStr;
	unsigned char macNumber;
	vector<mapUsed> macTextVec;

	//处理函数
	transDataFormed(transDataStruct *src);
	//存储照片
	void saveImg(const char *savePath);

private:
	char IDImg[38862];//直接拷贝下来的图像二进制文件
};

