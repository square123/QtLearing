#pragma once
#include "transDataForm.h"
#include <atomic>

//buffer��
class myThreadBuffer
{
public:	
	myThreadBuffer();//�漰��һЩ��ĳ�ʼ��
	~myThreadBuffer();//��Ҫ�����ָ����ͷ�
	void setMyThreadBuffer(const int numberOfClient,const int smallBufferCapacity,const int bigBufferCapacity);//���ú궨���ˣ�ֱ�������ú���ȥ�޸�


	//��д����(
	void readDataFromSmallBuffer(int clientIndex,atomic_int &pRead, atomic_int &pWrite, vector<transDataStruct> &data);//��ʵ���ֻ�ǵ�ַ�Ļ���������ʱд�������������ã�д����ʱ����ʱ�򲻼�����
	void writeDataFromSmallBuffer(int clientIndex,atomic_int &pRead, atomic_int &pWrite, transDataStruct &data);//����Сbuffer��˵����ʵÿ�λ�ȡ������ֻ��һ��������д������
	void readDataFromBigBuffer(atomic_int &pRead, atomic_int &pWrite, vector<transDataStruct> &data);
	void writeDataFromBigBuffer(atomic_int &pRead, atomic_int &pWrite, vector<transDataStruct> &data);

	//��дָ��洢(ʹ��ԭ������������ʵ�о����ã���������ٶ����Ļ����԰�����ĵ�)//��һ���洢���Ƕ����ڶ����洢����д��дһ���ȶ�Ҫ�󡣵�д���ڶ�ʱ����������������д�����ִ�������������궨��Ĵ�С
	atomic_int **smallReadWrite;  
	atomic_int bigReadWrite[2];//һ��һд

private:
	//buffer���֣��������еĶ�дָ�뱾���϶���ֻ��һ���߳̿���ʹ�������ڷǼ�������£������ڼ��������⣩
	//����߳���ʹ�õ�Сbuffer
	transDataStruct **smallBuffer;
	//�������Ϻʹ������õĴ�buffer
	transDataStruct *bigBuffer;

	//�жϺ���
	int calReadWriteLength(int pRead, int pWrite, int rollLength);//�����дָ��ĳ���

	//��պ���
	void resetZeroForSmallBuffer(int clientIndex, int targetIndex);
	void resetZeroForBigBuffer(int targetIndex);

	//���ò��� ����궨��
	int threadNumberOfClient; 
	int threadSmallBufferCapacity;
	int threadBigBufferCapacity;
};