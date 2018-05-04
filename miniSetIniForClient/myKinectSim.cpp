#include "myKinectSim.h"

KinectSim::KinectSim()
{

	pDepthFrame = nullptr;
	depthMat16.create(DepthHeight, DepthWidth, CV_16UC1);
	depthMat8.create(DepthHeight, DepthWidth, CV_8UC1 );

}

KinectSim::~KinectSim()
{
    SafeRelease1( pDepthSource );
    SafeRelease1( pDepthReader );
    SafeRelease1( pDepthDescription );
	if ( pSensor )
	{
		pSensor->Close();
	}
    SafeRelease1( pSensor );
	cv::destroyAllWindows();
}

HRESULT KinectSim::InitKinect()
{
    hResult = GetDefaultKinectSensor( &pSensor );
	if ( FAILED( hResult ) )
	{
		cerr <<"Error:GetDefaultKinectSensor" <<endl;
		return -1;
	}
	hResult = pSensor->Open();
	if ( FAILED( hResult ) )
	{
		cerr <<"Error:IKinectSensor::Open()" <<endl;
		return -1;
	}
}

HRESULT KinectSim::InitDepth()//������InitKinect()
{
	hResult = pSensor->get_DepthFrameSource( &pDepthSource );      
	if ( FAILED( hResult ) )
	{
		cerr<<"Error:IKinectSensor::get_DepthFrameSource()"<<endl;
		return -1;
	}
	hResult = pDepthSource->OpenReader( &pDepthReader );           
	if ( FAILED( hResult ) )
	{
		cerr <<"Error:IDepthFrameSource::OpenReader()"<<endl;
		return -1;
	}
}

void KinectSim::depthProcess2()
{
	UINT16 *pBuffer = NULL;
	UINT nBufferSize = 0; 
	hResult = pDepthReader->AcquireLatestFrame(&pDepthFrame );
	if ( SUCCEEDED(hResult) )
	{
		hResult = pDepthFrame->AccessUnderlyingBuffer(&nBufferSize, &pBuffer);
		for(int i=0;i<DepthHeight;i++)
		{
			ushort* ptrDepth16 = depthMat16.ptr<ushort>(i);  // 16
			uchar* ptrDepth8 = depthMat8.ptr<uchar>(i);  //8

			for(int j=0;j<DepthWidth;j++)
			{
				USHORT depth = *pBuffer;  //16   �������ֽ�Ϊ��λ���ж�ȡ
				ptrDepth16[j] = depth;        //ֱ�Ӵ洢��ʾ
				ptrDepth8[j] = depth%256; //תΪ8λ�洢��ʾ
				pBuffer++;               //16   �������ֽ�Ϊ��λ���ж�ȡ
			}
		}
	}
    SafeRelease1( pDepthFrame );  //���frameһ��Ҫ�ͷţ���Ȼ�޷�����
}

//�ж�һ���ض���������ֵ����ѡ���ǲ���Ҫִ�������ͼ�Ͳ�λ�ķָ�Ĺ�����
short KinectSim::detPeopleDepth3(Point tl,Point br)//ͨ���򵥵���ֵ�˲����ж����
{
    return medianOut(depthMat16,tl,br); //����˲���Ľ��
}

//������ѡһ����ֵ���˲�����
short KinectSim::medianOut(Mat&src,Point &tl,Point &br)//��16λ���ͼ������˲�
{
    vector<short> tempVec;
    for(int i=tl.y;i<br.y;i++)//�ȰѾ����з����Ԫ�ش洢����
    {
        short* data= src.ptr<short>(i);
        for(int j=tl.x;j<br.x;j++){
            short a=data[j];
            if (a!=0)
            {
                tempVec.push_back(a);
            }
        }
    }
    sort(tempVec.begin(),tempVec.end()); //������
    if (tempVec.size()==0)
    {
        return 0;
    }
    return tempVec[tempVec.size()/2]; // �����
}
