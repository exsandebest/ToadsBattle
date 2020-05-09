#ifndef END_H
#define END_H

#include <QDialog>

namespace Ui {
class End;
}

class End : public QDialog
{
    Q_OBJECT

public:
    explicit End(QWidget *parent = nullptr);
    ~End();

private slots:
    void on_btn_ok_clicked();

private:
    Ui::End *ui;
};

#endif // END_H
