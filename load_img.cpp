#include <iostream>
#include <sstream>
#include <cmath>
#include <core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/stitching.hpp>

using namespace std;
using namespace cv;

#define PHOTO_NUM 100
struct RBG
{
    int R, B, G;
};
int main()
{
    Mat img = imread("C:\\Users\\pc\\Desktop\\birthday\\tiger.jpg");
    int Max_length = 32, Max_width = 20;
    int row = img.rows / Max_width;
    int col = img.cols / Max_length;
    string url = "C:\\Users\\pc\\Desktop\\birthday\\唐嫣文件\\";
    string name = "唐嫣_";
    vector<RBG> source; //存储各个照片的平均像素值
    vector<Mat> photo;
    for(int i = 0; i < PHOTO_NUM; ++i)
    {
        stringstream ss;
        string number;
        ss << i;
        ss >> number;
        string temp_url = url+name+number+".jpg";
        cout << temp_url << endl;
        Mat temp = imread(temp_url);
        cv::resize(temp, temp, Size(Max_length, Max_width), 0, 0, CV_INTER_LINEAR);
        photo.push_back(temp);
        // namedWindow("test", WINDOW_NORMAL);
        // imshow("test", temp);
        // cv::waitKey(1);
        long long int sum_R = 0, sum_G = 0, sum_B = 0;
        for(int j = 0; j < temp.rows; ++j)
            for(int k = 0; k < temp.cols; ++k)
            {
                sum_R += temp.at<Vec3b>(j, i)[0];
                sum_G += temp.at<Vec3b>(j, i)[1];
                sum_B += temp.at<Vec3b>(j, i)[2];
            }
        int size = temp.rows*temp.cols;
        RBG tt;
        tt.R = sum_R / size;
        tt.G = sum_G / size;
        tt.B = sum_B / size;
        source.push_back(tt);
    }
    Mat result;
    for(int i = 0; i < row; ++i)
    {
        Mat row_photo;
        for(int j = 0; j < col; ++j)//每一个块
        {
            long long int sum_R = 0, sum_G = 0, sum_B = 0;
            for(int x = i * Max_length; x < (i + 1) * Max_length; ++x)
                for(int y = j * Max_width; y < (j + 1) * Max_width; ++y)
                {
                    sum_R += img.at<Vec3b>(y, x)[0];
                    sum_G += img.at<Vec3b>(y, x)[1];
                    sum_B += img.at<Vec3b>(y, x)[2];
                }
            int size = Max_width * Max_length;
            sum_R /= size;
            sum_G /= size;
            sum_B /= size;
            int flag, Max_Error = 10000;
            for(int k = 0; k < PHOTO_NUM; ++k)
            {
                int temp = abs(source[k].R-sum_R) + abs(source[k].G-sum_G) + abs(source[k].B-sum_B);
                if(temp < Max_Error)
                {
                    flag = k;   //找出RGB误差最小的
                    Max_Error = temp;
                }    
            }
            if(row_photo.data == 0) //貌似空白的不能拼接
                row_photo = photo[flag];
            else
            {
                vconcat(row_photo, photo[flag], row_photo);
            }
        }
        if(result.data == 0)
            result = row_photo;
        else
        {
            hconcat(result, row_photo, result);
        }
    }
    cv::imwrite("test.jpg", result);
    cv::waitKey(1);
    system("pause");
    return 0;
}