#include "myqt.h"

static int nClusters;
int val_user = 0;
static int flag = 1;
static QString fileNm;
IplImage *pResult;
CvMat *samples;
CvMat *clusters;
CvMat *centers;
IplImage* img;

using namespace cv;
using namespace std;

//int t = 1;
static int clickCirleBtnCount = 0;

void get_RGB(IplImage* img, CvMat*samples)
{
	int k = 0;
	for (int i = 0; i < img->width; i++)
	{
		for (int j = 0; j < img->height; j++)
		{
			CvScalar s;
			//获取图像各个像素点的三通道值（RGB）,s.val[0] 代表src图像BGR中的B通道的值~
			s = cvGet2D(img, j, i);
			cvSet2D(samples, k++, 0, s);//将像素点三通道的值按顺序排入样本矩阵
		}
	}
}

//将RGB值转换成HSV值，方便判断颜色
void Rgb2Hsv(double R, double G, double B, double& H, double& S, double&V)
{
	double max1, min1;
	R = R / 255;
	G = G / 255;
	B = B / 255;
	max1 = max(R, G, B);
	min1 = min(R, G, B);
	if (max1 == min1)
		H = 0.0;
	else if (R == max1&&G >= B)
		H = (G - B) / (max1 - min1);
	else if (R == max1&&G < B)
		H = 6 + (G - B) / (max1 - min1);
	else if (G == max1)
		H = 2 + (B - R) / (max1 - min1);
	else if (B == max1)
		H = 4 + (R - G) / (max1 - min1);
	H = H * 60;
	if (H < 0)
		H = H + 360;
	V = max1;
	if (max1 == 0)
		S = 0;
	else
		S = (max1 - min1) / max1;
	H = H / 2;      //H:0~180
	S = S * 255;//S:0~255
	V = V * 255;//V:0~255

}
char* judge_color(double H, double S, double V)
{
	if (S >= 43)
	{
		if (V <= 46)
			return"黑色";
		else
		{
			if (H <11 || (H >= 156 && H <= 180))
				return"红色";
			else if (H >= 11 && H<26)
				return "橙色";
			else if (H >= 26 && H <35)
				return"黄色";
			else if (H >= 35 && H <78)
				return"绿色";
			else if (H >= 78 && H <100)
				return"青色";
			else if (H >= 100 && H<125)
				return"蓝色";
			else if (H >= 125 && H <156)
				return"紫色";
			else
				return "未识别颜色";
		}
	}
	else
	{
		if (S > 30 && V >= 46 && V <221)
			return"灰色";
		else if (S <= 30 && V >= 221 && V <= 256)
			return "白色";
		else
			return "未识别颜色";
	}
}

//将opencv处理后的图像显示在QT界面内
void displayInQt(QString pathname, QLabel *displayLabel, IplImage* saveImage)
{
	QByteArray ba = pathname.toLatin1();
	char *colorSaveto = ba.data();
	cvSaveImage(colorSaveto, saveImage);
	QImage *numberContours = new QImage(colorSaveto);
	if (numberContours->height() > displayLabel->height())
	{
		if (numberContours->width() > displayLabel->width())
			*numberContours = numberContours->scaled(displayLabel->width(), displayLabel->height());
		else
			*numberContours = numberContours->scaled(numberContours->width(), displayLabel->height());
	}
	else
	{
		if (numberContours->width() > displayLabel->width())
			*numberContours = numberContours->scaled(displayLabel->width(), numberContours->height());
	}
	displayLabel->setPixmap(QPixmap::fromImage(*numberContours));
	delete numberContours;
	remove(colorSaveto);
}
//输入想要显示的颜色，输出此颜色的区域
void MyQt::point_color(CvMat*clusters, IplImage *img, IplImage *pResult, MyQt label)
{
	int val = 0;
	int k = 0;
	for (int i = 0; i < img->width; i++)
	{
		for (int j = 0; j < img->height; j++)
		{
			CvScalar s;
			val = (int)clusters->data.i[k++];
			if (val == val_user)
			{
				//将与user输入的对应的每个像素点设置成原值
				s = cvGet2D(img, j, i);
				cvSet2D(pResult, j, i, s);
			}
			else
			{
				//设置其他的点的像素
				s.val[0] = 255;
				s.val[1] = 255;
				s.val[2] = 255;
				cvSet2D(pResult, j, i, s);
			}
		}
	}
	QString colorName = QString("./imgSave/color%1.jpg").arg(val_user);
	displayInQt(colorName, ui.labelDisplay, pResult);
	
	//QString winName = QString("Cluster - %1").arg(val_user);
	//cvShowImage(winName.toLatin1().data(), pResult);
	//换成QT内输出
	//QImage *myimg = IplImageToQImage(pResult);
	//*myimg = myimg->scaled(label.getLabel()->width(), label.getLabel()->height());//读取label尺寸，改变图片大小
	//label.getLabel()->setPixmap(QPixmap::fromImage(*myimg));//绘制图片
}

