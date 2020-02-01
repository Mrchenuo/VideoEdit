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

	XImagePro();
	~XImagePro();
private:
	//ԭͼ
	cv::Mat src1, src2;
	
	//Ŀ��ͼ
	cv::Mat des;
};

