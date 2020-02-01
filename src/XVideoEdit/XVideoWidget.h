#pragma once
#include <QOpenGLWidget>
#include <opencv2/core.hpp>
#include <QImage>
class XVideoWidget :
	public QOpenGLWidget
{
	Q_OBJECT

public:
	XVideoWidget(QWidget *p);
	void paintEvent(QPaintEvent *e);
	virtual ~XVideoWidget();
public slots:
	void SetImage(cv::Mat mat);
private:
	void Mat2QImage(cv::Mat& mat);//Œ¥ π”√
protected:
	QImage img;
};