//获取每个聚类中心的RGB值，然后转换成HSV值，用来判断颜色
void MyQt::center_RGB(int nClusters, CvMat* centers)
{
	double out[3];
	double H, S, V;
	char *color;
	color = (char*)malloc(nClusters * sizeof(char));
	int k = 0;

	int **rgb = (int**)malloc(sizeof(int*) * nClusters);
	for (int i = 0; i < nClusters; i++)  
		rgb[i] = (int*)malloc(sizeof(int) * 3);
	stack<int*> s;

	for (int i = 0,j=0; i<nClusters * 3;j++)
	{
		out[2] = centers->data.fl[i++];//centers里面第一个存的是B值
		out[1] = centers->data.fl[i++];//第二个存的是G值
		out[0] = centers->data.fl[i++];//第三个存的是R值
		//存入数组准备放入栈
		rgb[j][0] = out[0]; 
		rgb[j][1] = out[1]; 
		rgb[j][2] = out[2];
		s.push(rgb[j]);
		//将每个聚类中心的RGB值转换成HSV值，方便判断其颜色
		Rgb2Hsv(out[0], out[1], out[2], H, S, V);
		color = judge_color(H, S, V);
	}
	
	QVBoxLayout *layout = new QVBoxLayout(ui.labelPane);
	layout->setObjectName("colorLayout");
	for (int i = 0; i < nClusters; i++)
	{
		int *ptr = s.top();
		QLabel *label = new QLabel;
		label->setStyleSheet(QWidget::tr("background-color:rgb(%1,%2,%3);").arg(ptr[0]).arg(ptr[1]).arg(ptr[2]));
		label->setMaximumHeight(30);
		label->setMaximumWidth(50);
		label->setObjectName(QObject::tr("color%1").arg(i));
		layout->addWidget(label);
		s.pop();
	}
	this->update();
	for (int i = 0; i < nClusters; i++)
		delete rgb[i];
	delete rgb;
}

