/********************************************************************************
** Form generated from reading UI file 'main_window.ui'
**
** Created by: Qt User Interface Compiler version 5.12.7
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAIN_WINDOW_H
#define UI_MAIN_WINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "glwidget.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionQuit;
    QAction *actionLoad;
    QAction *actionLoad_Specular;
    QAction *actionLoad_Diffuse;
    QWidget *Widget;
    QHBoxLayout *horizontalLayout;
    GLWidget *glwidget;
    QVBoxLayout *Configuration;
    QSpacerItem *Spacer;
    QGroupBox *TreeOptions;
    QLabel *label;
    QSpinBox *spinBox;
    QLabel *label_2;
    QDoubleSpinBox *doubleSpinBox;
    QSpinBox *spinBox_2;
    QLabel *label_3;
    QComboBox *comboBox;
    QLabel *label_4;
    QLabel *label_5;
    QDoubleSpinBox *doubleSpinBox_2;
    QLabel *label_6;
    QDoubleSpinBox *doubleSpinBox_3;
    QGroupBox *RenderOptions;
    QLabel *Label_NumFaces;
    QLabel *Label_Faces;
    QLabel *Label_Vertices;
    QLabel *Label_NumVertices;
    QLabel *Label_Framerate;
    QLabel *Label_NumFramerate;
    QMenuBar *menuBar;
    QMenu *menuFile;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(828, 638);
        MainWindow->setMinimumSize(QSize(827, 618));
        MainWindow->setBaseSize(QSize(600, 600));
        actionQuit = new QAction(MainWindow);
        actionQuit->setObjectName(QString::fromUtf8("actionQuit"));
        actionLoad = new QAction(MainWindow);
        actionLoad->setObjectName(QString::fromUtf8("actionLoad"));
        actionLoad_Specular = new QAction(MainWindow);
        actionLoad_Specular->setObjectName(QString::fromUtf8("actionLoad_Specular"));
        actionLoad_Diffuse = new QAction(MainWindow);
        actionLoad_Diffuse->setObjectName(QString::fromUtf8("actionLoad_Diffuse"));
        Widget = new QWidget(MainWindow);
        Widget->setObjectName(QString::fromUtf8("Widget"));
        Widget->setMinimumSize(QSize(827, 0));
        Widget->setBaseSize(QSize(600, 600));
        horizontalLayout = new QHBoxLayout(Widget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        glwidget = new GLWidget(Widget);
        glwidget->setObjectName(QString::fromUtf8("glwidget"));
        glwidget->setMinimumSize(QSize(600, 600));
        glwidget->setMaximumSize(QSize(16777215, 16777215));
        glwidget->setBaseSize(QSize(600, 600));

        horizontalLayout->addWidget(glwidget);

        Configuration = new QVBoxLayout();
        Configuration->setSpacing(6);
        Configuration->setObjectName(QString::fromUtf8("Configuration"));
        Spacer = new QSpacerItem(50, 50, QSizePolicy::Minimum, QSizePolicy::Maximum);

        Configuration->addItem(Spacer);

        TreeOptions = new QGroupBox(Widget);
        TreeOptions->setObjectName(QString::fromUtf8("TreeOptions"));
        TreeOptions->setMinimumSize(QSize(200, 0));
        TreeOptions->setMaximumSize(QSize(200, 16777215));
        label = new QLabel(TreeOptions);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(10, 30, 121, 31));
        label->setWordWrap(true);
        spinBox = new QSpinBox(TreeOptions);
        spinBox->setObjectName(QString::fromUtf8("spinBox"));
        spinBox->setGeometry(QRect(140, 30, 51, 31));
        spinBox->setMinimum(2);
        spinBox->setMaximum(10000);
        spinBox->setValue(10);
        label_2 = new QLabel(TreeOptions);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setEnabled(true);
        label_2->setGeometry(QRect(9, 250, 121, 31));
        label_2->setWordWrap(true);
        doubleSpinBox = new QDoubleSpinBox(TreeOptions);
        doubleSpinBox->setObjectName(QString::fromUtf8("doubleSpinBox"));
        doubleSpinBox->setEnabled(true);
        doubleSpinBox->setGeometry(QRect(80, 250, 111, 31));
        doubleSpinBox->setDecimals(9);
        doubleSpinBox->setMinimum(-10000.000000000000000);
        doubleSpinBox->setMaximum(10000.000000000000000);
        doubleSpinBox->setSingleStep(0.100000000000000);
        doubleSpinBox->setStepType(QAbstractSpinBox::DefaultStepType);
        doubleSpinBox->setValue(50.000000000000000);
        spinBox_2 = new QSpinBox(TreeOptions);
        spinBox_2->setObjectName(QString::fromUtf8("spinBox_2"));
        spinBox_2->setEnabled(false);
        spinBox_2->setGeometry(QRect(140, 70, 51, 31));
        spinBox_2->setMinimum(1);
        spinBox_2->setMaximum(3);
        spinBox_2->setValue(1);
        label_3 = new QLabel(TreeOptions);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setEnabled(false);
        label_3->setGeometry(QRect(10, 70, 121, 31));
        comboBox = new QComboBox(TreeOptions);
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->setObjectName(QString::fromUtf8("comboBox"));
        comboBox->setGeometry(QRect(10, 130, 181, 25));
        label_4 = new QLabel(TreeOptions);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(10, 110, 181, 21));
        label_5 = new QLabel(TreeOptions);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setEnabled(true);
        label_5->setGeometry(QRect(9, 280, 121, 31));
        label_5->setWordWrap(true);
        doubleSpinBox_2 = new QDoubleSpinBox(TreeOptions);
        doubleSpinBox_2->setObjectName(QString::fromUtf8("doubleSpinBox_2"));
        doubleSpinBox_2->setEnabled(true);
        doubleSpinBox_2->setGeometry(QRect(80, 280, 111, 31));
        doubleSpinBox_2->setDecimals(9);
        doubleSpinBox_2->setMinimum(-10000.000000000000000);
        doubleSpinBox_2->setMaximum(10000.000000000000000);
        doubleSpinBox_2->setSingleStep(1.000000000000000);
        doubleSpinBox_2->setValue(100.000000000000000);
        label_6 = new QLabel(TreeOptions);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setEnabled(true);
        label_6->setGeometry(QRect(9, 330, 121, 31));
        label_6->setWordWrap(true);
        doubleSpinBox_3 = new QDoubleSpinBox(TreeOptions);
        doubleSpinBox_3->setObjectName(QString::fromUtf8("doubleSpinBox_3"));
        doubleSpinBox_3->setEnabled(true);
        doubleSpinBox_3->setGeometry(QRect(80, 330, 111, 31));
        doubleSpinBox_3->setDecimals(9);
        doubleSpinBox_3->setMaximum(10000.000000000000000);
        doubleSpinBox_3->setSingleStep(0.050000000000000);
        doubleSpinBox_3->setValue(0.500000000000000);

        Configuration->addWidget(TreeOptions);

        RenderOptions = new QGroupBox(Widget);
        RenderOptions->setObjectName(QString::fromUtf8("RenderOptions"));
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(RenderOptions->sizePolicy().hasHeightForWidth());
        RenderOptions->setSizePolicy(sizePolicy);
        RenderOptions->setMaximumSize(QSize(200, 120));
        RenderOptions->setBaseSize(QSize(0, 100));
        Label_NumFaces = new QLabel(RenderOptions);
        Label_NumFaces->setObjectName(QString::fromUtf8("Label_NumFaces"));
        Label_NumFaces->setGeometry(QRect(90, 40, 91, 17));
        Label_Faces = new QLabel(RenderOptions);
        Label_Faces->setObjectName(QString::fromUtf8("Label_Faces"));
        Label_Faces->setGeometry(QRect(10, 40, 67, 17));
        Label_Vertices = new QLabel(RenderOptions);
        Label_Vertices->setObjectName(QString::fromUtf8("Label_Vertices"));
        Label_Vertices->setGeometry(QRect(10, 60, 67, 17));
        Label_NumVertices = new QLabel(RenderOptions);
        Label_NumVertices->setObjectName(QString::fromUtf8("Label_NumVertices"));
        Label_NumVertices->setGeometry(QRect(90, 60, 91, 17));
        Label_Framerate = new QLabel(RenderOptions);
        Label_Framerate->setObjectName(QString::fromUtf8("Label_Framerate"));
        Label_Framerate->setGeometry(QRect(10, 90, 71, 17));
        Label_NumFramerate = new QLabel(RenderOptions);
        Label_NumFramerate->setObjectName(QString::fromUtf8("Label_NumFramerate"));
        Label_NumFramerate->setGeometry(QRect(90, 90, 91, 17));

        Configuration->addWidget(RenderOptions);


        horizontalLayout->addLayout(Configuration);

        MainWindow->setCentralWidget(Widget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 828, 22));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        MainWindow->setMenuBar(menuBar);

        menuBar->addAction(menuFile->menuAction());
        menuFile->addAction(actionQuit);
        menuFile->addAction(actionLoad);

        retranslateUi(MainWindow);
        QObject::connect(glwidget, SIGNAL(SetFaces(QString)), Label_NumFaces, SLOT(setText(QString)));
        QObject::connect(glwidget, SIGNAL(SetVertices(QString)), Label_NumVertices, SLOT(setText(QString)));
        QObject::connect(glwidget, SIGNAL(SetFramerate(QString)), Label_NumFramerate, SLOT(setText(QString)));
        QObject::connect(doubleSpinBox, SIGNAL(valueChanged(double)), glwidget, SLOT(SetDistanceOffset(double)));
        QObject::connect(spinBox_2, SIGNAL(valueChanged(int)), glwidget, SLOT(SetLevelOfDetail(int)));
        QObject::connect(comboBox, SIGNAL(currentTextChanged(QString)), glwidget, SLOT(SetMethod(QString)));
        QObject::connect(spinBox, SIGNAL(valueChanged(int)), glwidget, SLOT(SetNumInstances(int)));
        QObject::connect(doubleSpinBox, SIGNAL(valueChanged(double)), glwidget, SLOT(SetDamping(double)));
        QObject::connect(doubleSpinBox_2, SIGNAL(valueChanged(double)), glwidget, SLOT(SetElasticity(double)));
        QObject::connect(doubleSpinBox_3, SIGNAL(valueChanged(double)), glwidget, SLOT(SetLength(double)));

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "It Just Works\342\204\242", nullptr));
        actionQuit->setText(QApplication::translate("MainWindow", "Quit", nullptr));
        actionLoad->setText(QApplication::translate("MainWindow", "Load Model", nullptr));
#ifndef QT_NO_TOOLTIP
        actionLoad->setToolTip(QApplication::translate("MainWindow", "Load Model", nullptr));
#endif // QT_NO_TOOLTIP
        actionLoad_Specular->setText(QApplication::translate("MainWindow", "Load Specular", nullptr));
        actionLoad_Diffuse->setText(QApplication::translate("MainWindow", "Load Diffuse", nullptr));
        TreeOptions->setTitle(QApplication::translate("MainWindow", "Options", nullptr));
        label->setText(QApplication::translate("MainWindow", "Num. particles", nullptr));
        label_2->setText(QApplication::translate("MainWindow", "Damping", nullptr));
        label_3->setText(QApplication::translate("MainWindow", "Particle System", nullptr));
        comboBox->setItemText(0, QApplication::translate("MainWindow", "Euler (Original)", nullptr));
        comboBox->setItemText(1, QApplication::translate("MainWindow", "Euler (Semi)", nullptr));
        comboBox->setItemText(2, QApplication::translate("MainWindow", "Verlet", nullptr));

        comboBox->setCurrentText(QApplication::translate("MainWindow", "Verlet", nullptr));
        label_4->setText(QApplication::translate("MainWindow", "Physics Method", nullptr));
        label_5->setText(QApplication::translate("MainWindow", "Elasticity", nullptr));
        label_6->setText(QApplication::translate("MainWindow", "Length", nullptr));
        RenderOptions->setTitle(QApplication::translate("MainWindow", "Object and Scene Information", nullptr));
        Label_NumFaces->setText(QApplication::translate("MainWindow", "0", nullptr));
        Label_Faces->setText(QApplication::translate("MainWindow", "Faces", nullptr));
        Label_Vertices->setText(QApplication::translate("MainWindow", "Vertices", nullptr));
        Label_NumVertices->setText(QApplication::translate("MainWindow", "0", nullptr));
        Label_Framerate->setText(QApplication::translate("MainWindow", "Framerate", nullptr));
        Label_NumFramerate->setText(QApplication::translate("MainWindow", "0", nullptr));
        menuFile->setTitle(QApplication::translate("MainWindow", "File", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAIN_WINDOW_H
