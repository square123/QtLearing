#ifndef INIWINDOW_H
#define INIWINDOW_H

#include <map>
#include <fstream>
#include <string>

using namespace std;

struct iniData
{
	int clientNum; //�ͻ�������
	int clientPort; //�ͻ��˶˿�
	int whiteListTimeInterval; //����������ʱ����
	int whiteListPort;//�������˿�
	int bufferSize;//buffer�ĳߴ�����
	std::string idcardPath;//�������洢������Ƭ��·��
	std::string mysqlHost;//MySQL ip��ַ
	std::string mysqlUser;//MySQL �û���
	std::string mysqlPassword;//MySQL ����
	std::string mysqlDataBaseName;//MySQL ���ݿ�����
	int mysqlPort;//MySQL�˿�

	iniData& operator =(iniData src)
	{
		this->clientNum = src.clientNum;
		this->clientPort = src.clientPort;
		this->whiteListPort =src.whiteListPort;
		this->whiteListTimeInterval = src.whiteListTimeInterval;
		this->bufferSize = src.bufferSize;
		this->idcardPath = src.idcardPath;
		this->mysqlHost = src.mysqlHost;
		this->mysqlPassword = src.mysqlPassword;
		this->mysqlDataBaseName = src.mysqlDataBaseName;
		this->mysqlPort = src.mysqlPort;
		this->mysqlUser = src.mysqlUser;
		return *this;
	}
};

//���׵Ķ�д����
int readIniFile(const char *, iniData &);
int writeIniFile(const char *, iniData &);

#endif