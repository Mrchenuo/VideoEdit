#include "XVideoThread.h"
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <XFilter.h>

using namespace cv;
using namespace std;

//一号视频源
static VideoCapture cap1;

//保存视频
static VideoWriter vw;

static bool isExit = false;//防止XVideoThread对象已经被析构了，而run这个线程还在执行，出错
//打开一号视频源文件
bool XVideoThread::Open(const std::string file)
{
	mutex.lock();
	int re = cap1.open(file);
	mutex.unlock();
	cout << re << endl;
	if (!re)
		return re;
	fps = cap1.get(CAP_PROP_FPS);
	width = cap1.get(CAP_PROP_FRAME_WIDTH);
	height= cap1.get(CAP_PROP_FRAME_HEIGHT);

	if (fps <= 0)
		fps = 25;
	return true;
}

XVideoThread::XVideoThread()
{
	start();
}


double XVideoThread::GetPos()
{
	double pos = 0;
	mutex.lock();
	if (!cap1.isOpened())
	{
		mutex.unlock();
		return pos;
	}
	double count=cap1.get(CAP_PROP_FRAME_COUNT);
	double cur = cap1.get(CAP_PROP_POS_FRAMES);
	if(count>0.001)
		pos = cur / count;
	mutex.unlock();
	return pos;
}

bool XVideoThread::Seek(int frame)
{
	mutex.lock();
	if (!cap1.isOpened())
	{
		mutex.unlock();
		return false;
	}
	int re = cap1.set(CAP_PROP_POS_FRAMES, frame);
	mutex.unlock();
	return re;
}

bool XVideoThread::Seek(double pos)
{
	double count = cap1.get(CAP_PROP_FRAME_COUNT);
	int frame = pos * count;
	Seek(frame);
	return false;
}

bool XVideoThread::StartSave(const std::string filename, int width, int height, bool isColor)
{
	cout << "开始导出" << endl;
	Seek(0);//从头导出
	mutex.lock();
	if (!cap1.isOpened())
	{
		mutex.unlock();
		return false;
	}
	if (width <= 0)
		width = cap1.get(CAP_PROP_FRAME_WIDTH);

	if (height <= 0)
		height = cap1.get(CAP_PROP_FRAME_HEIGHT);

	vw.open(filename,
		VideoWriter::fourcc('X', '2', '6', '4'),
		this->fps,
		Size(width, height),
		isColor
	);
	if (!vw.isOpened())
	{
		cout << "start save failed" << endl;
		mutex.unlock();
		return false;
	}
	isWrite = true;
	mutex.unlock();
	return true;
}

void XVideoThread::StopSave()
{
	cout << "停止导出" << endl;
	mutex.lock();
	vw.release();
	isWrite = false;
	mutex.unlock();
}

XVideoThread::~XVideoThread()
{
	mutex.lock();
	isExit = true;
	mutex.unlock();
	wait();
}

void XVideoThread::run()
{
	//由于run函数与其他函数如Open、Seek等可能不在同一线程中，所以需要用锁mutex来实现互斥
	Mat mat1;
	for (;;)
	{
		mutex.lock();

		if (isExit)
		{
			mutex.unlock();
			break;
		}
			
		if (!cap1.isOpened())
		{
			mutex.unlock();
			msleep(5);
			continue;
		}

		if (!isPlay)
		{
			mutex.unlock();
			msleep(5);
			continue;
		}

		//读取一帧视频，并进行颜色转换
		if (!cap1.read(mat1) || mat1.empty())
		{
			mutex.unlock();
			//导出到结尾位置，停止导出
			if (isWrite)
			{
				StopSave();
				SaveEnd();
			}
			msleep(5);
			continue;
		}
		//显示图像1
		if (!isWrite)//在导出时不显示，防止导出过快，堆栈溢出
			ViewImage1(mat1);

		//通过过滤器处理视频 
		Mat des = XFilter::Get()->Filter(mat1, Mat());

		//显示生成图像
		if (!isWrite)
			ViewDes(des);

		int s = 0;
		s = 1000 / fps;
		if (isWrite)
		{
			s = 1;//使导出快速结束
			vw.write(des);
		}
		
		msleep(s);
		mutex.unlock();
	}
}
