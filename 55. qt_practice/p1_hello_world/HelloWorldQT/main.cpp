
// 内容主要来自https://www.cnblogs.com/suvvm/p/10643826.html

#include <QApplication>
#include <QWidget>
#include <QLabel>

int main(int argc,char ** argv)
{
    //在程序编译完成后会生成一个.exe后缀的可执行文件
    //argc是命令行参数个数
    //argv[0]记录程序名
    //argv[1]~argv[argc-1]记录的输入参数
    //argc argv是标识符，可以修改名称
    //main()函数有argc和argv或没有，即int main() 或void main，都是合法的。
    QApplication app(argc, argv);
    //应用程序抽象类
    //使用在argv中的argc个命令行参数构造一个名为app的QApplication对象。
    //QApplication 类管理图形用户界面应用程序的控制流和主要设置
    //任何一个使用Qt的图形用户界面应用程序，都存在一个QApplication对象

    QWidget w;
    //QWidget类是所有用户界面对象的基类
    //在这里可以把它看作一个窗口，构造了一个窗口对象
    w.setWindowTitle("Hello World!");
    //将窗口的标题设为Hello World!
    QLabel textLabel;
    //构造一个标签
    textLabel.resize(100,15);
    //设定标签宽40高15
    textLabel.move(150, 170);
    //将标签移动到距离左上角水平100px垂直130px的位置
    textLabel.setText("Hello World!");
    //标签的初始显示字符串NULL
    // setParent 之后就会在Qwidget w 中进行显示了
    textLabel.setParent(&w);

    textLabel.show();
    w.show();
    //显示窗体
    return app.exec();
    //exec()函数有一个消息循环，在调用之后，程序就被锁定。等待窗口的关闭
}
