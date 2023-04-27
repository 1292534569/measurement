#include"locate.h"
#include<vector>
#include"imageBaseOP.h"
#define pi 3.14159

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
void cannyEdgeDetection(Mat img, Mat& result,int guaSize, 
    double hightThres, double lowThres) 
{
    // ��˹�˲�->��ֵ�˲�
    Rect rect; // IOU����
    cv::Mat filterImg = cv::Mat::zeros(img.rows, img.cols, CV_64FC1);
    img.convertTo(img, CV_64FC1);
    result = cv::Mat::zeros(img.rows, img.cols, CV_64FC1);
    int guassCenter = guaSize / 2; // ��˹�˵����� // (2* guassKernelSize +1) * (2*guassKernelSize+1)��˹�˴�С
    double sigma = 1;   // �����С
    cv::Mat guassKernel = cv::Mat::zeros(guaSize, guaSize, CV_64FC1);
    for (int i = 0; i < guaSize; i++) {
        for (int j = 0; j < guaSize; j++) {
            guassKernel.at<double>(i, j) = (1.0 / (2.0 * pi * sigma * sigma)) *
                (double)exp(-(((double)pow((i - (guassCenter + 1)), 2) + (double)pow((j - (guassCenter + 1)), 2)) / (2.0 * sigma * sigma)));
            // std::cout<<guassKernel.at<double>(i, j) << " ";
        }
        // std::cout<<std::endl;
    }
    cv::Scalar sumValueScalar = cv::sum(guassKernel);
    double sum = sumValueScalar.val[0];
    std::cout << sum << std::endl;
    guassKernel = guassKernel / sum;
    //    for(int i = 0; i< guaSize; i++){
    //        for(int j = 0; j < guaSize; j++){
    //            std::cout<<guassKernel.at<double>(i, j) << " ";
    //        }
    //        std::cout<<std::endl;
    //    }
    for (int i = guassCenter; i < img.rows - guassCenter; i++) {
        for (int j = guassCenter; j < img.cols - guassCenter; j++) {
            rect.x = j - guassCenter;
            rect.y = i - guassCenter;
            rect.width = guaSize;
            rect.height = guaSize;
            filterImg.at<double>(i, j) = cv::sum(guassKernel.mul(img(rect))).val[0];
            // std::cout<<filterImg.at<double>(i,j) << " ";
        }
        // std::cout<<std::endl;
    }
    cv::Mat guassResult;
    filterImg.convertTo(guassResult, CV_8UC1);
    cv::imshow("guass-result", guassResult);
    // std::cout<<cv::sum(guassKernel).val[0]<<std::endl;
    // �����ݶ�,��sobel����
    cv::Mat gradX = cv::Mat::zeros(img.rows, img.cols, CV_64FC1); // ˮƽ�ݶ�
    cv::Mat gradY = cv::Mat::zeros(img.rows, img.cols, CV_64FC1); // ��ֱ�ݶ�
    cv::Mat grad = cv::Mat::zeros(img.rows, img.cols, CV_64FC1);  // �ݶȷ�ֵ
    cv::Mat thead = cv::Mat::zeros(img.rows, img.cols, CV_64FC1); // �ݶȽǶ�
    cv::Mat locateGrad = cv::Mat::zeros(img.rows, img.cols, CV_64FC1); //����
    // x�����sobel����
    cv::Mat Sx = (cv::Mat_<double>(3, 3) << -1, 0, 1,
        -2, 0, 2,
        -1, 0, 1
        );
    // y����sobel����
    cv::Mat Sy = (cv::Mat_<double>(3, 3) << 1, 2, 1,
        0, 0, 0,
        -1, -2, -1
        );
    // �����ݶȸ�ֵ�ͽǶ�
    for (int i = 1; i < img.rows - 1; i++) {
        for (int j = 1; j < img.cols - 1; j++) {
            // ������� 3*3
            rect.x = j - 1;
            rect.y = i - 1;
            rect.width = 3;
            rect.height = 3;
            cv::Mat rectImg = cv::Mat::zeros(3, 3, CV_64FC1);
            filterImg(rect).copyTo(rectImg);
            // �ݶȺͽǶ�
            gradX.at<double>(i, j) += cv::sum(rectImg.mul(Sx)).val[0];
            gradY.at<double>(i, j) += cv::sum(rectImg.mul(Sy)).val[0];
            grad.at<double>(i, j) = sqrt(pow(gradX.at<double>(i, j), 2) + pow(gradY.at<double>(i, j), 2));
            thead.at<double>(i, j) = atan(gradY.at<double>(i, j) / gradX.at<double>(i, j));
            // �����ĸ�����
            if (0 <= thead.at<double>(i, j) <= (pi / 4.0)) {
                locateGrad.at<double>(i, j) = 0;
            }
            else if (pi / 4.0 < thead.at<double>(i, j) <= (pi / 2.0)) {
                locateGrad.at<double>(i, j) = 1;
            }
            else if (-pi / 2.0 <= thead.at<double>(i, j) <= (-pi / 4.0)) {
                locateGrad.at<double>(i, j) = 2;
            }
            else if (-pi / 4.0 < thead.at<double>(i, j) < 0) {
                locateGrad.at<double>(i, j) = 3;
            }
        }
    }
    // debug
    cv::Mat tempGrad;
    grad.convertTo(tempGrad, CV_8UC1);
    imshow("grad", tempGrad);
    // �ݶȹ�һ��
    double gradMax;
    cv::minMaxLoc(grad, &gradMax); // �����ֵ
    if (gradMax != 0) {
        grad = grad / gradMax;
    }
    // debug
    cv::Mat tempGradN;
    grad.convertTo(tempGradN, CV_8UC1);
    imshow("gradN", tempGradN);

    // ˫��ֵȷ��
    cv::Mat caculateValue = cv::Mat::zeros(img.rows, img.cols, CV_64FC1); // grad���һά
    cv::resize(grad, caculateValue, cv::Size(1, (grad.rows * grad.cols)));
    // caculateValue.convertTo(caculateValue, CV_64FC1);
    cv::sort(caculateValue, caculateValue, CV_SORT_EVERY_COLUMN + CV_SORT_ASCENDING); // ����
    long long highIndex = img.rows * img.cols * hightThres;
    double highValue = caculateValue.at<double>(highIndex, 0); // �����ֵ
    // debug
    // std::cout<< "highValue: "<<highValue<<" "<<  caculateValue.cols << " "<<highIndex<< std::endl;

    double lowValue = highValue * lowThres; // ��С��ֵ
    // 3.�Ǽ���ֵ���ƣ� �������Բ�ֵ
    for (int i = 1; i < img.rows - 1; i++) {
        for (int j = 1; j < img.cols - 1; j++) {
            // �˸���λ
            double N = grad.at<double>(i - 1, j);
            double NE = grad.at<double>(i - 1, j + 1);
            double E = grad.at<double>(i, j + 1);
            double SE = grad.at<double>(i + 1, j + 1);
            double S = grad.at<double>(i + 1, j);
            double SW = grad.at<double>(i - 1, j - 1);
            double W = grad.at<double>(i, j - 1);
            double NW = grad.at<double>(i - 1, j - 1);
            // �����жϣ����Բ�ֵ����
            double tanThead; // tan�Ƕ�
            double Gp1; // ����������ݶ�ǿ��
            double Gp2;
            // ��Ƕȣ�����ֵ
            tanThead = abs(tan(thead.at<double>(i, j)));
            switch ((int)locateGrad.at<double>(i, j)) {
            case 0:
                Gp1 = (1 - tanThead) * E + tanThead * NE;
                Gp2 = (1 - tanThead) * W + tanThead * SW;
                break;
            case 1:
                Gp1 = (1 - tanThead) * N + tanThead * NE;
                Gp2 = (1 - tanThead) * S + tanThead * SW;
                break;
            case 2:
                Gp1 = (1 - tanThead) * N + tanThead * NW;
                Gp2 = (1 - tanThead) * S + tanThead * SE;
                break;
            case 3:
                Gp1 = (1 - tanThead) * W + tanThead * NW;
                Gp2 = (1 - tanThead) * E + tanThead * SE;
                break;
            default:
                break;
            }
            // NMS -�Ǽ���ֵ���ƺ�˫��ֵ���
            if (grad.at<double>(i, j) >= Gp1 && grad.at<double>(i, j) >= Gp2) {
                //˫��ֵ���
                if (grad.at<double>(i, j) >= highValue) {
                    grad.at<double>(i, j) = highValue;
                    result.at<double>(i, j) = 255;
                }
                else if (grad.at<double>(i, j) < lowValue) {
                    grad.at<double>(i, j) = 0;
                }
                else {
                    grad.at<double>(i, j) = lowValue;
                }

            }
            else {
                grad.at<double>(i, j) = 0;
            }
        }
    }
    // NMS ������ֵ������ݶ�ͼ
    cv::Mat tempGradNMS;
    grad.convertTo(tempGradNMS, CV_8UC1);
    imshow("gradNMS", tempGradNMS);

    // 4.���ƹ�������ֵ�� 3*3. �ҵ�����ֵ��255
    for (int i = 1; i < img.rows - 1; i++) {
        for (int j = 1; j < img.cols - 1; j++) {
            if (grad.at<double>(i, j) == lowValue) {
                // 3*3������ǿ�ݶ�
                rect.x = j - 1;
                rect.y = i - 1;
                rect.width = 3;
                rect.height = 3;
                for (int i1 = 0; i1 < 3; i1++) {
                    for (int j1 = 0; j1 < 3; j1++) {
                        if (grad(rect).at<double>(i1, j1) == highValue) {
                            result.at<double>(i, j) = 255;
                            std::cout << result.at<double>(i, j);
                            break;
                        }
                    }
                }
            }
        }
    }
    // ���
    result.convertTo(result, CV_8UC1);
    imshow("result", result);


}