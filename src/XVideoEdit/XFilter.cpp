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
			//亮度对比度调整
			case XTASK_GAIN:				
				p.Gain(tasks[i].para[0], tasks[i].para[1]);
				break;

			//旋转
			case XTASK_ROTATE90:
				p.Rotate90();
				break;
			case XTASK_ROTATE180:
				p.Rotate180();
				break;
			case XTASK_ROTATE270:
				p.Rotate270();
				break;

			//镜像
			case XTASK_FLIPX:
				p.FlipX();
				break;
			case XTASK_FLIPY:
				p.FlipY();
				break;
			case XTASK_FLIPXY:
				p.FlipXY();
				break;

			//尺寸调整
			case XTASK_RESIZE:
				p.Resize(tasks[i].para[0],tasks[i].para[1]);
				break;

			//图像金字塔
			case XTASK_PYDOWN:
				p.PyDown(tasks[i].para[0]);
				break;
			case XTASK_PYUP:
				p.PyUp(tasks[i].para[0]);
				break;

			//画面裁剪
			case XTASK_CLIP:
				p.Clip(tasks[i].para[0], tasks[i].para[1], tasks[i].para[2], tasks[i].para[3]);
				break;

			//灰度
			case XTASK_GRAY:
				p.Gray();
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
