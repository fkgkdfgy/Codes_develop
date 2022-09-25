#include "declare.h"
#include "ui_declare.h"

declare::declare(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::declare)
{
    ui->setupUi(this);
}

declare::~declare()
{
    delete ui;
}

void declare::on_buttonBox_accepted()
{
    close();
}

void declare::on_buttonBox_rejected()
{
    close();
}