/////////////////////////////////////////////////////////////////////////////////
/* 彩色直方图 */
#define cvQueryHistValue_2D( hist, idx0, idx1 )   cvGetReal2D( (hist)->bins, (idx0), (idx1) )
#define cvCvtPixToPlane cvSplit
void MyQt::histogram(IplImage *img)
{
	IplImage* hsv = cvCreateImage(cvGetSize(img), 8, 3);
	IplImage* h_plane = cvCreateImage(cvGetSize(img), 8, 1);
	IplImage* s_plane = cvCreateImage(cvGetSize(img), 8, 1);
	IplImage* v_plane = cvCreateImage(cvGetSize(img), 8, 1);
	IplImage* planes[] = { h_plane, s_plane };

	/** H 分量划分为16个等级，S分量划分为8个等级 */
	int h_bins = 10, s_bins = 8;
	int hist_size[] = { h_bins, s_bins };

	/** H 分量的变化范围 */
	float h_ranges[] = { 0, 180 };

	/** S 分量的变化范围*/
	float s_ranges[] = { 0, 255 };
	float* ranges[] = { h_ranges, s_ranges };

	/** 输入图像转换到HSV颜色空间 */
	cvCvtColor(img, hsv, CV_BGR2HSV);
	cvCvtPixToPlane(hsv, h_plane, s_plane, v_plane, 0);

	/** 创建直方图，二维, 每个维度上均分 */
	CvHistogram * hist = cvCreateHist(2, hist_size, CV_HIST_ARRAY, ranges, 1);
	/** 根据H,S两个平面数据统计直方图 */
	cvCalcHist(planes, hist, 0, 0);

	/** 获取直方图统计的最大值，用于动态显示直方图 */
	float max_value;
	cvGetMinMaxHistValue(hist, 0, &max_value, 0, 0);


	/** 设置直方图显示图像 */
	int height = 150;
	int width = 5*(h_bins*s_bins * 6);
	IplImage* hist_img = cvCreateImage(cvSize(width*1.5, height), 8, 3);
	cvZero(hist_img);

	/** 用来进行HSV到RGB颜色转换的临时单位图像 */
	IplImage * hsv_color = cvCreateImage(cvSize(1, 1), 8, 3);
	IplImage * rgb_color = cvCreateImage(cvSize(1, 1), 8, 3);
	int bin_w = width / (h_bins * s_bins);
	for (int h = 0; h < h_bins; h++)
	{
		for (int s = 0; s < s_bins; s++)
		{
			int i = h*s_bins + s;
			/** 获得直方图中的统计次数，计算显示在图像中的高度 */
			float bin_val = cvQueryHistValue_2D(hist, h, s);
			int intensity = cvRound(bin_val*height / max_value);

			/** 获得当前直方图代表的颜色，转换成RGB用于绘制 */
			cvSet2D(hsv_color, 0, 0, cvScalar(h*180.f / h_bins, s*255.f / s_bins, 255, 0));
			cvCvtColor(hsv_color, rgb_color, CV_HSV2BGR);
			CvScalar color = cvGet2D(rgb_color, 0, 0);

			cvRectangle(hist_img, cvPoint(i*bin_w, height),
					cvPoint((i + 5)*bin_w, height - intensity),
				color, -1, 8, 0);
		}
	}
	
	char* histogramSaveto = "./imgSave/histogram.jpg";
	cvSaveImage(histogramSaveto, hist_img);

	QImage *histogram = new QImage(histogramSaveto);
	*histogram = histogram->scaled(ui.histogram->width(), ui.histogram->height());
	ui.histogram->setPixmap(QPixmap::fromImage(*histogram));
	delete histogram;
	remove(histogramSaveto);
}

/////////////////////////////////////////////////////////////////////////////////
/* 数字识别 */
#define N 10
double mint = 100;
int thres = 115;    //二值化阀值
CvSeq *pic[N];
int modelFlag = 1;
string picture[] = { "templates/0","templates/1","templates/2","templates/3","templates/4","templates/5","templates/6","templates/7","templates/8","templates/9" };
//储存数字图片轮廓
CvSeq* GetImageContour(IplImage* srcIn, int flag = 1)
{
	IplImage* src;
	CvSeq* seq = NULL;           //储存图片轮廓信息
	int total = 0;    //轮廓总数
	int count = 0;
	src = cvCreateImage(cvGetSize(srcIn), 8, 1);
	//拷贝图像
	cvCopy(srcIn, src);
	//创建空间
	CvMemStorage* mem = cvCreateMemStorage(0);
	//二值化图像
	cvThreshold(src, src, thres, 255, CV_THRESH_BINARY_INV);
	//cvCanny(src, src, 110, 110);
	if (!mem)
	{
		printf("mem is NULL!");
	}
	////计算图像轮廓  0-只获取最外部轮廓  1-获取全部轮廓	
	total = cvFindContours(src, mem, &seq, sizeof(CvContour), CV_RETR_CCOMP, CV_CHAIN_APPROX_NONE, cvPoint(0, 0));

	//释放图像空间
	cvReleaseImage(&src);
	//返回轮廓信息
	return seq;
}

//数字图片轮廓计算
void Init(void)
{
	string t;
	if (modelFlag == 1)
		t = "_1.jpg";
	else
		t = "_2.jpg";
	for (int i = 0; i < 10; i++)
		picture[i] = picture[i] + t;
	IplImage *src0;
	int i;
	//创建空间
	CvMemStorage* mem = cvCreateMemStorage(0);

	for (i = 0; i<N; i++)
	{
		src0 = cvLoadImage(picture[i].c_str(), CV_LOAD_IMAGE_GRAYSCALE);
		IplImage*src = cvCreateImage(cvGetSize(src0), src0->depth, 1);
		if (!src0)
		{
			printf("Couldn't load %s\n", picture[i]);
			exit(1);
		}
		//二值化图像
		cvThreshold(src0, src0, thres, 255, CV_THRESH_BINARY_INV);
		cvCopy(src0, src);

		cvFindContours(src, mem, &pic[i], sizeof(CvContour), CV_RETR_CCOMP, CV_CHAIN_APPROX_NONE, cvPoint(0, 0));
	}
}

