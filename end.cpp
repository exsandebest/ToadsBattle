#include "end.h"
#include "ui_end.h"
#include <QCloseEvent>

End::End(QWidget *parent, int gameResult) : QDialog(parent), ui(new Ui::End) {
    ui->setupUi(this);
    QApplication::processEvents();

    QString endingsFiles[3] = {":img/end_lose.png", ":img/end_draw.png", ":img/end_win.png"};
    QPixmap pix(endingsFiles[gameResult + 1]);
    pix = pix.scaled(this->size(), Qt::IgnoreAspectRatio);
    QPalette pal;
    pal.setBrush(QPalette::Background, pix);
    this->setPalette(pal);
}

End::~End() { delete ui; }

void End::on_btn_ok_clicked() {
    this->close();
    emit end();
}

void End::closeEvent(QCloseEvent * e) {
    emit end();
    e->accept();
}

