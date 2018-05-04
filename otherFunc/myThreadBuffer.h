#pragma once
#include "transDataForm.h"
#include <atomic>

//buffer类
class myThreadBuffer
{
public:	
	myThreadBuffer();//涉及到一些类的初始化
	~myThreadBuffer();//需要加入对指针的释放
	void setMyThreadBuffer(const int numberOfClient,const int smallBufferCapacity,const int bigBufferCapacity);//不用宏定义了，直接在设置函数去修改


	//读写函数(
	void readDataFromSmallBuffer(int clientIndex,atomic_int &pRead, atomic_int &pWrite, vector<transDataStruct> &data);//其实如果只是地址的话，读数据时写的索引不加引用，写数据时读的时候不加引用
	void writeDataFromSmallBuffer(int clientIndex,atomic_int &pRead, atomic_int &pWrite, transDataStruct &data);//对于小buffer来说，其实每次获取的数据只是一个，不用写成向量
	void readDataFromBigBuffer(atomic_int &pRead, atomic_int &pWrite, vector<transDataStruct> &data);
	void writeDataFromBigBuffer(atomic_int &pRead, atomic_int &pWrite, vector<transDataStruct> &data);

	//读写指针存储(使用原语来加锁，其实感觉不用，如果后续速度慢的话可以把这个改掉)//第一个存储的是读，第二个存储的是写，写一定比读要大。当写等于读时，数据已满不能再写。发现此类问题请增大宏定义的大小
	atomic_int **smallReadWrite;  
	atomic_int bigReadWrite[2];//一读一写

private:
	//buffer部分（由于所有的读写指针本质上都是只有一个线程可以使用它，在非极端情况下，不存在加锁的问题）
	//多个线程所使用的小buffer
	transDataStruct **smallBuffer;
	//数据整合和处理所用的大buffer
	transDataStruct *bigBuffer;

	//判断函数
	int calReadWriteLength(int pRead, int pWrite, int rollLength);//求出读写指针的长度

	//清空函数
	void resetZeroForSmallBuffer(int clientIndex, int targetIndex);
	void resetZeroForBigBuffer(int targetIndex);

	//设置参数 替代宏定义
	int threadNumberOfClient; 
	int threadSmallBufferCapacity;
	int threadBigBufferCapacity;
};