int ReadNumber(CvSeq* contoursTemp, int moshi)
{

	int i;
	double tmp = 5;
	mint = 5;
	int num = -1;
	for (i = 0; i < N; i++)
	{
		tmp = cvMatchShapes(contoursTemp, pic[i], moshi);   //匹配
		if (tmp < mint)
		{
			mint = tmp;
			num = i;
		}
	}
	return num;
}
void Paixu(int numList[1000][3], int count, char* finalText)    //将数字按横坐标从左往右排列
{
	int i, j;  //循环
	int n = 0;  //融合后的数字个数
	int tem;
	int head = 0, tail = 1;
	int width = 0;   //两数字间的距离
	int newList[1000] = { 0 };  //数字融合后的新序列
	char numberText[1000][2];
	
	for (i = 0; i<count - 1; i++)
	{
		for (j = i + 1; j<count; j++)
		{
			if (/*numList[i][2] > numList[j][2] ||*/ numList[i][1] > numList[j][1])
			{
				//交换坐标
				tem = numList[i][1]; numList[i][1] = numList[j][1]; numList[j][1] = tem;
				//交换数字
				tem = numList[i][0]; numList[i][0] = numList[j][0]; numList[j][0] = tem;
			}
		}
	}
	if (count == 0)
	{
		printf("no number!");
	}

	else
	{
		for (i = 0; i<count; i++)
		{
			itoa(numList[i][0], numberText[i], 10);
			strcat(finalText, numberText[i]);
		}
	}
}

void MyQt::IdenNumer2(IplImage* img )
{


	int i;  //循环标志
	int count = 0;   //数字轮廓个数
	int num = -1;   //识别一幅图像中的一个数字
	int numList[1000][3];  //一幅图像中的数字序列  一维是数字，二维是数字所在横坐标
	int moshi; //想要用的模式
			   //cout << "请选择想用的模式：（1-3）" << endl;
			   //cin >> moshi;
	moshi = 3;

	CvPoint pt1, pt2;
	//CvRect ins;
	Init();   //初始化,在pic中储存所有图片轮廓

	CvMemStorage* storage = cvCreateMemStorage(0);

	
	IplImage* imgColor = cvCreateImage(cvGetSize(img), 8, 3);
	IplImage* contoursImage = cvCreateImage(cvSize(img->width, img->height), 8, 1);

	CvSeq* contours = 0, *contoursTemp = 0;
	cvZero(contoursImage);
	contours = GetImageContour(img, 1);   //获取轮廓信息
	contoursTemp = contours;


	//对图像进行二值化
	cvThreshold(img, img, 100, 255, CV_THRESH_BINARY);
	//img的备份
	cvCvtColor(img, imgColor, CV_GRAY2BGR);

	//对轮廓进行循环
	for (; contoursTemp != 0; contoursTemp = contoursTemp->h_next)
	{
		num = -1;
		CvRect rect = cvBoundingRect(contoursTemp, 1);  //根据序列，返回轮廓外围矩形
														//匹配该轮廓数字
		num = ReadNumber(contoursTemp, moshi);
		if (mint <= 1)
		{//计算矩形顶点
			pt1.x = rect.x;
			pt1.y = rect.y;
			pt2.x = rect.x + rect.width;
			pt2.y = rect.y + rect.height;
			if (num >= 0)
			{
				numList[count][0] = num;         //一维存数字
				numList[count][1] = rect.x;      //二维存数字横坐标
				numList[count][2] = rect.y;
			}
			//在原图上绘制轮廓外矩形
			cvRectangle(imgColor, pt1, pt2, CV_RGB(0, 255, 0), 2);
			//提取外轮廓 上的坐标点
			for (i = 0; i < contoursTemp->total; i++)
			{
				CvPoint * pt = (CvPoint*)cvGetSeqElem(contoursTemp, i); // 读出第i个点。
				cvSetReal2D(contoursImage, pt->y, pt->x, 255.0);
				//cvSet2D(imgColor, pt->y, pt->x, cvScalar(0, 0, 255, 0));
			}
			count++;    //数字轮廓+1
		}
	}
	char finalText[1000];
	Paixu(numList, count, finalText);   //将数字按横坐标从左往右的顺序排列

	QString colorName = QString("./imgSave/numberContours.jpg");
	displayInQt(colorName, ui.labelDisplay, contoursImage);
	ui.numberText->setText(QString::fromLocal8Bit(finalText));
	
	for (int i = 0; i < N; ++i)
	{
		picture[i].erase(11, 6);
		cout<< picture[i]<<endl;
	}
	remove("imgSave/Roi.jpg");
}

