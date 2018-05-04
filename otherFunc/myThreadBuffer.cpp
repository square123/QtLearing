#include "myThreadBuffer.h"

//涉及到一些类的初始化
myThreadBuffer::myThreadBuffer()
{
	//数据的初始化
	threadNumberOfClient=0;
	threadSmallBufferCapacity=0;
	threadBigBufferCapacity=0;

	//置零
	memset(bigReadWrite,0,sizeof(atomic_int)*2);

	//初始化
	bigReadWrite[0]=0;
	bigReadWrite[1]=1;
}

//调参
void myThreadBuffer::setMyThreadBuffer(const int numberOfClient,const int smallBufferCapacity,const int bigBufferCapacity)
{
	//给类内的参数赋值
	threadNumberOfClient=numberOfClient;
	threadSmallBufferCapacity=smallBufferCapacity;
	threadBigBufferCapacity=bigBufferCapacity;

	//申请空间（&置零）
	smallReadWrite = new atomic_int *[threadNumberOfClient];
	for (int i=0;i<threadNumberOfClient;i++)
	{
		smallReadWrite[i]= new atomic_int[2];//一读一写
		memset(smallReadWrite[i],0,2*sizeof(atomic_int));
	}

	smallBuffer = new transDataStruct *[threadNumberOfClient];
	for (int i=0;i<threadNumberOfClient;i++)
	{
		smallBuffer[i]= new transDataStruct[threadSmallBufferCapacity];
		memset(smallBuffer[i],0,threadSmallBufferCapacity*sizeof(transDataStruct));
	}

	bigBuffer = new transDataStruct[threadBigBufferCapacity];
	memset(bigBuffer,0,sizeof(transDataStruct)*threadBigBufferCapacity);

	//初始值赋值
	for(int i=0;i<threadNumberOfClient;++i)
	{
		smallReadWrite[i][0]=0;//读指针
		smallReadWrite[i][1]=1;//写指针
	}

}

//负责一些二维指针的释放
myThreadBuffer::~myThreadBuffer()
{
	//释放空间
	for(int i=0;i<threadNumberOfClient;i++)
	{
		delete smallReadWrite[i];
		smallReadWrite[i]=NULL;
		delete smallBuffer[i];
		smallBuffer[i]=NULL;
	}
	delete [threadNumberOfClient]smallReadWrite;
	smallReadWrite=NULL;	
	delete [threadNumberOfClient]smallBuffer;
	smallBuffer=NULL;

	delete []bigBuffer;
	bigBuffer=NULL;
}

//读写函数
void myThreadBuffer::readDataFromSmallBuffer(int clientIndex,atomic_int &pRead, atomic_int &pWrite, vector<transDataStruct> &outputData)
{
	int length=calReadWriteLength(pRead,pWrite,threadSmallBufferCapacity);
	if(pRead<threadSmallBufferCapacity&&pWrite<threadSmallBufferCapacity)
	for (int i = 0; i < length-1; i++)//长度为1是不读的
	{
		transDataStruct temp;
		memset(&temp,0,sizeof(transDataStruct));
		pRead=((pRead+1)%threadSmallBufferCapacity);//读数据前更新索引
		memcpy(&temp,&smallBuffer[clientIndex][pRead],sizeof(transDataStruct));
		outputData.push_back(temp);
		resetZeroForSmallBuffer(clientIndex,pRead);//读完清空
	}
}

void myThreadBuffer::writeDataFromSmallBuffer(int clientIndex,atomic_int &pRead, atomic_int &pWrite, transDataStruct &inputData)
{
	if(pRead<threadSmallBufferCapacity&&pWrite<threadSmallBufferCapacity)
	if (pWrite!=pRead)//不等于才可以存储东西
	{
		memcpy(&smallBuffer[clientIndex][pWrite],&inputData,sizeof(transDataStruct));
		pWrite=((pWrite+1)%threadSmallBufferCapacity);//存储完才更新索引
	}
}

void myThreadBuffer::readDataFromBigBuffer(atomic_int &pRead, atomic_int &pWrite, vector<transDataStruct> &outputData)
{
	int length=calReadWriteLength(pRead,pWrite,threadBigBufferCapacity);
	if(pRead<threadBigBufferCapacity&&pWrite<threadBigBufferCapacity)
	for (int i = 0; i < length-1; i++)//长度为1是不读的
	{
		transDataStruct temp;
		memset(&temp,0,sizeof(transDataStruct));
		pRead=((pRead+1)%threadBigBufferCapacity);//读数据前更新索引
		memcpy(&temp,&bigBuffer[pRead],sizeof(transDataStruct));
		outputData.push_back(temp);
		resetZeroForBigBuffer(pRead);//读完清空
	}
}

void myThreadBuffer::writeDataFromBigBuffer(atomic_int &pRead, atomic_int &pWrite, vector<transDataStruct> &inputData)
{
	if(pRead<threadBigBufferCapacity&&pWrite<threadBigBufferCapacity)
	for (int i = 0; i < inputData.size(); ++i)
	{
		if (pWrite!=pRead)//不等于才可以存储东西
		{
			memcpy(&bigBuffer[pWrite],&inputData[i],sizeof(transDataStruct));
			pWrite=((pWrite+1)%threadBigBufferCapacity);//存储完才更新索引
		}
	}
}

//求出读写指针的长度
int myThreadBuffer::calReadWriteLength(int pRead, int pWrite, int rollLength)
{
	if(pWrite<pRead)
	{
		return pWrite-pRead+rollLength;
	}
	else
	{
		return pWrite-pRead;
	}
}

//清空函数
void myThreadBuffer::resetZeroForSmallBuffer(int clientIndex, int targetIndex)
{
	if(clientIndex<threadNumberOfClient&&targetIndex<threadSmallBufferCapacity)
	memset(&smallBuffer[clientIndex][targetIndex],0,sizeof(transDataStruct));
}

void myThreadBuffer::resetZeroForBigBuffer(int targetIndex)
{
	if(targetIndex<threadBigBufferCapacity)
	memset(&bigBuffer[targetIndex],0,sizeof(transDataStruct));
}