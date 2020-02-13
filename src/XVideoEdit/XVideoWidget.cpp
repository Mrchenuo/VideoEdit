#include "XVideoWidget.h"
#include <QPainter>
#include <opencv2/imgproc.hpp>
using namespace cv;

XVideoWidget::XVideoWidget(QWidget *p):QOpenGLWidget(p)
{
}

void XVideoWidget::paintEvent(QPaintEvent * e)
{
	QPainter p;
	p.begin(this);
	p.drawImage(QPoint(0, 0), img);
	p.end();
}


XVideoWidget::~XVideoWidget()
{
}

void XVideoWidget::Mat2QImage(cv::Mat & mat)
{
	Mat rgb;
	if (mat.channels() == 3)
	{
		cvtColor(mat, rgb, CV_BGR2RGB);
		img = QImage((const unsigned char*)(rgb.data),
			rgb.cols, rgb.rows,
			rgb.cols*rgb.channels(),
			QImage::Format_RGB888);
	}
	else
	{
		img = QImage((const unsigned char*)(mat.data),
			mat.cols, mat.rows,
			mat.cols*mat.channels(),
			QImage::Format_RGB888);
	}
}

void XVideoWidget::SetImage(cv::Mat mat)
{
	//��������ĳߴ�����4����ģ�����ͼ������
	QImage::Format fmt = QImage::Format_RGB888;
	int pixSize = 3;

	//Ϊ�Ҷ�ͼʱ
	if (mat.type() == CV_8UC1)
	{
		fmt = QImage::Format_Grayscale8;
		pixSize = 1;
	}

	if (img.isNull() ||img.format() != fmt)
	{
		delete img.bits();
		uchar *buf = new uchar[width()*height() * pixSize];
		img = QImage(buf, width(), height(), fmt);
	}
	Mat des;
	cv::resize(mat, des, Size(img.size().width(), img.size().height()));

	//����ͼ����ɫ��ʽ����Ϊbgrͼʱ��ת�����Ҷ�ͼʱֱ��ʹ�ã�
	if(pixSize>1)
		cv::cvtColor(des, des, COLOR_BGR2RGB);

	//�����ڴ�ռ�
	memcpy(img.bits(), des.data, des.cols*des.rows*des.elemSize());

	//Mat2QImage(mat);
	
	update();
}
