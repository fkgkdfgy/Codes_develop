#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QLabel>
#include "inputwidget.h"
namespace Ui {
class MainWidget;
}

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MainWidget(QWidget *parent = nullptr);
    ~MainWidget();

private:
    Ui::MainWidget *ui;
    InputWidget * input_widget;
    QLabel *zlabel;

public slots:

    void slot_show_these_parameters(double,double,double);

private slots:
    void on_pushButton_clicked();


    void on_pushButton_2_clicked();
};

#endif // MAINWIDGET_H
