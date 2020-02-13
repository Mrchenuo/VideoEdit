#include "XVideoUI.h"
#include <QFileDialog>
#include <QMessageBox>
#include <string>

#include "XVideoThread.h"
#include "XFilter.h"

using namespace std;
static bool pressSlider = false;
static bool isExport = false;
static bool isColor = true;//是否为彩色图

XVideoUI::XVideoUI(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	setWindowFlags(Qt::FramelessWindowHint);//隐藏标题栏

	qRegisterMetaType<cv::Mat>("cv::Mat");

	//原视频显示信号槽
	QObject::connect(XVideoThread::Get(),
		SIGNAL(ViewImage1(cv::Mat)),
		ui.src1,
		SLOT(SetImage(cv::Mat))
	);

	//生成视频显示信号槽
	QObject::connect(XVideoThread::Get(),
		SIGNAL(ViewDes(cv::Mat)),
		ui.des,
		SLOT(SetImage(cv::Mat))
	);

	//导出视频结束
	QObject::connect(XVideoThread::Get(),
		SIGNAL(SaveEnd()),
		this,
		SLOT(ExportEnd())
		);

	Pause();
	startTimer(40);
}

void XVideoUI::Open()
{
	QString name= QFileDialog::getOpenFileName(this, QString::fromLocal8Bit("请选择视频文件"));
	if (name.isEmpty()) 
		return;
	string file = name.toLocal8Bit();
	if (!XVideoThread::Get()->Open(file))
	{
		QMessageBox::information(this, "error", name + " open failed");
	}

	Play();

}

void XVideoUI::timerEvent(QTimerEvent * e)
{
	if (pressSlider) return;
	double pos=XVideoThread::Get()->GetPos();
	ui.playSlider->setValue(pos * 1000);
}

void XVideoUI::SliderPress()
{
	pressSlider = true;
}

void XVideoUI::SliderRelease()
{
	pressSlider = false;
}

void XVideoUI::SetPos(int pos)
{
	XVideoThread::Get()->Seek((double)pos/1000.0);
}

void XVideoUI::Set()
{
	XFilter::Get()->Clear();
	isColor = true;
	//视频图像裁剪
	bool isClip = false;//一旦做了裁剪，尺寸调整就不能做了
	double cx = ui.cx->value();
	double cy = ui.cy->value();
	double cw = ui.cw->value();
	double ch = ui.ch->value();
	if (cx + cy + cw + ch > 0.0001)
	{
		isClip = true;
		XFilter::Get()->Add(XTask{ XTASK_CLIP,{cx,cy,cw,ch} });

		//由于裁剪后尺寸需要调整
		XFilter::Get()->Add(XTask{ XTASK_RESIZE,
			{(double)XVideoThread::Get()->width,(double)XVideoThread::Get()->height}
			});
	}

	//图像金字塔
	bool isPy = false;
	int down = ui.pydown->value();
	int up= ui.pyup->value();

	if (up > 0)
	{
		isPy = true;
		XFilter::Get()->Add(XTask{ XTASK_PYUP,{(double)up} });
		int w = XVideoThread::Get()->width;
		int h = XVideoThread::Get()->height;
		for (int i = 0; i < up; i++)
		{
			w = w * 2;
			h = h * 2;
		}
		ui.width->setValue(w);
		ui.height->setValue(h);
	}

	if (down > 0)
	{
		isPy = true;
		XFilter::Get()->Add(XTask{ XTASK_PYDOWN,{(double)down}});
		int w = XVideoThread::Get()->width;
		int h = XVideoThread::Get()->height;
		for (int i = 0; i < down; i++)
		{
			w = w / 2;
			h = h / 2;
		}
		ui.width->setValue(w);
		ui.height->setValue(h);
	}

	

	//调整视频尺寸
	if (!isClip && !isPy &&ui.width->value() > 0 && ui.height->value() > 0)
	{
		XFilter::Get()->Add(XTask{ XTASK_RESIZE,
			{(double)ui.width->value(),(double)ui.height->value()}
		});
	}

	//对比度和亮度
	if (ui.bright->value() > 0 ||
		ui.contrast->value() > 1)
	{
		XFilter::Get()->Add(XTask{ XTASK_GAIN,
			{(double)ui.bright->value(),ui.contrast->value()} 
		});
	}

	//灰度图
	if (ui.color->currentIndex() == 1)
	{
		XFilter::Get()->Add(XTask{ XTASK_GRAY });
		isColor = false;
	}

	//图像旋转	1 90	2 180	3 270
	if (ui.rotate->currentIndex() == 1)
	{
		XFilter::Get()->Add(XTask{ XTASK_ROTATE90 });
	}
	else if (ui.rotate->currentIndex() == 2)
	{
		XFilter::Get()->Add(XTask{ XTASK_ROTATE180 });
	}
	else if (ui.rotate->currentIndex() == 3)
	{
		XFilter::Get()->Add(XTask{ XTASK_ROTATE270 });
	}

	//图像镜像	1 上下镜像	2 左右镜像	3 上下左右镜像
	if (ui.flip->currentIndex() == 1)
	{
		XFilter::Get()->Add(XTask{ XTASK_FLIPX });
	}
	else if (ui.flip->currentIndex() == 2)
	{
		XFilter::Get()->Add(XTask{ XTASK_FLIPY });
	}
	else if (ui.flip->currentIndex() == 3)
	{
		XFilter::Get()->Add(XTask{ XTASK_FLIPXY });
	}


}

void XVideoUI::Export()
{
	
	if (isExport)
	{
		//停止导出
		XVideoThread::Get()->StopSave();
		isExport = false;
		ui.exportButton->setText(QString::fromLocal8Bit("导出"));
		return;
	}
	QString name = QFileDialog::getSaveFileName(this, "save", "out1.avi");
	if (name.isEmpty())
		return;
	std::string filename = name.toLocal8Bit().data();

	if (XVideoThread::Get()->StartSave(filename, ui.width->value(), ui.height->value(),isColor))
	{
		isExport = true;
		ui.exportButton->setText(QString::fromLocal8Bit("停止"));
	}
}

void XVideoUI::ExportEnd()
{
	isExport = false;
	ui.exportButton->setText(QString::fromLocal8Bit("导出"));
}

void XVideoUI::Play()
{
	ui.pauseButton->show();
	ui.pauseButton->setGeometry(ui.playButton->geometry());
	XVideoThread::Get()->Play();
	ui.playButton->hide();
}

void XVideoUI::Pause()
{
	ui.playButton->show();
	//ui.pauseButton->setGeometry(ui.playButton->geometry());
	XVideoThread::Get()->Pause();
	ui.pauseButton->hide();
}
