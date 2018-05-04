#pragma once
//ͨ�õ�Э�����ͨ�ô�����
#include<string>
#include<fstream>
#include<sstream>
#include<vector>
#include <ctime>

using namespace std;

//·��ת������(��'\'ת����"//")
string transPath(string src);//no ref can not change the src
//     '/'to'//'
string transPath2(string src);

//time_tת����timestamp 
string timeIntToTimestampString(time_t input);

//�����ռ��������õ��Ľṹ
struct mapUsed  //������ʱҪʹ�õĸ�ʽ
{
	string macName;
	int avgRssi;
	int num;
	double score; //����Ҫ����ķ���

    mapUsed & operator =(mapUsed src)//  = func not test
    {
        macName = src.macName;
        avgRssi = src.avgRssi;
        num = src.num;
        score = src.score;
        return *this;
    }
};

//�ַ�����תmapUsed
void charArrayToMapUsed(char *src,unsigned char &srcNum, vector<mapUsed> &dst);

//mapUsedת�ַ�����
void mapUsedToCharArray(vector<mapUsed> &src, char *dst, unsigned char &dstNum);

//����Э������֡�ṹ
struct transDataStruct //һ������֡�Ĵ�С�������õĺܴ�50K�Ѿ��㹻���� ��char[50000]   
{
	char IDImg[38862]; //���֤��������ȡ��ͼƬ����  ע�����еĳ���һ��Ҫ�̶�
	char IDCardDat[1024];//�����֤�������������ı����� ��1024�����֤������Ĭ��ʹ�õ�buffer��С��
	char performancePath[100];//������ò��Ϣ�洢��·��
	char ClientIP[20];//�ͻ��˵�IP��ַ (����ȥ������Ҵ洢������������òϸ��Ϣ)
	char enterTime[15];//�ÿͽ�������ʱ��
	char leaveTime[15];//�ÿ��뿪�����ʱ��
	unsigned char macTextNum; //��1���ֽ�����Ϊͳ��MAC��ַ�������������ת�� �����Ա�ʾΪ255��������ô�̶̵�ʱ��������ܻ���15����ͳ��255��MAC��ַ����˲����� 
	char macText[9963];//MAC��ַ���ı���С���൱����9K�Ĵ洢�ռ䣬�����ݲ��Թ���һ�����ڻ���������������1K ��һ����¼����ͳ��һ����35�ֽڣ���������һ����Ϣ���ֳ�Ϊ38 || 255*38=9690<9964 ��˿ռ�ԶԶ�Ĺ�
}; 

//���������ϳ�TransDataStruct
void becomeTransDataStruct(transDataStruct *dst,char *srcIdImg, char *idData, const char* performPath, const char *IP, const char *eTime, const char *lTime, vector<mapUsed> &srcMap);

//��ʽ������֡�ṹ & ����ͼƬ
class transDataFormed 
{
public:	
	//�ı���Ϣ
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

	//������
	transDataFormed(transDataStruct *src);
	//�洢��Ƭ
	void saveImg(const char *savePath);

private:
	char IDImg[38862];//ֱ�ӿ���������ͼ��������ļ�
};

