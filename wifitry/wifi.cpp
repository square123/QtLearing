#include "wifi.h"

//wifi���캯������ɿͻ��˰������Ķ�ȡ��̽�����socket�Ĵ���
Wifi::Wifi(const char* setMac,int setPort, int setTHD)
{
	//���ò���
	wifiMac=setMac;
	wifiPort=setPort;
	wifiTHD=setTHD;

	//����������
	ifstream infile; 
	infile.open("deny.txt");   
	string sss;
	while(getline(infile,sss))//�ȶ�ȡһ�е�����
	{
		denyList.insert(sss);
	}
	infile.close();             //�ر��ļ������� 

	//��̽�����ͨ�ŵĳ�ʼ��������UDPЭ�飩
	if(WSAStartup(MAKEWORD(2,2),&wsd)!=0){
		printf("WSAStartup failed\n");
	}
	////����Ƕ����
	s=socket(AF_INET, SOCK_DGRAM, 0);
	if(s==INVALID_SOCKET){   
		printf("Failed socket() %d\n",WSAGetLastError());
		::WSACleanup();
		system("pause");
	}
	////��Ƕ����
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

//�������� ���socket�Ĺر�
Wifi::~Wifi()
{
	closesocket(s);
	::WSACleanup();
}

//��¼Macֵ�Ͷ�Ӧ��RSSIֵ
void Wifi::wifiProcess()
{
	//socket
	int clientAddrLength = sizeof(clientAddr); 
	char buffer[BUFFER_SIZE];
	ZeroMemory(buffer, BUFFER_SIZE); 
	//�������
	if (recvfrom(s,buffer,BUFFER_SIZE,0,(SOCKADDR *)&clientAddr,&clientAddrLength) == SOCKET_ERROR)
	{
		printf("recvfrom() failed: %d\n",WSAGetLastError());
		closesocket(s);
		::WSACleanup();
		system("pause");
	}
	
	//�ѽ��պ�����ݸ�ʽ��
	mncatsWifi datatemp=mncatsWifi(buffer);

	//�޳�denyList�е�Mac��ַ���˲������ǲ��Ҳ���
	auto deIt=find(denyList.begin(),denyList.end(),datatemp.mac2); //����denyList�е�Ԫ�أ����û�ҵ�������end()

	//�޳�·������mac��ַ ����������֡ & �ź�ǿ�ȴ�����ֵ & �ź�ǿ�Ȳ�Ϊ0 & ���ڰ������� &  ��õ����ݱ�����̽���MAC��ַ
	if((datatemp.dtype!="80")&&(int(datatemp.crssi)>wifiTHD)&&(int(datatemp.crssi)!=0)&&(deIt==denyList.end())&&(datatemp.mac1==wifiMac))
	{
		selMapDing[datatemp.mac2].push_back(int(datatemp.crssi)); 
	}
}

//��Ŀwifi���������Ѿ��ϳ���Ϊ�˻ع˾��ȱ���������Ŀǰʹ�õ���wifiProcessedSimplify ���԰����������ͬ����ɾ����
void  Wifi::wifiProcessed(vector<mapUsed> &dst,ofstream &raw,ofstream &rawPro,string enterTime)
{
	//����Ҫ�����֤����Ϊ�ļ���������ÿ���ļ��м�����־ѡ�����ʱ������
	raw<<enterTime<<" "<<selMapDing.size()<<endl; //ʱ�� Mac����

	//ԭʼ�������+����
	for (auto const& name:selMapDing)
	{
		mapUsed temp;
		temp.macName=name.first;
		raw<<name.first<<" "<<name.second.size()<<" ";//Mac���� ���� RSSIֵ
		temp.num=name.second.size();
		for (auto const & ttt:name.second) 
		{
			raw<<ttt<<" ";
		}
		raw<<endl;
		int sumTemp=accumulate(name.second.begin(),name.second.end(),0);
		temp.avgRssi=sumTemp/int(name.second.size());
		temp.score=temp.num/(temp.avgRssi==0?1000.0:abs(temp.avgRssi));//�����п��ܳ���Ϊ0��������Ͱ�����滻�޳���
		dst.push_back(temp);
	}

	//����������
	rawPro<<enterTime<<" "<<selMapDing.size()<<endl; //ʱ�� Mac����
	sort(dst.begin(),dst.end(),[](mapUsed r1,mapUsed r2){return r1.score>r2.score;}); //��󰴴�С���� //��������ʽ�ǿ��Ե����� ��Ϊ�ڷ������Ѿ����ǵ�0�Ĳ���������ں��������ʱ�����ٿ��ǵ�0�����
	for (int it=0;it<dst.size();it++) //������õ���Ϣ���
	{
		rawPro<<dst[it].macName<<" "<<dst[it].avgRssi<<" "<<dst[it].num<<" "<<dst[it].score<<endl;
	}
}

//���µ�wifi����������-------------��Ҫ��������򣬲���֮ǰ�Ĵ洢�ļ�����ɾ������ת���ͷ��ͷֿ���������϶ȣ� 
void Wifi::wifiProcessedSimplify(vector<mapUsed> &srcMap,string enterTime,string leaveTime, char *idData, char *performPath, transDataStruct *dst) 
{	
	//�����¼�µ�����
	for (auto const& name:selMapDing)
	{
		mapUsed temp;
		temp.macName=name.first;
		temp.num=name.second.size();
		int sumTemp=accumulate(name.second.begin(),name.second.end(),0);
		temp.avgRssi=sumTemp/int(name.second.size());
		temp.score=temp.num/(temp.avgRssi==0?1000.0:abs(temp.avgRssi));//�����п��ܳ���Ϊ0��������Ͱ�����滻�޳���
		srcMap.push_back(temp);
	}

	//���ݷ�����������
	sort(srcMap.begin(),srcMap.end(),[](mapUsed r1,mapUsed r2){return r1.score>r2.score;}); //��󰴴�С���� //��������ʽ�ǿ��Ե����� ��Ϊ�ڷ������Ѿ����ǵ�0�Ĳ���������ں��������ʱ�����ٿ��ǵ�0�����

	//��ȡͼƬ  
	char idImg[38862]={0};
	ifstream imgf(transPath(string(idData+292)),ios::binary);//����Ǵ����֤�ļ�·����ֱ�Ӹ��Ƶ�
	imgf.read(idImg,sizeof(idImg));
	imgf.close();

	//�������
	becomeTransDataStruct(dst,idImg,idData,performPath,inet_ntoa(clientAddr.sin_addr),(char *)enterTime.c_str(),(char *)leaveTime.c_str(),srcMap);
}

//����¼�Ľṹ��������������
void Wifi::reSelMacRssi()
{
	selMapDing.erase(selMapDing.begin(),selMapDing.end()); //�������
}