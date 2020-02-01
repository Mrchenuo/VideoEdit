#include "XVideoUI.h"
#include <QFileDialog>
#include <QMessageBox>
#include <string>

#include "XVideoThread.h"
#include "XFilter.h"

using namespace std;
static bool pressSlider = false;
static bool isExport = false;
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

}

void XVideoUI::timerEvent(QTimerEvent * e)
{
	if (pressSlider) return;
	double pos=XVideoThread::Get()->getPos();
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
	//对比度和亮度
	if (ui.bright->value() > 0 ||
		ui.contrast->value() > 1)
	{
		XFilter::Get()->Add(XTask{ XTASK_GAIN,
			{(double)ui.bright->value(),ui.contrast->value()} 
			});
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
	if (XVideoThread::Get()->StartSave(filename))
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
