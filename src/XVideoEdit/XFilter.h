#pragma once
#include<opencv2/core.hpp>
#include<vector>

enum XTaskType
{
	XTASK_NONE,
	XTASK_GAIN,				//���ȶԱȶȵ���
	XTASK_ROTATE90,			//��ת90��
	XTASK_ROTATE180,		//��ת180��
	XTASK_ROTATE270,		//��ת270��
	XTASK_FLIPX,			//���Ҿ���
	XTASK_FLIPY,			//���¾���
	XTASK_FLIPXY,			//�������Ҿ���
	XTASK_RESIZE,			//�ߴ����
	XTASK_PYDOWN,			//���²���(��˹��������
	XTASK_PYUP,				//���ϲ�����������˹��������
	XTASK_CLIP,				//����ü�
	XTASK_GRAY,				//�Ҷ�
};
struct  XTask
{
	XTaskType type;
	std::vector<double> para;
};

class XFilter
{
public:
	virtual cv::Mat Filter(cv::Mat mat1,cv::Mat mat2)=0;
	virtual void Add(XTask task) = 0;
	virtual void Clear() = 0;
	static XFilter *Get();
	virtual ~XFilter();
protected:
	XFilter();
	
};

