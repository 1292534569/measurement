#include"locate.h"
#include<vector>
#include"imageBaseOP.h"
#define pi 3.14159
#define CV_SORT_EVERY_ROW    0
#define CV_SORT_EVERY_COLUMN 1
#define CV_SORT_ASCENDING    0
#define CV_SORT_DESCENDING   16

using namespace std;

//�Ľ���canny��Եʶ������
void cannyEdgeDetection(Mat img, Mat& result,int guaSize, 
    double hightThres, double lowThres) 
{
    // ��ֵ�˲�
    Rect rect; // IOU����
    Mat filterImg = Mat::zeros(img.rows, img.cols, CV_64FC1);
    img.convertTo(img, CV_64FC1);
    result = Mat::zeros(img.rows, img.cols, CV_64FC1);
    int guassCenter = guaSize / 2; // ��˹�˵����� // (2* guassKernelSize +1) * (2*guassKernelSize+1)��˹�˴�С
    double sigma = 1;   // �����С
    Mat guassKernel = Mat::zeros(guaSize, guaSize, CV_64FC1);
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
    guassKernel = guassKernel / sum;
 
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
    Mat guassResult;
    filterImg.convertTo(guassResult, CV_8UC1);
    // �����ݶ�,��sobel����
    cv::Mat gradX = cv::Mat::zeros(img.rows, img.cols, CV_64FC1); // ˮƽ�ݶ�
    cv::Mat gradY = cv::Mat::zeros(img.rows, img.cols, CV_64FC1); // ��ֱ�ݶ�
    cv::Mat grad = cv::Mat::zeros(img.rows, img.cols, CV_64FC1);  // �ݶȷ�ֵ
    cv::Mat thead = cv::Mat::zeros(img.rows, img.cols, CV_64FC1); // �ݶȽǶ�
    cv::Mat locateGrad = cv::Mat::zeros(img.rows, img.cols, CV_64FC1); //����
    // x�����sobel����
    cv::Mat Sx = (cv::Mat_<double>(3, 3) << -1, 0, 1,
                                            -2, 0, 2,
                                            -1, 0, 1);
                                            
    // y����sobel����
    cv::Mat Sy = (cv::Mat_<double>(3, 3) << 1, 2, 1,
                                            0, 0, 0,
                                            -1, -2, -1);
    // �����ݶȸ�ֵ�ͽǶ�
    for (int i = 1; i < img.rows - 1; i++) {
        for (int j = 1; j < img.cols - 1; j++) {
            // ������� 3*3
            rect.x = j - 1;
            rect.y = i - 1;
            rect.width = 3;
            rect.height = 3;
            Mat rectImg = Mat::zeros(3, 3, CV_64FC1);
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