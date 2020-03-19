#include <iostream>
#include <sstream>
#include <cmath>
#include <core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/stitching.hpp>

using namespace std;
using namespace cv;

struct RBG
{
    int R, B, G;
    float priority;
};
int main()
{
    string s;
    int PHOTO_NUM;
    cout << "�����ͼ����ԭͼ���ľ���·��" << endl;
    cin >> s;
    Mat img = imread(s); //��Ҫ��Ϊ��ͼ��ͼƬ����
    int Max_length = 5, Max_width = 4;
    if (img.data == 0)
    {
        cout << "�������, �Ҳ�����ͼƬ"<< endl;
        system("pause");
        return 1;
    }
    else
    {
        if(img.cols % 100 != 0 || img.rows % 100 != 0)
        {
            cout << "ͼƬ��С������Ҫ��"<< endl;
            system("pause");
            return 1;
        }
        else
        {
            Max_length = img.cols/100;
            Max_width = img.rows/100;
        }
        
    }
    
    int row = img.rows / Max_width;
    int col = img.cols / Max_length;

    string url; //��Ϊƴ���õ�Сͼ����
    string name;
    cout << "�����ز�ͼ�ļ��еľ���·��(��C:\\xxx ���Դ��Ҽ�-�����Բ鿴)" << endl;
    cin >> url;
    url += "\\";   //��ֹ���˼ӷ�б��  
    cout << "�����ز�ͼ�ļ�����ǰ׺(�� ����_�������˵��µ��»���)" <<endl;
    cin >> name;
    cout << "�����ز�ͼ�ļ���������ȡ��������ز�ͼ�ļ����ж���ͼƬ��" <<endl;
    cin >> PHOTO_NUM;
    vector<RBG> source; //�洢������Ƭ��ƽ������ֵ
    vector<Mat> photo;
    for(int i = 0; i < PHOTO_NUM; ++i) //����ÿ��Сͼ��RGBֵ
    {
        stringstream ss;
        string number;
        ss << i;
        ss >> number;
    
        string temp_url = url+name+number+".jpg";
        cout << temp_url << endl;
        Mat temp = imread(temp_url);
        // cv::resize(temp, temp, Size(500, 400), 0, 0, CV_INTER_LINEAR);
        // photo.push_back(temp);
        cv::resize(temp, temp, Size(Max_length*4, Max_width*4), 0, 0, CV_INTER_LINEAR);
        photo.push_back(temp);
        long long int sum_R = 0, sum_G = 0, sum_B = 0;
        for(int j = 0; j < temp.rows; ++j)
            for(int k = 0; k < temp.cols; ++k)
            {
                sum_R += temp.at<Vec3b>(j, k)[0];
                sum_G += temp.at<Vec3b>(j, k)[1];
                sum_B += temp.at<Vec3b>(j, k)[2];
            }
        int size = temp.rows*temp.cols;
        RBG tt;
        tt.R = sum_R / size;
        tt.G = sum_G / size;
        tt.B = sum_B / size;
        tt.priority = 0; //���ȼ�
        source.push_back(tt);
    }
    Mat result;
    for(int i = 0; i < row; ++i)
    {
        Mat row_photo;
        for(int j = 0; j < col; ++j)//ÿһ����
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
            for(int k = 0; k < PHOTO_NUM; ++k) // Ѱ�������С��Сͼ
            {
                int temp = abs(source[k].R-sum_R) + abs(source[k].G-sum_G) + abs(source[k].B-sum_B) + source[k].priority;
                if(temp < Max_Error)
                {
                    flag = k;   //�ҳ�RGB�����С��
                    Max_Error = temp;
                }    
            }
            if(row_photo.data == 0) //ò�ƿհ׵Ĳ���ƴ��
                row_photo = photo[flag];
            else
            {
                vconcat(row_photo, photo[flag], row_photo);
            }
            source[flag].priority += 1;
        }
        if(result.data == 0)
            result = row_photo;
        else
        {
            hconcat(result, row_photo, result);
        }
    }
    cv::imwrite("test3.jpg", result);
    cv::waitKey(1);
    system("pause");
    return 0;
}