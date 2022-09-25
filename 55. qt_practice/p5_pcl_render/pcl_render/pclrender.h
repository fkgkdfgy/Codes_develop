#ifndef PCLRENDER_H
#define PCLRENDER_H

#include <QWidget>
#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QWidget>
#include "QVTKWidget.h"
#include <pcl/visualization/pcl_visualizer.h>

using namespace pcl::visualization;

class PCLRender : public QWidget
{
    Q_OBJECT
public:
    explicit PCLRender(QWidget *parent = nullptr);
    QVTKWidget * qvtkWidget;
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

	boost::shared_ptr<PCLVisualizer> viewer;// PCL¿ÉÊÓ»¯Æ÷

	void setupUi(QWidget *Form);
	void retranslateUi(QWidget *Form);

signals:

};

#endif // PCLRENDER_H
