#pragma once
#include <QThread>
#include <QMutex>
#include "opencv2/core.hpp"

class XVideoThread : public QThread
{

	Q_OBJECT

public:
	int fps = 0;
	int width = 0;
	int height = 0;

	static XVideoThread* Get()
	{
		static XVideoThread vt;
		return &vt;
	}
	//打开一号视频源文件
	bool Open(const std::string file);

	//播放视频
	void Play() { mutex.lock(); isPlay = true; mutex.unlock();}
	//暂停视频
	void Pause() { mutex.lock(); isPlay = false; mutex.unlock(); }

	//返回当前播放的位置
	double GetPos();

	//跳转视频
	///@para frame int 帧位置
	bool Seek(int frame);
	bool Seek(double pos);
	
	//开始保存视频
	bool StartSave(const std::string filename, int width=0, int height=0,bool isColor=true);	

	//停止保存视频，写入视频帧的索引
	void StopSave();

	~XVideoThread();

	//线程入口函数
	void run();
signals:
	//显示原视频图像1图像
	void ViewImage1(cv::Mat mat);

	//显示生成后图像
	void ViewDes(cv::Mat mat);

	void SaveEnd();
protected:
	XVideoThread();//保证外部无法生成对象，确保对象的唯一性
	QMutex mutex;
	//是否开始写视频
	bool isWrite = false;
	//是否开始播放视频
	bool isPlay = false;
};

