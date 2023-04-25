#pragma once
#include<opencv2/opencv.hpp>
#include<iostream>
using namespace cv;
//ͼ����������
cv::Mat add(cv::Mat img1, cv::Mat img2);//��add
cv::Mat sub(cv::Mat img1, cv::Mat img2);//��substract
cv::Mat div(cv::Mat img1, cv::Mat img2);//��divide
cv::Mat mul(cv::Mat img1, cv::Mat img2);//��multiply
//��ȡ
cv::Mat QF_ReadImage(const cv::String& path, int flag = 1);
//����
void QF_WriteImage(const cv::String& filename, const cv::Mat& src);
//��ʾ 
//flag 0��ʾCV_WINDOW_NORMAL�����ɵ��ڴ�С��1��ʾWINDOW_AUTOSIZE���ɵ��ڴ�С
void QF_ShowImage(const cv::Mat& src, const String winName, int flag = 1);
//����
void QF_drawline(const cv::Mat& src, cv::Point pt1, cv::Point pt2,
	const cv::Scalar& color, int thickness);
//������
void QF_DrawRectangle(const cv::Mat& src, cv::Rect rect,
	cv::Scalar& color, int thickness);

////��Բ
void QF_DrawCircle(const cv::Mat& src, cv::Point center,
	int radius, cv::Scalar color, int Thickness);
//��Բ  cv��û����Բ����
void QF_drawEllipse(const cv::Mat& src);
//���Ƽ�ͷ
void QF_DrawArrow(const cv::Mat& src, cv::Point p1, cv::Point p2,
	const cv::Scalar color, int thickness);
//����
void QF_DrawText(cv::Mat& src, // �����Ƶ�ͼ��
	const cv::String& text, // �����Ƶ�����
	cv::Point origin, // �ı�������½�
	int fontFace, // ���� (��cv::FONT_HERSHEY_PLAIN)
	double fontScale, // �ߴ����ӣ�ֵԽ������Խ��
	cv::Scalar color, // ��������ɫ��BGR��
	int thickness);
//����ͼ��
cv::Mat QF_CreateImage(int rows, int cols, int PixelType);
//�������ƶ�ͼ��
cv::Mat QF_CopyImage(cv::Mat& src, int x/*x��ƫ����*/, int y/*y��ƫ����*/);
//����任
void QF_WarpAffine(Mat& src, Mat& dst, Mat& matrix, Size dsize);
//�������� ����һ��2*3�ľ�������warpaffine ��Ҫ����������
cv::Mat QF_GetAffineTransformation(const Point2f* src, const Point2f* dst);
//��͸�ӱ任���� ��Ҫ�����ĸ���
cv::Mat QF_GetPerspectiveTransformation(const Point2f* src, const Point2f* dst);
//͸�ӱ任
void QF_WarpPerspective(Mat& src, Mat& dst, Mat& matrix);
