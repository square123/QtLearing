#include "transDataForm.h"

using namespace std;

//·��ת������(��'\'ת����"//")
string transPath(string src)
{
	std::string::size_type pos = 0;

	while ((pos=src.find('\\',pos))!=std::string::npos)
	{
		src.replace(pos,1,"//");
		pos+=2;
	}
	return src;
}

//'/' to "//"
string transPath2(string src)
{
    std::string::size_type pos = 0;

    while ((pos=src.find('/',pos))!=std::string::npos)
    {
        src.replace(pos,1,"//");
        pos+=2;
    }
    return src;
}

//time_tת����timestamp 
string timeIntToTimestampString(time_t input)
{
	tm *pTmp=localtime(&input);
	char pStr[15]={};
    sprintf(pStr,"%d%02d%02d%02d%02d%02d",pTmp->tm_year+1900,pTmp->tm_mon+1,pTmp->tm_mday,pTmp->tm_hour,pTmp->tm_min,pTmp->tm_sec);//fix some error about day
	return string(pStr);
}

//�ַ�����תmapUsed
void charArrayToMapUsed(char *src,unsigned char &srcNum, vector<mapUsed> &dst)
{
	//Ĭ��һ����Ϣ����38���ֽ� ��ʽ��XX:XX:XX:XX:XX:XX -XX X X.XXXXXX (38�ֽ��㹻)
	for(unsigned char i=0;i<srcNum;i++)
	{
		mapUsed tempMapUsed;
		string tempStr(src+(i*38*sizeof(char)),src+((i+1)*38*sizeof(char)));
		istringstream iTempStr(tempStr);
		iTempStr>>tempMapUsed.macName>>tempMapUsed.avgRssi>>tempMapUsed.num>>tempMapUsed.score;
		dst.push_back(tempMapUsed);
	}
}

//mapUsedת�ַ�����
void mapUsedToCharArray(vector<mapUsed> &src, char *dst, unsigned char &dstNum) //��ת��������double����һλ������ʧ����Ӱ�첻��Ͳ�������
{
	//Ĭ��һ����Ϣ����38���ֽ� ��ʽ��XX:XX:XX:XX:XX:XX -XX X X.XXXXXX (38�ֽ��㹻)
	dstNum=src.size();
	for(unsigned char i=0;i<dstNum;i++)
	{
		string tempStr;
		tempStr=src[i].macName+" "+to_string(src[i].avgRssi)+" "+to_string(src[i].num)+" "+to_string(src[i].score)+" ";
		memcpy(dst+i*38*sizeof(char),tempStr.c_str(),38*sizeof(char));
	}
}

//���������ϳ�TransDataStruct
void becomeTransDataStruct(transDataStruct *dst,char *srcIdImg, char *idData, const char* performPath, const char *IP, const char *eTime, const char *lTime, vector<mapUsed> &srcMap)
{
	memcpy(dst->IDImg,srcIdImg,38862);
	memcpy(dst->IDCardDat,idData,1024);
	memcpy(dst->performancePath,performPath,100);
	memcpy(dst->ClientIP,IP,20);
	memcpy(dst->enterTime,eTime,15);
	memcpy(dst->leaveTime,lTime,15);
	unsigned char num=0;
	char macTemp[9963]={0};
	mapUsedToCharArray(srcMap,macTemp,num);
	dst->macTextNum=num;
	memcpy(dst->macText,macTemp,9963);
}

//��ʽ������֡�ṹ & ����ͼƬ//������
transDataFormed::transDataFormed(transDataStruct *src)
{
	idStr=string((src->IDCardDat)+148);
	nameStr=string((src->IDCardDat));
	sexStr=string((src->IDCardDat)+32);
	peopleStr=string((src->IDCardDat)+38);
	birthDateStr=string((src->IDCardDat)+58);
	addressStr=string((src->IDCardDat)+76);
	performanceStr=string(src->performancePath);
	IPstr=string(src->ClientIP);
	enterTimeStr=string(src->enterTime);
	leaveTimeStr=string(src->leaveTime);
	macNumber=src->macTextNum;
	charArrayToMapUsed(src->macText,macNumber,macTextVec);
	memcpy(IDImg,src->IDImg,38862);
}

//�洢��Ƭ
void transDataFormed::saveImg(const char *savePath) //���ӣ�"F://XXX//XXXX//"  
{
	string dst=savePath;
	dst+=idStr;
	dst+=".bmp";
	ofstream imgfile(dst.c_str(),ios::binary);
	imgfile.write(IDImg,38862);// ����Ĵ�СĿǰ�ǹ̶����Ǹ������֤��������SDK�����Ƶ�
	imgfile.close();
}
