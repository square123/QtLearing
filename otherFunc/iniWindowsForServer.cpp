#include "iniWindowsForServer.h"

//�ο���������
//https://blog.csdn.net/m_buddy/article/details/54097131

int readIniFile(const char * srcPath, iniData & outData)
{
	//�Ȱ��ļ����뵽map
	ifstream inFile(srcPath);
	if (!inFile)
	{
		return -1;
	}
	map<string,map<string,string>> tmp; //root key value
	string root;
	string line_text;
	while (getline(inFile,line_text))
	{
		int left_pos = 0;
		int right_pos = 0;
		int mid_pos = 0;

		//��rootֵ
		if((line_text.npos!=(left_pos = line_text.find("[")))&&(line_text.npos!=(right_pos = line_text.find("]"))))
		{
			root = line_text.substr(left_pos+1,right_pos-1);
			continue;
		}
		//����Ҫע�⣬����ļ�ֱ�ӳ��� key=value ��ǰ�������ͻ����� 
		if (line_text.npos!=(mid_pos = line_text.find("=")))
		{
			string keyStr = line_text.substr(0,mid_pos);
			string valueStr = line_text.substr(mid_pos+1,line_text.size()-1);
			tmp[root].insert(pair<string, string>(keyStr,valueStr));
			continue;
		}
	}
	inFile.close();
	//�ٰ����ݵ��뵽�̶��Ľṹ��
	outData.clientNum=stoi(tmp["client"]["number"]);
	outData.clientPort=stoi(tmp["client"]["port"]);
	outData.whiteListTimeInterval=stoi(tmp["whiteList"]["timeInterval"]);
	outData.whiteListPort=stoi(tmp["whiteList"]["port"]);
	outData.bufferSize=stoi(tmp["buffer"]["size"]);
	outData.idcardPath = tmp["idcard"]["path"];
	outData.mysqlDataBaseName =tmp["mysql"]["dataBaseName"];
	outData.mysqlHost = tmp["mysql"]["host"];
	outData.mysqlUser = tmp["mysql"]["user"];
	outData.mysqlPort = stoi(tmp["mysql"]["port"]);
	outData.mysqlPassword = tmp["mysql"]["password"];

	return 0;
}

int writeIniFile(const char * outPath, iniData & srcData)
{
	//�Ƚ�����д�뵽Map��
	map<string,map<string,string>> tmp; //root key value

	tmp["buffer"]["size"] = to_string(srcData.bufferSize);
	tmp["whiteList"]["port"] = to_string(srcData.whiteListPort);
	tmp["client"]["port"] = to_string(srcData.clientPort);
	tmp["client"]["number"] = to_string(srcData.whiteListTimeInterval);
	tmp["whiteList"]["timeInterval"]=to_string(srcData.whiteListTimeInterval);
	tmp["idcard"]["path"] = srcData.idcardPath;

	tmp["mysql"]["dataBaseName"] = srcData.mysqlDataBaseName;
	tmp["mysql"]["host"] = srcData.mysqlHost;
	tmp["mysql"]["user"] = srcData.mysqlUser;
	tmp["mysql"]["port"] = to_string(srcData.mysqlPort);
	tmp["mysql"]["password"] = srcData.mysqlPassword;

	//д�뵽�ļ�
	ofstream outFile(outPath);
	if (!outFile)
	{
		return -1;
	}
	for (auto i:tmp)
	{
		outFile<<"["<<i.first<<"]"<<endl;
		for(auto ii:i.second)
		{
			outFile<<ii.first<<"="<<ii.second<<endl;
		}
		outFile<<endl;
	}
	outFile.close();
	return 0;
}