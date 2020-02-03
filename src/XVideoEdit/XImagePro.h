#pragma once
#include <opencv2/core.hpp>
class XImagePro
{
public:
	//����ԭͼ��������֮ǰ�Ĵ�����
	void Set(cv::Mat mat1, cv::Mat mat2);

	//��ȡ�������
	cv::Mat Get() { return des; }

	//�������ȺͶԱȶ�
	//@para bright double ���� 0~100
	//@para contrast int �Աȶ� 1.0~3.0
	void Gain(double bright, double contrast);

	//ͼ����ת
	void Rotate90();
	void Rotate180();
	void Rotate270();

	//���¾���
	void FlipX();
	//���Ҿ���
	void FlipY();
	//�������Ҿ���
	void FlipXY();

	//ͼ��ߴ�
	void Resize(int width, int height);

	//ͼ�������
	void PyDown(int count);
	void PyUp(int count);

	XImagePro();
	~XImagePro();
private:
	//ԭͼ
	cv::Mat src1, src2;
	
	//Ŀ��ͼ
	cv::Mat des;
};

