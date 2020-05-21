#include "end.h"
#include "ui_end.h"
#include <QCloseEvent>

extern int gameResult;

End::End(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::End)
{
    ui->setupUi(this);
    QApplication::processEvents();
    if (gameResult == 1) {
        QPixmap pix(":img/end_win.png");
        pix = pix.scaled(this->size(), Qt::IgnoreAspectRatio);
        QPalette pal;
        pal.setBrush(QPalette::Background, pix);
        this->setPalette(pal);
    } else if (gameResult == -1) {
        QPixmap pix(":img/end_lose.png");
        pix = pix.scaled(this->size(), Qt::IgnoreAspectRatio);
        QPalette pal;
        pal.setBrush(QPalette::Background, pix);
        this->setPalette(pal);
    } else if (gameResult == 0){
        QPixmap pix(":img/end_draw.png");
        pix = pix.scaled(this->size(), Qt::IgnoreAspectRatio);
        QPalette pal;
        pal.setBrush(QPalette::Background, pix);
        this->setPalette(pal);
    }
    QApplication::processEvents();
}

End::~End()
{
    delete ui;
}

void End::on_btn_ok_clicked()
{
    this->close();
    emit end();
}

void End::closeEvent(QCloseEvent * e) {
    emit end();
    e->accept();
}

