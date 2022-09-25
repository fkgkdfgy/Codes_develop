#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>

class MainWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MainWidget(QWidget *parent = nullptr);

    QPushButton * button;
    QLabel * label;
signals:

public slots:
// 设置槽函数
    void changeText(){
        label->setText("Hello New World");
    }
};

#endif // MAINWIDGET_H