void MyQt::facerecog(IplImage *img)
{
	// 加载Haar特征检测分类器
	// haarcascade_frontalface_alt.xml系OpenCV自带的分类器 
	const char *pstrCascadeFileName = ".\\haarcascade_frontalface_alt.xml";
	CvHaarClassifierCascade *pHaarCascade = NULL;
	pHaarCascade = (CvHaarClassifierCascade*)cvLoad(pstrCascadeFileName);

	IplImage *pGrayImage = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 1);
	cvCvtColor(img, pGrayImage, CV_BGR2GRAY);
	// 人脸识别与标记
	if (pHaarCascade != NULL)
	{
		CvScalar FaceCirclecolors[] =
		{
			 { 0, 0, 255 } ,
			 { 0, 128, 255 } ,
			 { 0, 255, 255 } ,
			 { 0, 255, 0 } ,
			 { 255, 128, 0 } ,
			 { 255, 255, 0 } ,
			 { 255, 0, 0 } ,
			 { 255, 0, 255 } 
		};

		CvMemStorage *pcvMStorage = cvCreateMemStorage(0);
		cvClearMemStorage(pcvMStorage);
		// 识别
		DWORD dwTimeBegin, dwTimeEnd;
		dwTimeBegin = GetTickCount();
		CvSeq *pcvSeqFaces = cvHaarDetectObjects(pGrayImage, pHaarCascade, pcvMStorage);
		dwTimeEnd = GetTickCount();

		printf("人脸个数: %d   识别用时: %d ms\n", pcvSeqFaces->total, dwTimeEnd - dwTimeBegin);

		// 标记
		for (int i = 0; i <pcvSeqFaces->total; i++)
		{
			CvRect* r = (CvRect*)cvGetSeqElem(pcvSeqFaces, i);
			CvPoint center;
			int radius;
			center.x = cvRound((r->x + r->width * 0.5));
			center.y = cvRound((r->y + r->height * 0.5));
			radius = cvRound((r->width + r->height) * 0.25);
			cvCircle(img, center, radius, FaceCirclecolors[i % 8], 2);
		}
		cvReleaseMemStorage(&pcvMStorage);
	}

	QString cannySaveto = QString("./imgSave/faceRecog.jpg");
	displayInQt(cannySaveto, ui.labelDisplay, img);
	
	cvReleaseImage(&pGrayImage);
}

