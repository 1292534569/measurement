#include"locate.h"
#include<vector>
#include"imageBaseOP.h"

using namespace std;
void QF_findLine(Mat src)
{
	//ת��Ϊ�Ҷ�ͼ
	Mat src_gray;
	Mat dst;
	cvtColor(src, src_gray, COLOR_BGR2GRAY);
	//�ָ�ͼ��,ƽ��ȡͼ��������1/10��Ϊ�㼯
	int height = src_gray.rows;
	int width = src_gray.cols;
	int rowSet_size = height / 10;//�˴������㼯��С

	Mat kernel = (Mat_<int>(1, 3) << 1, 0, -1);//�ߵķ���:��������
	Mat convl;//���������
	//������ ��convl���Ϊ��srcһ����С�ľ���
	Mat add = Mat::zeros(rowSet_size - 1, width, CV_8UC1);
	//����ÿʮ����һ�ξ��
	for (int i = 0; i < height; i += rowSet_size)
	{
		Mat tmp_row = src_gray.row(i);//ȡһ��
		Mat tmp_con;//����һ�еľ�����
		filter2D(tmp_row, tmp_con, src_gray.depth(), kernel);
		//��һ�еľ������ӵ��ܵ���ȥ
		vconcat(convl, tmp_con, convl);
		vconcat(convl, add, convl);//���
	}
	//�����������10�ı����Ļ�����༸��,����Ҫɾ����
	//���Ƕ����Ԫ�ض�����,��Ӱ��������Ͻ��,�ȿ����в���
	while (convl.rows != height)
	{

	}
	vector<Point2f> line_pix;//����һ�������ݶȼ�ֵ�������
	for (int i = 0; i < height; i++)//����dst����ֵ�㱣����line_pix��
	{
		for (int j = 0; j < width; j++)
		{
			if (dst.at<uchar>(i, j) == 255)
			{
				Point pt = Point(j, i);
				line_pix.push_back(pt);
			}
		}
	}
	Vec4f line_para;
	fitLineRansac(line_pix, line_para, 1000, 1., -7., 7.);
	//��ȡ��бʽ�ĵ��б��
	cv::Point point0;
	point0.x = line_para[2];
	point0.y = line_para[3];

	double k = line_para[1] / line_para[0];

	//����ֱ�ߵĶ˵�(y = k(x - x0) + y0)
	cv::Point point1, point2;
	point1.x = 0;
	point1.y = k * (0 - point0.x) + point0.y;
	point2.x = 640;
	point2.y = k * (640 - point0.x) + point0.y;

	cv::line(src, point1, point2, cv::Scalar(0, 255, 0), 20);

}
//Ransac���
void fitLineRansac(const std::vector<cv::Point2f>& points,
	cv::Vec4f& line,
	int iterations = 1000,
	double sigma = 1.,
	double k_min = -7.,
	double k_max = 7.)
{
	unsigned int n = points.size();

	if (n < 2)
	{
		return;
	}

	cv::RNG rng;
	double bestScore = -1.;
	for (int k = 0; k < iterations; k++)
	{
		int i1 = 0, i2 = 0;
		while (i1 == i2)
		{
			i1 = rng(n);
			i2 = rng(n);
		}
		const cv::Point2f& p1 = points[i1];
		const cv::Point2f& p2 = points[i2];

		cv::Point2f dp = p2 - p1;//ֱ�ߵķ�������
		dp *= 1. / norm(dp);
		double score = 0;

		if (dp.y / dp.x <= k_max && dp.y / dp.x >= k_min)
		{
			for (int i = 0; i < n; i++)
			{
				cv::Point2f v = points[i] - p1;
				double d = v.y * dp.x - v.x * dp.y;//����a��b���/����b����.||b||=1./norm(dp)
				//score += exp(-0.5*d*d/(sigma*sigma));//���巽ʽ��һ��
				if (fabs(d) < sigma)
					score += 1;
			}
		}
		if (score > bestScore)
		{
			line = cv::Vec4f(dp.x, dp.y, p1.x, p1.y);
			bestScore = score;
		}
	}
}

//ֱ���ݶ��ұ�
void myLocate(Mat src0, Mat dst)
{
	
	Mat src = src0.clone();
	cvtColor(src, src, COLOR_BGR2GRAY);
	//����sobel����
	Mat kernel = (Mat_<int>(3, 3) << -1, 0, 1, -2, 0, 2, -3, 0, 3);
	//���
	filter2D(src, dst, src.depth(), kernel);
	int row = dst.rows;
	int col = dst.cols;
	//��dst���ж�ֵ�� ����100��ȫ����ֵΪ255,�����Ϊ0
	//threshold(dst, dst, 100, 255, THRESH_BINARY);
	//vector<Point> line_pix;//����һ�������ݶȼ�ֵ�������
	//for (int i = 0; i < row; i++)//����dst����ֵ�㱣����line_pix��
	//{
	//	for (int j = 0; j < col; j++)
	//	{
	//		if (dst.at<uchar>(i, j) == 255)
	//		{
	//			Point pt = Point(j, i);
	//			line_pix.push_back(pt);
	//		}
	//	}
	//}
	////�ڲ�ɫͼ���ϻ��� �����ʱ��ֻ���ûҶ�ͼ,�ò�ɫͼ�����bug
	////���Ƶ�ʱ������ǻҶ�ͼ��������
	//Mat src_bgr = src.clone();
	//cvtColor(src_bgr, src_bgr, COLOR_GRAY2BGR);
	////����line_pix������ÿ�������ص��Ի��Ʊ�Ե
	//for (int i = 0; i < line_pix.size() - 1; i++)
	//{
	//	//cout << line_pix[i] << " ";
	//	line(src_bgr, line_pix[i], line_pix[i + 1], Scalar(0, 0, 255), 1);
	//}
	////Rect rec(100, 100, 800, 500);
	////rectangle(src_bgr, rec, Scalar(0, 0, 255), 30);
	//QF_ShowImage(src_bgr, "01", 0);
}

//�Ľ���canny��Եʶ������
void my_canny(Mat src, Mat dst)
{
	cvtColor(src, src, COLOR_BGR2GRAY);
	
}