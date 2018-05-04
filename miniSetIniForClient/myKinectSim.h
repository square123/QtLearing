#ifndef MYKINECT_H
#define MYKINECT_H

#include <opencv2/opencv.hpp>

#include <kinect.h>
#include <vector>
#include <algorithm>


using namespace cv;
using namespace std;

template<class Interface> 
inline void SafeRelease1( Interface *& pInterfaceToRelease )
{
	if ( pInterfaceToRelease != NULL )
	{
		pInterfaceToRelease->Release();
		pInterfaceToRelease = NULL;
	}
}

class KinectSim//only for get depth window so simplify the Kinect class
{
public://存储一些程序可以使用的量
	static const int DepthWidth = 512;
	static const int DepthHeight = 424;
	static const int ColorWidth = 1920;
	static const int ColorHeight = 1080;
	
	Mat depthMat8;
	Mat depthMat16;

	HRESULT hResult ;
	HRESULT InitKinect();
	HRESULT InitDepth();

    void depthProcess2();
    short detPeopleDepth3(Point tl,Point br);//对16位深度图像进行滤波来判断情况
    short medianOut(Mat&src,Point &tl,Point &br);//中值

    KinectSim();
    ~KinectSim();

private://存储一些实现的过程量
	IKinectSensor* pSensor ;

	IDepthFrameSource* pDepthSource;
	IDepthFrameReader* pDepthReader;
	IFrameDescription* pDepthDescription;
	IDepthFrame* pDepthFrame;
};
#endif
