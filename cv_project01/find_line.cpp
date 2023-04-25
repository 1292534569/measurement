#include"find_line.h"
//����
cv::Mat	QF_blur(cv::Mat& src) {
	cv::Mat dst, edge, gray;
	//������srcͬ���ͺʹ�С�ľ���(dst)
	dst.create(src.size(), src.type());

	// ��ԭͼ��ת��Ϊ�Ҷ�ͼ��
	//cvtColor(src, gray, CV_BGR2GRAY);

	//ʹ�� 3x3�ں�������
	//cv::GaussianBlur
	blur(gray, edge, cv::Size(3, 3));
	return edge;

}
//�����ݶ�
cv::Mat QF_grad(cv::Mat& src) {
	cv::Mat grad_x, grad_y;
	cv::Mat abs_grad_x, abs_grad_y, dst;

	//�� X�����ݶ�
	Sobel(src, grad_x, CV_16S, 1, 0, 3, 1, 1);
	convertScaleAbs(grad_x, abs_grad_x);

	//��Y�����ݶ�
	Sobel(src, grad_y, CV_16S, 0, 1, 3, 1, 1);
	convertScaleAbs(grad_y, abs_grad_y);

	//��5���ϲ��ݶ�(����)
	addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, dst);
	return dst;

}
//�ұ�
void QF_LocateEdge(cv::Mat& Src /*, &dstV, pstart, pEnd, dir, thresh, flags*/) {

}
