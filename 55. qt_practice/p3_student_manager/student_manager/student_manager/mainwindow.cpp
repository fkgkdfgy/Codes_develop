#include <string>
#include "mainwindow.h"
#include "ui_mainwindow.h"

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    d = new declare;
    d->exec();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_addButton_clicked()
{
    string name = ui->nameLine->text().toStdString();
    double cn_score = ui->cnLine->text().toDouble();
    double en_score = ui->enLine->text().toDouble();
    double math_score = ui->enLine->text().toDouble();

    InfoManager::StudentInfo tmp;
    tmp.name = name;
    tmp.en_score = en_score;
    tmp.cn_score = cn_score;
    tmp.math_score = math_score;

    info_manager.addStudentInfo(tmp);

    ui->nameLine->clear();
    ui->cnLine->clear();
    ui->enLine->clear();
    ui->mathLine->clear();


}

void MainWindow::on_exitButton_clicked()
{
    QApplication::exit();
}

void MainWindow::on_searchButton_clicked()
{
    string name = ui->nameLine->text().toStdString();
    int idx = info_manager.searchInfo(name);
    InfoManager::StudentInfo tmp;

    if(idx>=0)
    {
        tmp = info_manager.student_info[idx];
        ui->nameLine->setText(QString::fromStdString(name));
        ui->enLine->setText(QString::fromStdString(std::to_string(tmp.en_score)));
        ui->cnLine->setText(QString::fromStdString(std::to_string(tmp.cn_score)));
        ui->mathLine->setText(QString::fromStdString(std::to_string(tmp.math_score)));
    }
    else
    {
        QString not_found = "NOT FOUND";
        ui->nameLine->setText(not_found);
        ui->enLine->setText(not_found);
        ui->cnLine->setText(not_found);
        ui->mathLine->setText(not_found);
        QString tmp_string = "sorry, do not find this person";
        ui->textEdit->setText(tmp_string);
    }
}

void MainWindow::on_deleteButton_clicked()
{
    string name = ui->nameLine->text().toStdString();
    int idx = info_manager.searchInfo(name);
    InfoManager::StudentInfo tmp;
    if(idx>=0)
    {
        info_manager.student_info.erase(info_manager.student_info.begin()+idx);
    }
    else
    {
        QString tmp_string = "sorry, do not find this person";
        ui->textEdit->setText(tmp_string);
    }

}

void MainWindow::on_averageButton_clicked()
{
    if(info_manager.student_info.size() == 0)
    {
        QString no_info = "NO INFO";
        ui->textEdit->setText(no_info);
    }
    else
    {
        InfoManager::StudentInfo tmp = info_manager.avgInfo();
        ui->nameLine->setText(QString::fromStdString(tmp.name));
        ui->enLine->setText(QString::fromStdString(std::to_string(tmp.en_score)));
        ui->cnLine->setText(QString::fromStdString(std::to_string(tmp.cn_score)));
        ui->mathLine->setText(QString::fromStdString(std::to_string(tmp.math_score)));
        ui->textEdit->clear();
    }
}

void MainWindow::on_action_triggered()
{
    d->exec();
}
