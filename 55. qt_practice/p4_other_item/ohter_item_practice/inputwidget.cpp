#include "inputwidget.h"
#include "ui_inputwidget.h"

InputWidget::InputWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::InputWidget)
{
    ui->setupUi(this);
}

InputWidget::~InputWidget()
{
    delete ui;
}

void InputWidget::on_add_button_clicked()
{
    bool ok = false;
    double x =0.,y=0.,theta=0.;
    x = ui->xlineEdit->text().toDouble(&ok);
    if(!ok)return;
    y = ui->ylineEdit->text().toDouble(&ok);
    if(!ok)return;
    theta = ui->thetalineEdit->text().toDouble(&ok);
    if(!ok)return;

    emit signal_show_these_parameters(x,y,theta);
}

void InputWidget::on_clear_button_clicked()
{
    ui->xlineEdit->clear();
    ui->ylineEdit->clear();
    ui->thetalineEdit->clear();
}

void InputWidget::on_exit_button_clicked()
{
    hide();
}
