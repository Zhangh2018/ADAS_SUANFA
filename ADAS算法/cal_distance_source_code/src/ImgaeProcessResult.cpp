#include "ImgaeProcessResult.h"


ImgaeProcessResult::ImgaeProcessResult(int width, int height,int f)
{
	sz=Size(width, height);
	F = f;
	int		iThLength = 20;//����̫�̵Ļ�
	float	fThObb = 2;
	float	fThPos = 1.0f;
	//float	fTaoCenters = 0.018f;//0.05
	int 	iNs = 16;//����
	float	fMaxCenterDistance = sqrt(float(sz.width*sz.width + sz.height*sz.height)) * 0.005;

	//float	fThScoreScore = 0.5f;//0.8	
	//fTaoCenters = 0.05f;
	// Other constant parameters settings.
	// Gaussian filter parameters, in pre-processing
	Size	szPreProcessingGaussKernelSize = Size(3, 3);
	double	dPreProcessingGaussSigma = 1.0;

	float	fDistanceToEllipseContour = 0.1f;	// (Sect. 3.3.1 - Validation)
	//float	fMinReliability					= 0.4f;	// Const parameters to discard bad ellipses 0.5

	// Initialize Detector with selected parameters
	cned.SetParameters(szPreProcessingGaussKernelSize,
		dPreProcessingGaussSigma,
		fThPos,
		fMaxCenterDistance,
		iThLength,
		fThObb,
		fDistanceToEllipseContour,
		0.2,
		0.2,
		iNs
		);
}


ImgaeProcessResult::~ImgaeProcessResult()
{
}

void ImgaeProcessResult::cal_distance(Mat gray, float& distance, float center_distance, float radius)
{
	Mat canny_img;
	Canny(gray, canny_img, 30, 150);
	//CNEllipseDetector cned;


	// Parameters Settings (Sect. 4.2)

	// Detect 
	Mat1b gray_clone = canny_img.clone();
	vector<Ellipsed> ellsCned_ori;
	cned.Detect(gray_clone, ellsCned_ori);

	vector<Ellipsed> ellsCned;
	for (int i = 0; i < ellsCned_ori.size(); ++i){
		if ((ellsCned_ori[i]._a<80) && (ellsCned_ori[i]._b / ellsCned_ori[i]._a)>0.25)
		{
			ellsCned.push_back(ellsCned_ori[i]);
		}
	}

	//��ʾ���֣��������ʵ��ʹ��ʱȥ����
	/*Mat show(canny_img.rows, canny_img.cols,CV_8U,Scalar(255));
	cned.DrawDetectedEllipses(show, ellsCned);

	imshow("gray", gray);
	imshow("show", show);
	waitKey(1);*/

	//ell_centers�������������Բ��x��y��
	//��ell_centers[0]��ell_centers[1]�������������Բ��x��y��ell_centers[2]��ell_centers[3]�������������Բ��x��y
	vector<vector<int>> ell_centers(4);
	int first_x = 0;
	int first_y = 0;
	float threshold2 = center_distance / radius; //ʵ��Բ�ľ���뾶֮��
	float threshold3 = 0;
	//������ֵ�����������Բ�����ĵ�y�Ĳ�ֵ����100����������Բ������ͬһ�������Բ
	int just_thres;
	int just_thres_x;
	int num = int(ellsCned.size());
	//���û����Բ�������˳�
	if (num == 0)
		return;
	else
	{
		Ellipsed& e = ellsCned[0];
		//�Ե�һ����ԲΪ��׼
		first_x = cvRound(e._xc);
		first_y = cvRound(e._yc);
		/*just_thres = (cvRound(e._a) + cvRound(e._b)) / 2;
		just_thres_x = just_thres / 2;*/

		ell_centers[0].push_back(cvRound(e._xc));
		ell_centers[1].push_back(cvRound(e._yc));

	}
	/*vector<Ellipsed> ellsCned2;*/
	for (int i = 0; i < num; ++i)
	{
		Ellipsed& e = ellsCned[i];
		just_thres = (cvRound(e._a) + cvRound(e._b)) / 2; //����ÿ��Բ�뾶
		float threshold3 = (float)(pow((cvRound(e._xc) - first_x), 2) + pow((cvRound(e._yc) - first_y), 2));
		float threshold4 = sqrt(threshold3) / just_thres; //Բ�ľ���뾶֮��
		/*if (1.5*threshold2 >= threshold4 && threshold4 >= 0.5*threshold2 || threshold4 <= 0.1*threshold2)
		{
		ellsCned2.push_back(ellsCned[i]);
		}

		Mat show1(canny_img.rows, canny_img.cols,CV_8U,Scalar(255));
		Mat show(canny_img.rows, canny_img.cols,CV_8U,Scalar(255));
		cned.DrawDetectedEllipses(show1, ellsCned);
		cned.DrawDetectedEllipses(show, ellsCned2);

		imshow("gray", gray);
		imshow("show1", show1);
		imshow("show", show);
		waitKey(1);*/

		//������ֵ�ж��Ƿ�����ͬһ���������Բ
		if (1.5*threshold2 >= threshold4 && threshold4 >= 0.5*threshold2)
		{
			ell_centers[2].push_back(cvRound(e._xc));
			ell_centers[3].push_back(cvRound(e._yc));
		}
		if (threshold4 <= 0.1*threshold2)
		{
			ell_centers[0].push_back(cvRound(e._xc));
			ell_centers[1].push_back(cvRound(e._yc));
		}

	}
	//���ֻ��һ�����������Բ�����ء�
	if (ell_centers[2].size() == 0)
		return;
	//mean�д�ŵ�����������������Բ�����ƽ����������
	//��mean[0]Ϊ������Բ�����x��ֵ��mean[1]Ϊ������Բ�����y��ֵ��mean[2]Ϊ������Բ�����x��ֵ��mean[3]Ϊ������Բ�����y��ֵ
	int mean[4];
	for (int i = 0; i < 4; i++)
	{
		int sums = std::accumulate(std::begin(ell_centers[i]), std::end(ell_centers[i]), 0.0);
		mean[i] = sums / ell_centers[i].size();
	}
	//�������
	float dis_2 = (float)(pow((mean[0] - mean[2]), 2) + pow((mean[1] - mean[3]), 2));
	//distance = sqrt(dis_2);


	//��ʾ���֣��������ʵ��ʹ��ʱȥ����

	//����ӣ�******************************************************************
	//1�����һ��Բ����Բ����
	//2��������Բ��֮������ؾ��롣
	float center_pix_distance = sqrt(dis_2);//�����ʾ������Բ��֮������ؾ��롣
	//����ӣ�******************************************************************

	distance = center_distance*F / center_pix_distance;
}
