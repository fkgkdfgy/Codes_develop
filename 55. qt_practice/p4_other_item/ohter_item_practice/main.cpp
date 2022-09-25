// p4 的主要目的是联系一些之前没有注意到的东西
// 1.两个widget之间的交互 (自己定义signals) ok
// 2.在没有UI的情况下如何进行手工的编辑 ok
// 3.label->to*(ok) 来保证输入的正确性 ok

#include <QApplication>
#include "mainwidget.h"

int main(int argc,char ** argv)
{
    QApplication app(argc,argv);
    MainWidget w;
    w.show();
    return app.exec();
}
