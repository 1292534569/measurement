#pragma once
#include<opencv2/opencv.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
//�ұ�����find_line.h
// ����
cv::Mat	QF_blur(cv::Mat& src);
//�����ݶ�
cv::Mat QF_grad(cv::Mat& src);
void QF_LocateEdge(cv::Mat& Src /*, &dstV, pstart, pEnd, dir, thresh, flags*/);

class FindLine {
	//����ͼ���ݶ�

};