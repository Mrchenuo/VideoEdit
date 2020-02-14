#include "XVideoThread.h"
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <XFilter.h>

using namespace cv;
using namespace std;

//һ����ƵԴ
static VideoCapture cap1;

//������Ƶ
static VideoWriter vw;

static bool isExit = false;//��ֹXVideoThread�����Ѿ��������ˣ���run����̻߳���ִ�У�����
//��һ����ƵԴ�ļ�
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
	cout << "��ʼ����" << endl;
	Seek(0);//��ͷ����
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
	cout << "ֹͣ����" << endl;
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
	//����run����������������Open��Seek�ȿ��ܲ���ͬһ�߳��У�������Ҫ����mutex��ʵ�ֻ���
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

		//��ȡһ֡��Ƶ����������ɫת��
		if (!cap1.read(mat1) || mat1.empty())
		{
			mutex.unlock();
			//��������βλ�ã�ֹͣ����
			if (isWrite)
			{
				StopSave();
				SaveEnd();
			}
			msleep(5);
			continue;
		}
		//��ʾͼ��1
		if (!isWrite)//�ڵ���ʱ����ʾ����ֹ�������죬��ջ���
			ViewImage1(mat1);

		Mat mat2 = mark;
		//ͨ��������������Ƶ 
		Mat des = XFilter::Get()->Filter(mat1, mat2);

		//��ʾ����ͼ��
		if (!isWrite)
			ViewDes(des);

		int s = 0;
		s = 1000 / fps;
		if (isWrite)
		{
			s = 1;//ʹ�������ٽ���
			vw.write(des);
		}
		
		msleep(s);
		mutex.unlock();
	}
}
