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
	//��һ����ƵԴ�ļ�
	bool Open(const std::string file);

	//������Ƶ
	void Play() { mutex.lock(); isPlay = true; mutex.unlock();}
	//��ͣ��Ƶ
	void Pause() { mutex.lock(); isPlay = false; mutex.unlock(); }

	//���ص�ǰ���ŵ�λ��
	double GetPos();

	//��ת��Ƶ
	///@para frame int ֡λ��
	bool Seek(int frame);
	bool Seek(double pos);
	
	//��ʼ������Ƶ
	bool StartSave(const std::string filename, int width=0, int height=0,bool isColor=true);	

	//ֹͣ������Ƶ��д����Ƶ֡������
	void StopSave();

	//���ˮӡ
	void SetMark(cv::Mat m) { mutex.lock(); this->mark = m; mutex.unlock(); }

	~XVideoThread();

	//�߳���ں���
	void run();
signals:
	//��ʾԭ��Ƶͼ��1ͼ��
	void ViewImage1(cv::Mat mat);

	//��ʾ���ɺ�ͼ��
	void ViewDes(cv::Mat mat);

	void SaveEnd();
protected:
	XVideoThread();//��֤�ⲿ�޷����ɶ���ȷ�������Ψһ��
	QMutex mutex;
	//�Ƿ�ʼд��Ƶ
	bool isWrite = false;
	//�Ƿ�ʼ������Ƶ
	bool isPlay = false;
	//ˮӡ
	cv::Mat mark;
};

