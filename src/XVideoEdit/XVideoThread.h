#pragma once
#include <QThread>
#include <QMutex>
#include "opencv2/core.hpp"

class XVideoThread : public QThread
{

	Q_OBJECT

public:
	int fps = 0;

	static XVideoThread* Get()
	{
		static XVideoThread vt;
		return &vt;
	}
	//��һ����ƵԴ�ļ�
	bool Open(const std::string file);
	
	//���ص�ǰ���ŵ�λ��
	double getPos();

	//��ת��Ƶ
	///@para frame int ֡λ��
	bool Seek(int frame);
	bool Seek(double pos);
	
	//��ʼ������Ƶ
	bool StartSave(const std::string filename, int width=0, int height=0);	

	//ֹͣ������Ƶ��д����Ƶ֡������
	void StopSave();

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
};

