 #pragma once
#include "myqt.h"
#include <QtWidgets/QApplication>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;
int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	MyQt w;
	w.show();
	return a.exec();
}