#include "XFilter.h"
#include <QThread>
#include <QMutex>
#include "XImagePro.h"
class CXFilter :public XFilter
{
public:
	std::vector<XTask> tasks;
	QMutex mutex;
	cv::Mat Filter(cv::Mat mat1, cv::Mat mat2)
	{
		mutex.lock();
		XImagePro p;
		p.Set(mat1, mat2);
		for (int i = 0; i < tasks.size(); i++)
		{
			switch (tasks[i].type)
			{
			//���ȶԱȶȵ���
			case XTASK_GAIN:				
				p.Gain(tasks[i].para[0], tasks[i].para[1]);
				break;

			//��ת
			case XTASK_ROTATE90:
				p.Rotate90();
				break;
			case XTASK_ROTATE180:
				p.Rotate180();
				break;
			case XTASK_ROTATE270:
				p.Rotate270();
				break;

			//����
			case XTASK_FLIPX:
				p.FlipX();
				break;
			case XTASK_FLIPY:
				p.FlipY();
				break;
			case XTASK_FLIPXY:
				p.FlipXY();
				break;

			//�ߴ����
			case XTASK_RESIZE:
				p.Resize(tasks[i].para[0],tasks[i].para[1]);
				break;

			//ͼ�������
			case XTASK_PYDOWN:
				p.PyDown(tasks[i].para[0]);
				break;
			case XTASK_PYUP:
				p.PyUp(tasks[i].para[0]);
				break;
			default:
				break;
			}
		}
		cv::Mat re = p.Get();
		mutex.unlock();
		return re;
	}
	void Add(XTask task)
	{
		mutex.lock();
		tasks.push_back(task);
		mutex.unlock();
	}
	void Clear()
	{
		mutex.lock();
		tasks.clear();
		mutex.unlock();
	}
};



XFilter::XFilter()
{
}


XFilter * XFilter::Get()
{
	static CXFilter cx;
	return &cx;
}

XFilter::~XFilter()
{
}
