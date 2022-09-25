#ifndef INPUTWIDGET_H
#define INPUTWIDGET_H

#include <QWidget>

namespace Ui {
class InputWidget;
}

class InputWidget : public QWidget
{
    Q_OBJECT

public:
    explicit InputWidget(QWidget *parent = nullptr);
    ~InputWidget();

signals:
    void signal_show_these_parameters(double,double,double);

private slots:
    void on_add_button_clicked();

    void on_clear_button_clicked();

    void on_exit_button_clicked();

private:
    Ui::InputWidget *ui;
};

#endif // INPUTWIDGET_H
