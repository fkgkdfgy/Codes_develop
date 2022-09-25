#ifndef DECLARE_H
#define DECLARE_H

#include <QDialog>

namespace Ui {
class declare;
}

class declare : public QDialog
{
    Q_OBJECT

public:
    explicit declare(QWidget *parent = nullptr);
    ~declare();

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::declare *ui;
};

#endif // DECLARE_H
