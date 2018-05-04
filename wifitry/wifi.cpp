#include "wifi.h"

//wifi构造函数（完成客户端白名单的读取和探针接受socket的处理）
Wifi::Wifi(const char* setMac,int setPort, int setTHD)
{
	//设置参数
	wifiMac=setMac;
	wifiPort=setPort;
	wifiTHD=setTHD;

	//白名单机制
	ifstream infile; 
	infile.open("deny.txt");   
	string sss;
	while(getline(infile,sss))//先读取一行的数据
	{
		denyList.insert(sss);
	}
	infile.close();             //关闭文件输入流 

	//与探针进行通信的初始化操作（UDP协议）
	if(WSAStartup(MAKEWORD(2,2),&wsd)!=0){
		printf("WSAStartup failed\n");
	}
	////创建嵌套字
	s=socket(AF_INET, SOCK_DGRAM, 0);
	if(s==INVALID_SOCKET){   
		printf("Failed socket() %d\n",WSAGetLastError());
		::WSACleanup();
		system("pause");
	}
	////绑定嵌套字
	servAddr.sin_family = AF_INET;
	servAddr.sin_port = htons(wifiPort);//port
	servAddr.sin_addr.S_un.S_addr=htonl(INADDR_ANY);//ip 
	if(bind(s,(SOCKADDR *)&servAddr,sizeof(servAddr)) == SOCKET_ERROR){
		printf("bind() failed: %d\n",WSAGetLastError());
		closesocket(s);
		::WSACleanup();
		system("pause");
	}
}

//析构函数 完成socket的关闭
Wifi::~Wifi()
{
	closesocket(s);
	::WSACleanup();
}

//记录Mac值和对应的RSSI值
void Wifi::wifiProcess()
{
	//socket
	int clientAddrLength = sizeof(clientAddr); 
	char buffer[BUFFER_SIZE];
	ZeroMemory(buffer, BUFFER_SIZE); 
	//定义接收
	if (recvfrom(s,buffer,BUFFER_SIZE,0,(SOCKADDR *)&clientAddr,&clientAddrLength) == SOCKET_ERROR)
	{
		printf("recvfrom() failed: %d\n",WSAGetLastError());
		closesocket(s);
		::WSACleanup();
		system("pause");
	}
	
	//把接收后的数据格式化
	mncatsWifi datatemp=mncatsWifi(buffer);

	//剔除denyList中的Mac地址，此布操作是查找操作
	auto deIt=find(denyList.begin(),denyList.end(),datatemp.mac2); //查找denyList中的元素，如果没找到，返回end()

	//剔除路由器的mac地址 必须是数据帧 & 信号强度大于阈值 & 信号强度不为0 & 不在白名单中 &  获得的数据必须是探针的MAC地址
	if((datatemp.dtype!="80")&&(int(datatemp.crssi)>wifiTHD)&&(int(datatemp.crssi)!=0)&&(deIt==denyList.end())&&(datatemp.mac1==wifiMac))
	{
		selMapDing[datatemp.mac2].push_back(int(datatemp.crssi)); 
	}
}

//项目wifi处理函数（已经废除，为了回顾就先保留下来，目前使用的是wifiProcessedSimplify 可以把这个函数连同定义删掉）
void  Wifi::wifiProcessed(vector<mapUsed> &dst,ofstream &raw,ofstream &rawPro,string enterTime)
{
	//这里要以身份证号作为文件命名，在每个文件中加入日志选项，还有时间输入
	raw<<enterTime<<" "<<selMapDing.size()<<endl; //时间 Mac个数

	//原始数据输出+处理
	for (auto const& name:selMapDing)
	{
		mapUsed temp;
		temp.macName=name.first;
		raw<<name.first<<" "<<name.second.size()<<" ";//Mac名称 个数 RSSI值
		temp.num=name.second.size();
		for (auto const & ttt:name.second) 
		{
			raw<<ttt<<" ";
		}
		raw<<endl;
		int sumTemp=accumulate(name.second.begin(),name.second.end(),0);
		temp.avgRssi=sumTemp/int(name.second.size());
		temp.score=temp.num/(temp.avgRssi==0?1000.0:abs(temp.avgRssi));//考虑有可能出现为0的情况，就把这个替换剔除掉
		dst.push_back(temp);
	}

	//处理后的数据
	rawPro<<enterTime<<" "<<selMapDing.size()<<endl; //时间 Mac个数
	sort(dst.begin(),dst.end(),[](mapUsed r1,mapUsed r2){return r1.score>r2.score;}); //最后按大小排序 //最后的排序方式是可以调整的 因为在分数中已经考虑到0的操作，因此在后续的输出时不需再考虑到0的情况
	for (int it=0;it<dst.size();it++) //将排序好的信息输出
	{
		rawPro<<dst[it].macName<<" "<<dst[it].avgRssi<<" "<<dst[it].num<<" "<<dst[it].score<<endl;
	}
}

//最新的wifi函数处理功能-------------主要完成了排序，并将之前的存储文件操作删除（将转换和发送分开，降低耦合度） 
void Wifi::wifiProcessedSimplify(vector<mapUsed> &srcMap,string enterTime,string leaveTime, char *idData, char *performPath, transDataStruct *dst) 
{	
	//处理记录下的数据
	for (auto const& name:selMapDing)
	{
		mapUsed temp;
		temp.macName=name.first;
		temp.num=name.second.size();
		int sumTemp=accumulate(name.second.begin(),name.second.end(),0);
		temp.avgRssi=sumTemp/int(name.second.size());
		temp.score=temp.num/(temp.avgRssi==0?1000.0:abs(temp.avgRssi));//考虑有可能出现为0的情况，就把这个替换剔除掉
		srcMap.push_back(temp);
	}

	//根据分数计算排序
	sort(srcMap.begin(),srcMap.end(),[](mapUsed r1,mapUsed r2){return r1.score>r2.score;}); //最后按大小排序 //最后的排序方式是可以调整的 因为在分数中已经考虑到0的操作，因此在后续的输出时不需再考虑到0的情况

	//读取图片  
	char idImg[38862]={0};
	ifstream imgf(transPath(string(idData+292)),ios::binary);//这个是从身份证文件路径下直接复制的
	imgf.read(idImg,sizeof(idImg));
	imgf.close();

	//打包数据
	becomeTransDataStruct(dst,idImg,idData,performPath,inet_ntoa(clientAddr.sin_addr),(char *)enterTime.c_str(),(char *)leaveTime.c_str(),srcMap);
}

//将记录的结构体数组重新置零
void Wifi::reSelMacRssi()
{
	selMapDing.erase(selMapDing.begin(),selMapDing.end()); //清空数据
}