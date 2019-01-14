/********************************************************************************
** Form generated from reading UI file 'myqt.ui'
**
** Created by: Qt User Interface Compiler version 5.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MYQT_H
#define UI_MYQT_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MyQtClass
{
public:
    QWidget *centralWidget;
    QLabel *label;
    QSlider *verticalSlider;
    QLabel *label_2;
    QLabel *histogram;
    QLabel *tip;
    QLabel *labelPane;
    QLabel *numberText;
    QPushButton *faceDeteBtn;
    QPushButton *numberDeteBtn;
    QPushButton *circleDeteBtn;
    QPushButton *openImage;
    QPushButton *submitClusters;
    QLineEdit *nClusters;
    QLabel *labelDisplay;
    QPushButton *displayCluster;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MyQtClass)
    {
        if (MyQtClass->objectName().isEmpty())
            MyQtClass->setObjectName(QStringLiteral("MyQtClass"));
        MyQtClass->resize(1003, 696);
        MyQtClass->setStyleSheet(QStringLiteral("background-color: #BFDAAF;"));
        centralWidget = new QWidget(MyQtClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        label = new QLabel(centralWidget);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(60, 20, 351, 261));
        label->setStyleSheet(QStringLiteral("background-color: #86AC97;"));
        verticalSlider = new QSlider(centralWidget);
        verticalSlider->setObjectName(QStringLiteral("verticalSlider"));
        verticalSlider->setGeometry(QRect(470, 20, 31, 261));
        verticalSlider->setMaximum(9);
        verticalSlider->setSingleStep(3);
        verticalSlider->setPageStep(0);
        verticalSlider->setValue(0);
        verticalSlider->setOrientation(Qt::Vertical);
        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(220, 360, 71, 51));
        label_2->setLayoutDirection(Qt::LeftToRight);
        label_2->setStyleSheet(QString::fromUtf8("font: 75 10pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";"));
        label_2->setTextFormat(Qt::AutoText);
        label_2->setAlignment(Qt::AlignCenter);
        histogram = new QLabel(centralWidget);
        histogram->setObjectName(QStringLiteral("histogram"));
        histogram->setGeometry(QRect(30, 400, 481, 231));
        histogram->setStyleSheet(QStringLiteral("background-color: #86AC97;"));
        tip = new QLabel(centralWidget);
        tip->setObjectName(QStringLiteral("tip"));
        tip->setGeometry(QRect(480, 300, 121, 31));
        tip->setStyleSheet(QString::fromUtf8("font: 75 11pt \"\345\276\256\350\275\257\351\233\205\351\273\221 Light\";"));
        tip->setAlignment(Qt::AlignCenter);
        labelPane = new QLabel(centralWidget);
        labelPane->setObjectName(QStringLiteral("labelPane"));
        labelPane->setGeometry(QRect(500, 10, 71, 281));
        numberText = new QLabel(centralWidget);
        numberText->setObjectName(QStringLiteral("numberText"));
        numberText->setGeometry(QRect(630, 440, 351, 121));
        numberText->setMaximumSize(QSize(351, 231));
        numberText->setStyleSheet(QString::fromUtf8("font: 75 11pt \"\345\276\256\350\275\257\351\233\205\351\273\221 Light\";\n"
"background-color: rgb(255, 255, 255);"));
        numberText->setAlignment(Qt::AlignCenter);
        faceDeteBtn = new QPushButton(centralWidget);
        faceDeteBtn->setObjectName(QStringLiteral("faceDeteBtn"));
        faceDeteBtn->setGeometry(QRect(750, 300, 101, 31));
        faceDeteBtn->setStyleSheet(QString::fromUtf8("background-color:#56416C;\n"
"color:white;\n"
"font: 75 11pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";"));
        numberDeteBtn = new QPushButton(centralWidget);
        numberDeteBtn->setObjectName(QStringLiteral("numberDeteBtn"));
        numberDeteBtn->setGeometry(QRect(750, 380, 101, 31));
        numberDeteBtn->setStyleSheet(QString::fromUtf8("background-color:#56416C;\n"
"color:white;\n"
"font: 75 11pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";"));
        circleDeteBtn = new QPushButton(centralWidget);
        circleDeteBtn->setObjectName(QStringLiteral("circleDeteBtn"));
        circleDeteBtn->setGeometry(QRect(870, 300, 101, 31));
        circleDeteBtn->setStyleSheet(QString::fromUtf8("background-color:#56416C;\n"
"color:white;\n"
"font: 75 11pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";"));
        openImage = new QPushButton(centralWidget);
        openImage->setObjectName(QStringLiteral("openImage"));
        openImage->setGeometry(QRect(30, 300, 121, 41));
        openImage->setStyleSheet(QString::fromUtf8("background-color:#56416C;\n"
"color:white;\n"
"font: 75 11pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";"));
        submitClusters = new QPushButton(centralWidget);
        submitClusters->setObjectName(QStringLiteral("submitClusters"));
        submitClusters->setGeometry(QRect(350, 300, 111, 41));
        submitClusters->setStyleSheet(QString::fromUtf8("background-color: #FFF7C0;\n"
"color: rgb(167, 46, 37);\n"
"font: 75 11pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";"));
        nClusters = new QLineEdit(centralWidget);
        nClusters->setObjectName(QStringLiteral("nClusters"));
        nClusters->setGeometry(QRect(190, 300, 121, 41));
        nClusters->setStyleSheet(QStringLiteral("background-color: rgb(255, 255, 255);"));
        nClusters->setAlignment(Qt::AlignCenter);
        labelDisplay = new QLabel(centralWidget);
        labelDisplay->setObjectName(QStringLiteral("labelDisplay"));
        labelDisplay->setGeometry(QRect(630, 20, 351, 261));
        labelDisplay->setStyleSheet(QStringLiteral("background-color: #86AC97;"));
        displayCluster = new QPushButton(centralWidget);
        displayCluster->setObjectName(QStringLiteral("displayCluster"));
        displayCluster->setGeometry(QRect(630, 300, 101, 31));
        displayCluster->setStyleSheet(QString::fromUtf8("background-color:#56416C;\n"
"color:white;\n"
"font: 75 11pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";"));
        MyQtClass->setCentralWidget(centralWidget);
        label->raise();
        verticalSlider->raise();
        label_2->raise();
        histogram->raise();
        tip->raise();
        labelPane->raise();
        numberText->raise();
        faceDeteBtn->raise();
        numberDeteBtn->raise();
        circleDeteBtn->raise();
        submitClusters->raise();
        openImage->raise();
        nClusters->raise();
        labelDisplay->raise();
        displayCluster->raise();
        menuBar = new QMenuBar(MyQtClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1003, 23));
        MyQtClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MyQtClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MyQtClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MyQtClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MyQtClass->setStatusBar(statusBar);

        retranslateUi(MyQtClass);

        QMetaObject::connectSlotsByName(MyQtClass);
    } // setupUi

    void retranslateUi(QMainWindow *MyQtClass)
    {
        MyQtClass->setWindowTitle(QApplication::translate("MyQtClass", "MyQt", 0));
        label->setText(QString());
        label_2->setText(QApplication::translate("MyQtClass", "<html><head/><body><p>\351\242\234\350\211\262\345\210\206\345\270\203\345\233\276</p></body></html>", 0));
        histogram->setText(QString());
#ifndef QT_NO_TOOLTIP
        tip->setToolTip(QApplication::translate("MyQtClass", "help", 0));
#endif // QT_NO_TOOLTIP
        tip->setText(QString());
        labelPane->setText(QString());
        numberText->setText(QApplication::translate("MyQtClass", "\350\257\206\345\210\253\345\207\272\347\232\204\346\225\260\345\255\227", 0));
        faceDeteBtn->setText(QApplication::translate("MyQtClass", "\344\272\272\350\204\270\350\257\206\345\210\253", 0));
        numberDeteBtn->setText(QApplication::translate("MyQtClass", "\346\225\260\345\255\227\350\257\206\345\210\253", 0));
        circleDeteBtn->setText(QApplication::translate("MyQtClass", "\345\234\206\345\275\242/\345\244\232\350\276\271\345\275\242", 0));
        openImage->setText(QApplication::translate("MyQtClass", "\346\211\223\345\274\200\345\233\276\347\211\207", 0));
        submitClusters->setText(QApplication::translate("MyQtClass", "\345\210\206\346\236\220\351\242\234\350\211\262", 0));
        nClusters->setPlaceholderText(QApplication::translate("MyQtClass", "\345\244\247\347\272\246\345\207\240\347\247\215\351\242\234\350\211\262", 0));
        labelDisplay->setText(QString());
        displayCluster->setText(QApplication::translate("MyQtClass", "\351\242\234\350\211\262\350\257\206\345\210\253", 0));
    } // retranslateUi

};

namespace Ui {
    class MyQtClass: public Ui_MyQtClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MYQT_H
