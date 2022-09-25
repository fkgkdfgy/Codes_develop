#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "infomanager.h"
#include "declare.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_addButton_clicked();

    void on_exitButton_clicked();

    void on_searchButton_clicked();

    void on_deleteButton_clicked();

    void on_averageButton_clicked();

    void on_action_triggered();

private:
    Ui::MainWindow *ui;
    declare * d;
    InfoManager info_manager;
};

#endif // MAINWINDOW_H