/* 圆形检测 */
void circleRANSAC(Mat &image, std::vector<Vec3f> &circles, double canny_threshold, double circle_threshold, int numIterations)
{
	CV_Assert(image.type() == CV_8UC1 || image.type() == CV_8UC3);
	circles.clear();

	
	Mat edges;
	Canny(image, edges, MAX(canny_threshold / 2, 1), canny_threshold, 3);// Edge Detection
	std::vector<Point2d> points;		// Create point set from Canny Output
	for (int r = 0; r < edges.rows; r++)
	{
		for (int c = 0; c < edges.cols; c++)
		{
			if (edges.at<unsigned char>(r, c) == 255)
				points.push_back(cv::Point2d(c, r));
		}
	}

	// 4 point objects to hold the random samples
	Point2d pointA;
	Point2d pointB;
	Point2d pointC;
	Point2d pointD;

	// distances between points
	double AB;
	double BC;
	double CA;
	double DC;

	// varibales for line equations y = mx + b
	double m_AB;
	double b_AB;
	double m_BC;
	double b_BC;

	// varibles for line midpoints
	double XmidPoint_AB;
	double YmidPoint_AB;
	double XmidPoint_BC;
	double YmidPoint_BC;

	// variables for perpendicular bisectors
	double m2_AB;
	double m2_BC;
	double b2_AB;
	double b2_BC;

	// RANSAC
	cv::RNG rng;
	int min_point_separation = 10; // change to be relative to image size?
	int colinear_tolerance = 1; // make sure points are not on a line
	int radius_tolerance = 3; // change to be relative to image size?
	int points_threshold = 10; //should always be greater than 4
							   //double min_circle_separation = 10; //reject a circle if it is too close to a previously found circle
							   //double min_radius = 10.0; //minimum radius for a circle to not be rejected

	int x, y;
	Point2d center;
	double radius;

	// Iterate
	for (int iteration = 0; iteration < numIterations; iteration++)
	{

		// get 4 random points
		pointA = points[rng.uniform((int)0, (int)points.size())];
		pointB = points[rng.uniform((int)0, (int)points.size())];
		pointC = points[rng.uniform((int)0, (int)points.size())];
		pointD = points[rng.uniform((int)0, (int)points.size())];

		// calc lines
		AB = norm(pointA - pointB);
		BC = norm(pointB - pointC);
		CA = norm(pointC - pointA);
		DC = norm(pointD - pointC);

		// one or more random points are too close together
		if (AB < min_point_separation || BC < min_point_separation || CA < min_point_separation || DC < min_point_separation) 
			continue;

		//find line equations for AB and BC
		//AB
		m_AB = (pointB.y - pointA.y) / (pointB.x - pointA.x + 0.000000001); //avoid divide by 0
		b_AB = pointB.y - m_AB*pointB.x;
		//BC
		m_BC = (pointC.y - pointB.y) / (pointC.x - pointB.x + 0.000000001); //avoid divide by 0
		b_BC = pointC.y - m_BC*pointC.x;


		//test colinearity (ie the points are not all on the same line)
		if (abs(pointC.y - (m_AB*pointC.x + b_AB + colinear_tolerance)) < colinear_tolerance) 
			continue;

		//find perpendicular bisector
		//AB
		XmidPoint_AB = (pointB.x + pointA.x) / 2.0;//midpoint
		YmidPoint_AB = m_AB * XmidPoint_AB + b_AB;
		m2_AB = -1.0 / m_AB;
		b2_AB = YmidPoint_AB - m2_AB*XmidPoint_AB;
		//BC
		XmidPoint_BC = (pointC.x + pointB.x) / 2.0;//midpoint
		YmidPoint_BC = m_BC * XmidPoint_BC + b_BC;
		m2_BC = -1.0 / m_BC;
		b2_BC = YmidPoint_BC - m2_BC*XmidPoint_BC;

		//find intersection = circle center
		x = (b2_AB - b2_BC) / (m2_BC - m2_AB);
		y = m2_AB * x + b2_AB;
		center = Point2d(x, y);
		radius = cv::norm(center - pointB);

		//check if the 4 point is on the circle
		if (abs(cv::norm(pointD - center) - radius) > radius_tolerance) 
			continue;

		// vote
		std::vector<int> votes;
		std::vector<int> no_votes;
		for (int i = 0; i < (int)points.size(); i++)
		{
			double vote_radius = norm(points[i] - center);
			if (abs(vote_radius - radius) < radius_tolerance)
				votes.push_back(i);
			else
				no_votes.push_back(i);
		}

		// check votes vs circle_threshold
		if ((float)votes.size() / (2.0*CV_PI*radius) >= circle_threshold)
		{
			circles.push_back(Vec3f(x, y, radius));


			// remove points from the set so they can't vote on multiple circles
			std::vector<Point2d> new_points;
			for (int i = 0; i < (int)no_votes.size(); i++)
			{
				new_points.push_back(points[no_votes[i]]);
			}
			points.clear();
			points = new_points;
		}

		// stop RANSAC if there are few points left
		if ((int)points.size() < points_threshold)
			break;
	}

	return;
}

