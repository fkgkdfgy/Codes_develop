/********************************************************************************
** Form generated from reading UI file 'pclrender.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PCLRENDER_H
#define UI_PCLRENDER_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QWidget>
#include "QVTKWidget.h"

QT_BEGIN_NAMESPACE

class Ui_Form
{
public:
    QVTKWidget *qvtkWidget;
    QSlider *pointsize_slider;
    QLabel *label;
    QSpinBox *spinBox;
    QLabel *label_2;
    QSlider *pointtype_slider;
    QFrame *line;
    QFrame *line_2;
    QLabel *label_3;
    QFrame *line_3;
    QSlider *red_slider;
    QFrame *line_4;
    QSlider *green_slider;
    QLabel *label_4;

    void setupUi(QWidget *Form)
    {
        if (Form->objectName().isEmpty())
            Form->setObjectName(QString::fromUtf8("Form"));
        Form->resize(1014, 664);
        qvtkWidget = new QVTKWidget(Form);
        qvtkWidget->setObjectName(QString::fromUtf8("qvtkWidget"));
        qvtkWidget->setGeometry(QRect(200, 30, 801, 621));
        pointsize_slider = new QSlider(Form);
        pointsize_slider->setObjectName(QString::fromUtf8("pointsize_slider"));
        pointsize_slider->setGeometry(QRect(20, 80, 160, 22));
        pointsize_slider->setOrientation(Qt::Horizontal);
        label = new QLabel(Form);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(20, 40, 71, 31));
        QFont font;
        font.setFamily(QString::fromUtf8("Agency FB"));
        font.setPointSize(14);
        label->setFont(font);
        spinBox = new QSpinBox(Form);
        spinBox->setObjectName(QString::fromUtf8("spinBox"));
        spinBox->setGeometry(QRect(20, 370, 91, 31));
        label_2 = new QLabel(Form);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(20, 120, 71, 31));
        label_2->setFont(font);
        pointtype_slider = new QSlider(Form);
        pointtype_slider->setObjectName(QString::fromUtf8("pointtype_slider"));
        pointtype_slider->setGeometry(QRect(20, 160, 160, 22));
        pointtype_slider->setOrientation(Qt::Horizontal);
        line = new QFrame(Form);
        line->setObjectName(QString::fromUtf8("line"));
        line->setGeometry(QRect(20, 110, 118, 3));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);
        line_2 = new QFrame(Form);
        line_2->setObjectName(QString::fromUtf8("line_2"));
        line_2->setGeometry(QRect(20, 190, 118, 3));
        line_2->setFrameShape(QFrame::HLine);
        line_2->setFrameShadow(QFrame::Sunken);
        label_3 = new QLabel(Form);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(20, 200, 71, 31));
        label_3->setFont(font);
        line_3 = new QFrame(Form);
        line_3->setObjectName(QString::fromUtf8("line_3"));
        line_3->setGeometry(QRect(20, 270, 118, 3));
        line_3->setFrameShape(QFrame::HLine);
        line_3->setFrameShadow(QFrame::Sunken);
        red_slider = new QSlider(Form);
        red_slider->setObjectName(QString::fromUtf8("red_slider"));
        red_slider->setGeometry(QRect(20, 240, 160, 22));
        red_slider->setOrientation(Qt::Horizontal);
        line_4 = new QFrame(Form);
        line_4->setObjectName(QString::fromUtf8("line_4"));
        line_4->setGeometry(QRect(20, 350, 118, 3));
        line_4->setFrameShape(QFrame::HLine);
        line_4->setFrameShadow(QFrame::Sunken);
        green_slider = new QSlider(Form);
        green_slider->setObjectName(QString::fromUtf8("green_slider"));
        green_slider->setGeometry(QRect(20, 320, 160, 22));
        green_slider->setOrientation(Qt::Horizontal);
        label_4 = new QLabel(Form);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(20, 280, 71, 31));
        label_4->setFont(font);

        retranslateUi(Form);

        QMetaObject::connectSlotsByName(Form);
    } // setupUi

    void retranslateUi(QWidget *Form)
    {
        Form->setWindowTitle(QCoreApplication::translate("Form", "Form", nullptr));
        label->setText(QCoreApplication::translate("Form", "PointSize", nullptr));
        label_2->setText(QCoreApplication::translate("Form", "PointType", nullptr));
        label_3->setText(QCoreApplication::translate("Form", "Red", nullptr));
        label_4->setText(QCoreApplication::translate("Form", "Green", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Form: public Ui_Form {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PCLRENDER_H
