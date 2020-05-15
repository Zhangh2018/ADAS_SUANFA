//#include <iostream>
//#include <string>
//#include <fstream>
//#include <io.h>
//#include <vector>
#include "ImgaeProcessResult.h"
//#include <algorithm>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/video/background_segm.hpp>
#include <cv.h>   //cv.h OpenCV����Ҫ����ͷ�ļ������Ҫ��
#include <highgui.h>

using namespace std;
using namespace cv;

int main(int argc,char* argv[])
{
	//Mat img = imread("test3.bmp", 0);
	VideoCapture capture ("5.mp4");

	ImgaeProcessResult output(1280,720,5490);//��ʼ�������ߡ�����

	while(1)
	{
		Mat frame;
		capture>>frame;
		if (frame.empty())
		{break;
		}
		float distance = 0;
		output.cal_distance(frame, distance,1,0.15);//������룺��֡ͼ�񡢾��롢����Բ�ľ��롢Բ�뾶��
		cout << "distance:" << distance << endl;
		//system("pause");
	}
}