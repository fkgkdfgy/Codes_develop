#include "mainwidget.h"

MainWidget::MainWidget(QWidget *parent) : QWidget(parent)
{
    // 确定新建的窗口大小
    resize(400,400);
    // 创建一个Label 他的Parent 是MainWidget
    label = new QLabel("Hello World",this);
    label->setGeometry(150,100,120,50);
    // 创建一个button 同样Paretn 指向 MainWidget
    button = new QPushButton("change",this);
    button->setGeometry(150,175,120,50);

}
