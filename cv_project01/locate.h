#pragma once
#include<opencv2/opencv.hpp>
//#include <opencv2/highgui/highgui.hpp>
//#include <opencv2/imgproc/imgproc.hpp>
using namespace cv;
//�ұ�
void QF_findLine(Mat src);
void myLocate(Mat src0, Mat dst);//ֱ���ݶ��ұ�


void fitLineRansac(const std::vector<cv::Point2f>& points,
	cv::Vec4f& line, int iterations, double sigma,
	double k_min, double k_max);
//RANSAC ���2D ֱ��
//���������points--����㼯
//        iterations--��������
//        sigma--���ݺ�ģ��֮��ɽ��ܵĲ�ֵ,���������ؿ��һ��Ϊ10����
//              ��Parameter use to compute the fitting score��
//        k_min/k_max--��ϵ�ֱ��б�ʵ�ȡֵ��Χ.
//                     ���ǵ����ҳ�������ͼ���е�б��λ��һ����Χ�ڣ�
//                      ��Ӵ˲�����ͬʱ���Ա����ⴹ�ߺ�ˮƽ��
//�������:line--��ϵ�ֱ�߲���,(vx, vy, x0, y0) 
// (vx, vy) ֱ�߷���,(x0, y0)ֱ�ߵ㼯

//�Ľ���canny��Եʶ������
void cannyEdgeDetection(cv::Mat img, cv::Mat& result, int guaSize, 
	double hightThres, double lowThres);
