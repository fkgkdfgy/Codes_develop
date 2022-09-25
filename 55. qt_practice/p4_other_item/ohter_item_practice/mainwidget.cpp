#include <string>
#include "mainwidget.h"
#include "ui_mainwidget.h"

MainWidget::MainWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWidget)
{
    ui->setupUi(this);
    input_widget = new InputWidget;
    connect(input_widget,&InputWidget::signal_show_these_parameters,this,&MainWidget::slot_show_these_parameters);

    zlabel = new QLabel(this);
    zlabel->setObjectName(QString::fromUtf8("ylabel"));
    zlabel->setGeometry(QRect(250, 20, 75, 23));
    zlabel->setTextFormat(Qt::AutoText);
    zlabel->setText(QString::fromStdString("AAA"));
    zlabel->setVisible(true);

}

MainWidget::~MainWidget()
{
    delete ui;
}

void MainWidget::on_pushButton_clicked()
{
    input_widget->show();
}

void MainWidget::on_pushButton_2_clicked()
{
    QApplication::exit();
}

void MainWidget::slot_show_these_parameters(double x,double y, double theta)
{
    ui->textBrowser->clear();
    QString tmp_x ="X:"+ QString::fromStdString(std::to_string(x));
    QString tmp_y ="Y:"+ QString::fromStdString(std::to_string(y));
    QString tmp_theta ="Theta:"+ QString::fromStdString(std::to_string(theta));
    ui->textBrowser->append(tmp_x);
    ui->textBrowser->append(tmp_y);
    ui->textBrowser->append(tmp_theta);
}
