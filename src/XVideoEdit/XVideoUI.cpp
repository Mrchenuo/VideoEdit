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
	setWindowFlags(Qt::FramelessWindowHint);//���ر�����

	qRegisterMetaType<cv::Mat>("cv::Mat");

	//ԭ��Ƶ��ʾ�źŲ�
	QObject::connect(XVideoThread::Get(),
		SIGNAL(ViewImage1(cv::Mat)),
		ui.src1,
		SLOT(SetImage(cv::Mat))
	);

	//������Ƶ��ʾ�źŲ�
	QObject::connect(XVideoThread::Get(),
		SIGNAL(ViewDes(cv::Mat)),
		ui.des,
		SLOT(SetImage(cv::Mat))
	);

	//������Ƶ����
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
	QString name= QFileDialog::getOpenFileName(this, QString::fromLocal8Bit("��ѡ����Ƶ�ļ�"));
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

	//������Ƶ�ߴ�
	//double w = ui.width->value();
	//double h = ui.height->value();
	if (ui.width->value() > 0 && ui.height->value() > 0)
	{
		XFilter::Get()->Add(XTask{ XTASK_RESIZE,
			{(double)ui.width->value(),(double)ui.height->value()}
		});
	}

	//�ԱȶȺ�����
	if (ui.bright->value() > 0 ||
		ui.contrast->value() > 1)
	{
		XFilter::Get()->Add(XTask{ XTASK_GAIN,
			{(double)ui.bright->value(),ui.contrast->value()} 
		});
	}

	//ͼ����ת	1 90	2 180	3 270
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

	//ͼ����	1 ���¾���	2 ���Ҿ���	3 �������Ҿ���
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
		//ֹͣ����
		XVideoThread::Get()->StopSave();
		isExport = false;
		ui.exportButton->setText(QString::fromLocal8Bit("����"));
		return;
	}
	QString name = QFileDialog::getSaveFileName(this, "save", "out1.avi");
	if (name.isEmpty())
		return;
	std::string filename = name.toLocal8Bit().data();

	if (XVideoThread::Get()->StartSave(filename, ui.width->value(), ui.height->value()))
	{
		isExport = true;
		ui.exportButton->setText(QString::fromLocal8Bit("ֹͣ"));
	}
}

void XVideoUI::ExportEnd()
{
	isExport = false;
	ui.exportButton->setText(QString::fromLocal8Bit("����"));
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
