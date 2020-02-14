#pragma once

#include <QtWidgets/QWidget>
#include "ui_XVideoUI.h"

class XVideoUI : public QWidget
{

	Q_OBJECT

public:
	XVideoUI(QWidget *parent = Q_NULLPTR);
	void timerEvent(QTimerEvent* e);
public slots:
	void Open();
	void SliderPress();
	void SliderRelease();
	//�������϶�
	void SetPos(int);
	//���ù�����
	void Set();
	//������Ƶ
	void Export();
	//��������
	void ExportEnd();
	//����
	void Play();
	//��ͣ
	void Pause();
	//ˮӡ
	void Mark();
private:
	Ui::XVideoUIClass ui;
};
