/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QGridLayout *gridLayout;
    QLabel *labelN;
    QSpinBox *spinBoxN;
    QRadioButton *radioNaive;
    QRadioButton *radioMemo;
    QPushButton *btnDraw;
    QCheckBox *checkBoxAnimate;
    QPushButton *btnStep;
    QPushButton *btnSkip;
    QSpacerItem *horizontalSpacer;
    QHBoxLayout *mainLayout;
    QGraphicsView *graphicsView;
    QTextEdit *infoText;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1000, 640);
        MainWindow->setStyleSheet(QString::fromUtf8("\n"
"    /* Light modern background for central area */\n"
"    QMainWindow { background: #000000; }\n"
"    QLabel { color: #333; }\n"
"    QSpinBox, QRadioButton, QCheckBox { font: 10pt \"Segoe UI\"; }\n"
"    QGraphicsView { background: white; border-radius: 8px; border: 1px solid #e0e6ea; }\n"
"   "));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        gridLayout = new QGridLayout(centralwidget);
        gridLayout->setObjectName("gridLayout");
        labelN = new QLabel(centralwidget);
        labelN->setObjectName("labelN");

        gridLayout->addWidget(labelN, 0, 0, 1, 1);

        spinBoxN = new QSpinBox(centralwidget);
        spinBoxN->setObjectName("spinBoxN");
        QSizePolicy sizePolicy(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(spinBoxN->sizePolicy().hasHeightForWidth());
        spinBoxN->setSizePolicy(sizePolicy);
        spinBoxN->setMinimumSize(QSize(75, 0));
        spinBoxN->setMinimum(0);
        spinBoxN->setMaximum(40);
        spinBoxN->setValue(6);

        gridLayout->addWidget(spinBoxN, 0, 1, 1, 1);

        radioNaive = new QRadioButton(centralwidget);
        radioNaive->setObjectName("radioNaive");

        gridLayout->addWidget(radioNaive, 0, 2, 1, 1);

        radioMemo = new QRadioButton(centralwidget);
        radioMemo->setObjectName("radioMemo");

        gridLayout->addWidget(radioMemo, 0, 3, 1, 1);

        btnDraw = new QPushButton(centralwidget);
        btnDraw->setObjectName("btnDraw");

        gridLayout->addWidget(btnDraw, 0, 4, 1, 1);

        checkBoxAnimate = new QCheckBox(centralwidget);
        checkBoxAnimate->setObjectName("checkBoxAnimate");
        checkBoxAnimate->setChecked(true);

        gridLayout->addWidget(checkBoxAnimate, 0, 5, 1, 1);

        btnStep = new QPushButton(centralwidget);
        btnStep->setObjectName("btnStep");

        gridLayout->addWidget(btnStep, 0, 6, 1, 1);

        btnSkip = new QPushButton(centralwidget);
        btnSkip->setObjectName("btnSkip");

        gridLayout->addWidget(btnSkip, 0, 7, 1, 1);

        horizontalSpacer = new QSpacerItem(343, 13, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        gridLayout->addItem(horizontalSpacer, 0, 8, 1, 1);

        mainLayout = new QHBoxLayout();
        mainLayout->setObjectName("mainLayout");
        graphicsView = new QGraphicsView(centralwidget);
        graphicsView->setObjectName("graphicsView");
        graphicsView->setMinimumSize(QSize(400, 360));
        graphicsView->setStyleSheet(QString::fromUtf8("color: black;\n"
""));

        mainLayout->addWidget(graphicsView);

        infoText = new QTextEdit(centralwidget);
        infoText->setObjectName("infoText");
        infoText->setMinimumSize(QSize(320, 360));
        infoText->setStyleSheet(QString::fromUtf8("\n"
"      background-color: #FFFFFF;\n"
"      color: #000000;\n"
"    "));

        mainLayout->addWidget(infoText);


        gridLayout->addLayout(mainLayout, 1, 0, 1, 9);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1000, 21));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "Fib Call Tree Visualizer", nullptr));
        labelN->setText(QCoreApplication::translate("MainWindow", "n", nullptr));
        radioNaive->setText(QCoreApplication::translate("MainWindow", "Naive", nullptr));
        radioMemo->setText(QCoreApplication::translate("MainWindow", "Memoized", nullptr));
        btnDraw->setText(QCoreApplication::translate("MainWindow", "Draw", nullptr));
        checkBoxAnimate->setText(QCoreApplication::translate("MainWindow", "Animate (step)", nullptr));
        btnStep->setText(QCoreApplication::translate("MainWindow", "Step", nullptr));
        btnSkip->setText(QCoreApplication::translate("MainWindow", "Skip", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
