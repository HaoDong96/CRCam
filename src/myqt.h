#ifndef MYQT_H
#define MYQT_H
#include <opencv2/opencv.hpp>
#include <cv.h>
#include <highgui.hpp>
#include <QtWidgets/QMainWindow>
#include <QPainter>
#include <QMouseEvent>
#include <QFileDialog>
#include <QMessageBox>
#include <vector>
#include <QBoxlayout>
#include <QTextCodec>
#include <stack>
#include <iostream>  
#include <opencv/highgui.h>  
#include <fstream>
#include <sstream>
#include <math.h>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <cstdio>  
#include <cstdlib>  
#include <Windows.h>  
#include <vector>
#include <sstream>
#include <string>
#include "ui_myqt.h"
using namespace std;
using namespace cv;

#ifndef max
#define max(a,b,c) (a)>(b)?((a)>(c)?(a):(c)):((b)>(c)?(b):(c))
#endif

#ifndef min
#define min(a,b,c) (a)<(b)?((a)<(c)?(a):(c)):((b)<(c)?(b):(c))
#endif


class MyQt : public QMainWindow
{
	Q_OBJECT

public:
	MyQt(QWidget *parent = 0);
	
	
	void center_RGB(int nClusters, CvMat* centers);
	//void canny(IplImage *pGrayImage);
	void point_color(CvMat*clusters, IplImage *img, IplImage *pResult, MyQt label);
	void histogram(IplImage *img);
	void facerecog(IplImage *img);
	void IdenNumer2(IplImage* img);
	public slots:
	void openImage_clicked();
	void submitClusters_clicked();
	void displayCluster_clicked();
	void circleDeteBtn_clicked();
	void faceDeteBtn_clicked();
	void numberDeteBtn_clicked();
private:
	Ui::MyQtClass ui;
};
#endif // MYQT_H
