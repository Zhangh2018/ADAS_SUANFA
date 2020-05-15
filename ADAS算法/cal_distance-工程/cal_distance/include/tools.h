

#pragma once
#include "stdafx.h"
#include "CNEllipseDetector.h"
//#include<stdio.h>


#define V2SP Point2f p3,Point2f p2,Point2f p1,Point2f p4,Point2f p5,Point2f p6

void MultiImage_OneWin(const std::string& MultiShow_WinName, const vector<Mat>& SrcImg_V, CvSize SubPlot, CvSize ImgMax_Size);
Point2f lineCrossPoint(Point2f l1p1, Point2f l1p2, Point2f l2p1, Point2f l2p2);
void point2Mat(Point2f p1, Point2f p2, float mat[2][2]);
float value4SixPoints(V2SP);
void PyrDown(string picName);
Mat matResize(Mat src, double scale);
void SaveEllipses(const string& fileName, const vector<Ellipsed>& ellipses);
// 14pr
void SaveEllipses(const string& workingDir, const string& imgName, const vector<Ellipsed>& ellipses /*, const vector<double>& times*/);
void LoadGT(vector<Ellipsed>& gt, const string& sGtFileName, bool bIsAngleInRadians = true);
// useless 
bool LoadTest(vector<Ellipsed>& ellipses, const string& sTestFileName, vector<double>& times, bool bIsAngleInRadians = true);

bool TestOverlap(const Mat1b& gt, const Mat1b& test, float th);
int Count(const vector<bool> v);
float Evaluate(const vector<Ellipsed>& ellGT, const vector<Ellipsed>& ellTest, const float th_score, const Mat3b& img);
// end 14pr

//saalt
void salt(cv::Mat& image, int n);
//salt


//����ѡ���Ļ���
/**
���룺һ�����޵Ļ�������
�������ʾ���Σ�һ�����޵�һ����ɫ����ɫ���
*/
void showEdge(vector<vector<Point>> points_, Mat& picture);
//����ѡ���Ļ���

//file operation
int writeFile(string fileName_cpp, vector<string> vsContent);
int readFile(string fileName_cpp);
int readFileByChar(string fileName_split);
void Trim(string &str);

/******�����ض���ʽ������******/
//C++��û��Split()�����������Ҫ�Զ��庯���������ݣ���C#��Java�����������
vector<string> getStr(string str);

void listDir(string real_dir, vector<string>& files, bool r = false);

