#include "XImagePro.h"
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace cv;

void XImagePro::Set(cv::Mat mat1, cv::Mat mat2)
{
	if (mat1.empty())
		return;

	this->src1 = mat1;
	this->src2 = mat2;
	this->src1.copyTo(des);
}

void XImagePro::Gain(double bright, double contrast)
{
	if (des.empty())
		return;
	des.convertTo(des, -1, contrast, bright);
}

void XImagePro::Rotate90()
{
	if (des.empty())
		return;
	rotate(des, des, ROTATE_90_CLOCKWISE);
}

void XImagePro::Rotate180()
{
	if (des.empty())
		return;
	rotate(des, des, ROTATE_180);
}

void XImagePro::Rotate270()
{
	if (des.empty())
		return;
	rotate(des, des, ROTATE_90_COUNTERCLOCKWISE);
}

//上下镜像
void XImagePro::FlipX()
{
	if (des.empty())
		return;
	flip(des, des, 0);
}

//左右镜像
void XImagePro::FlipY()
{
	if (des.empty())
		return;
	flip(des, des, 1);
}

//上下左右镜像
void XImagePro::FlipXY()
{
	if (des.empty())
		return;
	flip(des, des, -1);
}

void XImagePro::Resize(int width, int height)
{
	if (des.empty())
		return;
	resize(des, des, Size(width, height));
}

void XImagePro::PyDown(int count)
{
	if (des.empty())
		return;
	for (int i = 0; i < count; i++)
	{
		pyrDown(des, des);
	}
}

void XImagePro::PyUp(int count)
{
	if (des.empty())
		return;
	for (int i = 0; i < count; i++)
	{
		pyrUp(des, des);
	}
}

void XImagePro::Clip(int x, int y, int w, int h)
{
	if (des.empty())
		return;

	if (x < 0 || y < 0 || w <= 0 || h <= 0)
		return;

	if (x > des.cols || y > des.rows)
		return;
	des = des(Rect(x, y, w, h));
}

void XImagePro::Gray()
{
	if (des.empty())
		return;
	cvtColor(des, des, COLOR_BGR2GRAY);
}

void XImagePro::Mark(int x, int y, double alpha)
{
	if (des.empty())
		return;
	if (src2.empty())
		return;

	Mat roi = des(Rect(x, y, src2.cols, src2.rows));
	addWeighted(src2, alpha, roi, 1 - alpha, 0, roi);//灰度图像和水印会有冲突，因为要求两个图像是同种类型
}

XImagePro::XImagePro()
{
}


XImagePro::~XImagePro()
{
}