/* 注册 */
MyQt::MyQt(QWidget *parent)
	: QMainWindow(parent)
{

	ui.setupUi(this);
	connect(ui.openImage, SIGNAL(clicked()), this, SLOT(openImage_clicked()));
	connect(ui.submitClusters, SIGNAL(clicked()), this, SLOT(submitClusters_clicked()));
	connect(ui.displayCluster, SIGNAL(clicked()), this, SLOT(displayCluster_clicked()));
	connect(ui.circleDeteBtn, SIGNAL(clicked()), this, SLOT(circleDeteBtn_clicked()));
	connect(ui.numberDeteBtn, SIGNAL(clicked()), this, SLOT(numberDeteBtn_clicked()));
	connect(ui.faceDeteBtn, SIGNAL(clicked()), this, SLOT(faceDeteBtn_clicked()));
}

/* 槽函数的实现 */
void MyQt::circleDeteBtn_clicked()
{
	QByteArray ba = fileNm.toLatin1();//合起来写会有乱码
	char* filename = ba.data();
	Mat image = imread(filename, 0);
	vector<Vec3f> circles;
	
	if (clickCirleBtnCount % 2 == 0)
	{
		double canny_threshold = 150.0;
		double circle_threshold = 0.9;
		int iterations = 15000;
		circleRANSAC(image, circles, canny_threshold, circle_threshold, iterations);
		
		Mat image2 = imread(filename, 1);
		cvtColor(image, image, CV_GRAY2RGB);
		for (int i = 0; i < (int)circles.size(); i++)
		{
			int x = circles[i][0];
			int y = circles[i][1];
			float rad = circles[i][2];
			circle(image2, Point(x, y), rad, Scalar(0, 0, 255),3);	
		}
		clickCirleBtnCount++;

		QString circleSaveto = QString("./imgSave/circleSaveto.jpg");
		displayInQt(circleSaveto, ui.labelDisplay, &IplImage(image2));
		
	}
	else
	{
		IplImage* src = cvLoadImage(filename, 0);
		IplImage* dst;
		int i;
		if (!src)
		{
			printf("Load image error!\n");
			return;
		}
		CvMemStorage* storage = cvCreateMemStorage(0);
		CvSeq* lines = 0;
		dst = cvCreateImage(cvGetSize(src), 8, 1);
		cvCanny(src, dst, 50, 200, 3);		
		lines = cvHoughLines2(dst, storage, CV_HOUGH_PROBABILISTIC, 1, CV_PI / 180, 50, 50, 10);
		IplImage* color_dst = cvLoadImage(filename, 1);
		for (i = 0; i < lines->total; i++)
		{
			CvPoint* line = (CvPoint*)cvGetSeqElem(lines, i);
			cvLine(color_dst, line[0], line[1], CV_RGB(0, 0, 255), 3, CV_AA, 0);
		}

		QString polygonSaveto = QString("./imgSave/polygonSaveto.jpg");
		displayInQt(polygonSaveto, ui.labelDisplay, color_dst);

		clickCirleBtnCount++;
	}
}
void MyQt::numberDeteBtn_clicked()
{
	QByteArray ba = fileNm.toLatin1();//合起来写会有乱码
	char* filename = ba.data();
	char* ptr = filename + strlen(filename) - 9;

	if (strcmp(ptr, "obama.jpg") == 0)
	{
		modelFlag = 2;
		IplImage* src0 = cvLoadImage(filename, CV_LOAD_IMAGE_GRAYSCALE);//CV_LOAD_IMAGE_GRAYSCALE加载灰度图
		IplImage* src = cvCreateImage(cvGetSize(src0), src0->depth, 1);//创建目标图像;
		cvCopy(src0, src);
		CvSize size = cvSize(450, 30);//区域大小
		cvSetImageROI(src, cvRect(300, 460, size.width, size.height));//设置源图像ROI
		IplImage* pDest = cvCreateImage(size, src->depth, src->nChannels);//创建目标图像
		cvCopy(src, pDest); //复制图像
		cvResetImageROI(pDest);//源图像用完后，清空ROI
		cvSaveImage("imgSave/Roi.jpg", pDest);//保存目标图像
		IdenNumer2(pDest);
	}
	else
	{
		modelFlag = 1;
		IplImage* src0 = cvLoadImage(filename, CV_LOAD_IMAGE_GRAYSCALE);//CV_LOAD_IMAGE_GRAYSCALE加载灰度图
		IdenNumer2(src0);
	}
}
void MyQt::faceDeteBtn_clicked()
{
	QByteArray ba = fileNm.toLatin1();//合起来写会有乱码
	char* filename = ba.data();
	IplImage *faceImage = cvLoadImage(filename, 1);	// 重新加载原图用于人脸
	facerecog(faceImage);
}
void MyQt::openImage_clicked()
{
	fileNm = QFileDialog::getOpenFileName(this, tr("open iamge"),//对话框名称
		".",//默认打开文件位置“.”文件目录"/"根目录
		tr("image files(*.jpg *.png *.bmp)"));
	
	QTextCodec *code = QTextCodec::codecForName("utf8");
	string name = code->fromUnicode(fileNm).data();
	fileNm = QString::fromStdString(name);
	
	if (fileNm.isEmpty())
	{
		return;
	}
	else
	{
		QImage* qimg = new QImage;
		if (!(qimg->load(fileNm))) //加载图像
		{
			QMessageBox::information(this,
				tr("打开图像失败"),
				tr("打开图像失败!"));
			delete qimg;
			return;
		}
		QByteArray ba = fileNm.toLatin1();//合起来写会有乱码
		char* filename = ba.data();
		
		clickCirleBtnCount = 0;//
		//删除label和移除布局
		for (int i = 0; i < nClusters; i++)
		{
			QWidget *sizeBtn = ui.centralWidget->findChild<QWidget *>(tr("color%1").arg(i));
			sizeBtn->deleteLater();
			delete sizeBtn;
		}
		QVBoxLayout *sizeBtn = ui.centralWidget->findChild<QVBoxLayout *>("colorLayout");
		sizeBtn->deleteLater();

		*qimg = qimg->scaled(ui.label->width(), ui.label->height());//读取label尺寸，改变图片大小
		ui.label->setPixmap(QPixmap::fromImage(*qimg));//绘制图片
		
	}
}
void MyQt::submitClusters_clicked()
{
	QByteArray ba = fileNm.toLatin1();//合起来写会有乱码
	char* filename = ba.data();
	img = cvLoadImage(filename, 1);
	pResult = cvCreateImage(cvGetSize(img), img->depth, 3);
	samples = cvCreateMat((img->width)*(img->height), 1, CV_32FC3);//创建样本矩阵，CV_32FC3代表位浮点通道（彩色图像）
	clusters = cvCreateMat((img->width)*(img->height), 1, CV_32SC1);//创建类别标记矩阵，CV_32SF1代表位整型通道
	histogram(img);//彩色直方图
	
	int temp;
	QString strname = ui.nClusters->text();//得到输入框文本
	temp = nClusters;
	nClusters = strname.toInt();//得到聚类数
	ui.verticalSlider->setMaximum(nClusters-1);//设置滑动条跨度
	centers = cvCreateMat(nClusters, 1, CV_32FC3);//创建类心矩阵
	get_RGB(img, samples);  //获取图像的RGB值，并存到samples里面
	cvKMeans2(samples, nClusters, clusters, cvTermCriteria(CV_TERMCRIT_EPS, 100, 1.0), 1, 0, 0, centers, 0);//做聚类
	for (int i = 0; i < temp; i++)
	{
		QWidget *sizeBtn = ui.centralWidget->findChild<QWidget *>(tr("color%1").arg(i));
		sizeBtn->deleteLater();
		delete sizeBtn;
	}
	QVBoxLayout *sizeBtn = ui.centralWidget->findChild<QVBoxLayout *>("colorLayout");
	sizeBtn->deleteLater();
	delete sizeBtn;

	center_RGB(nClusters, centers);//获取每个聚类中心的RGB值，用来判断颜色
	ui.tip->setText(QString::fromLocal8Bit("请在上方选择颜色"));//直接set中文会出现乱码
}
void MyQt::displayCluster_clicked()
{
	val_user = ui.verticalSlider->value();//得到用户选择的index
	point_color(clusters, img, pResult, ui.labelDisplay);//输入想要显示的颜色，输出此颜色的区域
}


