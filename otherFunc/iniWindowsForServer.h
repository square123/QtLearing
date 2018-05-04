#ifndef INIWINDOW_H
#define INIWINDOW_H

#include <map>
#include <fstream>
#include <string>

using namespace std;

struct iniData
{
	int clientNum; //客户端数量
	int clientPort; //客户端端口
	int whiteListTimeInterval; //白名单更新时间间隔
	int whiteListPort;//白名单端口
	int bufferSize;//buffer的尺寸设置
	std::string idcardPath;//服务器存储接收照片的路径
	std::string mysqlHost;//MySQL ip地址
	std::string mysqlUser;//MySQL 用户名
	std::string mysqlPassword;//MySQL 密码
	std::string mysqlDataBaseName;//MySQL 数据库命名
	int mysqlPort;//MySQL端口

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

//配套的读写函数
int readIniFile(const char *, iniData &);
int writeIniFile(const char *, iniData &);

#endif