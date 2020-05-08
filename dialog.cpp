#include "dialog.h"
#include "ui_dialog.h"

extern int botLevel;

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    this->showMaximized();
    QPixmap pix(":src/img/dialog_level_" + QString::number(botLevel) + ".jpg");
    pix = pix.scaled(this->size(), Qt::IgnoreAspectRatio);
    QPalette pal;
    pal.setBrush(QPalette::Background, pix);
    this->setPalette(pal);
}

Dialog::~Dialog()
{
    delete ui;
}
