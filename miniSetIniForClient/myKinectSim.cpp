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

HRESULT KinectSim::InitDepth()//必须先InitKinect()
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
				USHORT depth = *pBuffer;  //16   以两个字节为单位进行读取
				ptrDepth16[j] = depth;        //直接存储显示
				ptrDepth8[j] = depth%256; //转为8位存储显示
				pBuffer++;               //16   以两个字节为单位进行读取
			}
		}
	}
    SafeRelease1( pDepthFrame );  //这个frame一定要释放，不然无法更新
}

//判断一个特定区域的深度值，来选择是不是要执行人体存图和部位的分割的工作。
short KinectSim::detPeopleDepth3(Point tl,Point br)//通过简单的中值滤波来判断情况
{
    return medianOut(depthMat16,tl,br); //输出滤波后的结果
}

//用于挑选一个中值的滤波函数
short KinectSim::medianOut(Mat&src,Point &tl,Point &br)//对16位深度图像进行滤波
{
    vector<short> tempVec;
    for(int i=tl.y;i<br.y;i++)//先把矩阵中非零的元素存储起来
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
    sort(tempVec.begin(),tempVec.end()); //先排序
    if (tempVec.size()==0)
    {
        return 0;
    }
    return tempVec[tempVec.size()/2]; // 后输出
}
