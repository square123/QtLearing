#include "myThreadBuffer.h"

//�漰��һЩ��ĳ�ʼ��
myThreadBuffer::myThreadBuffer()
{
	//���ݵĳ�ʼ��
	threadNumberOfClient=0;
	threadSmallBufferCapacity=0;
	threadBigBufferCapacity=0;

	//����
	memset(bigReadWrite,0,sizeof(atomic_int)*2);

	//��ʼ��
	bigReadWrite[0]=0;
	bigReadWrite[1]=1;
}

//����
void myThreadBuffer::setMyThreadBuffer(const int numberOfClient,const int smallBufferCapacity,const int bigBufferCapacity)
{
	//�����ڵĲ�����ֵ
	threadNumberOfClient=numberOfClient;
	threadSmallBufferCapacity=smallBufferCapacity;
	threadBigBufferCapacity=bigBufferCapacity;

	//����ռ䣨&���㣩
	smallReadWrite = new atomic_int *[threadNumberOfClient];
	for (int i=0;i<threadNumberOfClient;i++)
	{
		smallReadWrite[i]= new atomic_int[2];//һ��һд
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

	//��ʼֵ��ֵ
	for(int i=0;i<threadNumberOfClient;++i)
	{
		smallReadWrite[i][0]=0;//��ָ��
		smallReadWrite[i][1]=1;//дָ��
	}

}

//����һЩ��άָ����ͷ�
myThreadBuffer::~myThreadBuffer()
{
	//�ͷſռ�
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

//��д����
void myThreadBuffer::readDataFromSmallBuffer(int clientIndex,atomic_int &pRead, atomic_int &pWrite, vector<transDataStruct> &outputData)
{
	int length=calReadWriteLength(pRead,pWrite,threadSmallBufferCapacity);
	if(pRead<threadSmallBufferCapacity&&pWrite<threadSmallBufferCapacity)
	for (int i = 0; i < length-1; i++)//����Ϊ1�ǲ�����
	{
		transDataStruct temp;
		memset(&temp,0,sizeof(transDataStruct));
		pRead=((pRead+1)%threadSmallBufferCapacity);//������ǰ��������
		memcpy(&temp,&smallBuffer[clientIndex][pRead],sizeof(transDataStruct));
		outputData.push_back(temp);
		resetZeroForSmallBuffer(clientIndex,pRead);//�������
	}
}

void myThreadBuffer::writeDataFromSmallBuffer(int clientIndex,atomic_int &pRead, atomic_int &pWrite, transDataStruct &inputData)
{
	if(pRead<threadSmallBufferCapacity&&pWrite<threadSmallBufferCapacity)
	if (pWrite!=pRead)//�����ڲſ��Դ洢����
	{
		memcpy(&smallBuffer[clientIndex][pWrite],&inputData,sizeof(transDataStruct));
		pWrite=((pWrite+1)%threadSmallBufferCapacity);//�洢��Ÿ�������
	}
}

void myThreadBuffer::readDataFromBigBuffer(atomic_int &pRead, atomic_int &pWrite, vector<transDataStruct> &outputData)
{
	int length=calReadWriteLength(pRead,pWrite,threadBigBufferCapacity);
	if(pRead<threadBigBufferCapacity&&pWrite<threadBigBufferCapacity)
	for (int i = 0; i < length-1; i++)//����Ϊ1�ǲ�����
	{
		transDataStruct temp;
		memset(&temp,0,sizeof(transDataStruct));
		pRead=((pRead+1)%threadBigBufferCapacity);//������ǰ��������
		memcpy(&temp,&bigBuffer[pRead],sizeof(transDataStruct));
		outputData.push_back(temp);
		resetZeroForBigBuffer(pRead);//�������
	}
}

void myThreadBuffer::writeDataFromBigBuffer(atomic_int &pRead, atomic_int &pWrite, vector<transDataStruct> &inputData)
{
	if(pRead<threadBigBufferCapacity&&pWrite<threadBigBufferCapacity)
	for (int i = 0; i < inputData.size(); ++i)
	{
		if (pWrite!=pRead)//�����ڲſ��Դ洢����
		{
			memcpy(&bigBuffer[pWrite],&inputData[i],sizeof(transDataStruct));
			pWrite=((pWrite+1)%threadBigBufferCapacity);//�洢��Ÿ�������
		}
	}
}

//�����дָ��ĳ���
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

//��պ���
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