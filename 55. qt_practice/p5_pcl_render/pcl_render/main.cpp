#include <QApplication>
#include "pclrender.h"

int main(int argc,char ** argv)
{
    QApplication app(argc,argv);

    PCLRender w;
    w.show();
    return app.exec();
}
