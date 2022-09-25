#include "mainwidget.h"
#include <QApplication>

int main(int argc, char **argv)
{
    QApplication app(argc,argv);

    MainWidget w;
    w.show();

    // 在QObject类中有一个connect函数其中要求传入四个参数，
    // 第一个参数为被用户操作的对象（发出信号的对象当前为我们的change按钮），
    // 第二个参数是一个被操作信号（当前为按钮被点击clicked()），
    // 第三个参数是我们要进行操作的对象（接受信号的对象当前为我们的主窗体w），
    // 第四个参数为对应的槽函数（这就需要我们自己设计一个槽函数了，我们将这个槽函数命名为changeText）。
    QObject::connect(w.button,SIGNAL(clicked()),&w,SLOT(changeText()));

    return app.exec();
}
