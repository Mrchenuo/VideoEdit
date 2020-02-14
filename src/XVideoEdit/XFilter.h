#pragma once
#include<opencv2/core.hpp>
#include<vector>

enum XTaskType
{
	XTASK_NONE,
	XTASK_GAIN,				//亮度对比度调整
	XTASK_ROTATE90,			//旋转90度
	XTASK_ROTATE180,		//旋转180度
	XTASK_ROTATE270,		//旋转270度
	XTASK_FLIPX,			//左右镜像
	XTASK_FLIPY,			//上下镜像
	XTASK_FLIPXY,			//上下左右镜像
	XTASK_RESIZE,			//尺寸调整
	XTASK_PYDOWN,			//向下采样(高斯金字塔）
	XTASK_PYUP,				//向上采样（拉普拉斯金字塔）
	XTASK_CLIP,				//画面裁剪
	XTASK_GRAY,				//灰度
	XTASK_MASK,				//水印
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

