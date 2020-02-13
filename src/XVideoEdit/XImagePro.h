#pragma once
#include <opencv2/core.hpp>
class XImagePro
{
public:
	//设置原图，会清理之前的处理结果
	void Set(cv::Mat mat1, cv::Mat mat2);

	//获取处理后结果
	cv::Mat Get() { return des; }

	//设置亮度和对比度
	//@para bright double 亮度 0~100
	//@para contrast int 对比度 1.0~3.0
	void Gain(double bright, double contrast);

	//图像旋转
	void Rotate90();
	void Rotate180();
	void Rotate270();

	//上下镜像
	void FlipX();
	//左右镜像
	void FlipY();
	//上下左右镜像
	void FlipXY();

	//图像尺寸
	void Resize(int width, int height);

	//图像金字塔
	void PyDown(int count);
	void PyUp(int count);

	//视频画面裁剪
	void Clip(int x, int y, int w, int h);

	//转为灰度图
	void Gray();

	XImagePro();
	~XImagePro();
private:
	//原图
	cv::Mat src1, src2;
	
	//目标图
	cv::Mat des;
};

