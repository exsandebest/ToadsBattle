#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "rules.h"

int botLevel = 1;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->showMaximized();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_btn_level_clicked()
{
    if (botLevel == 1){
        botLevel = 2;
        ui->btn_level->setText("LEVEL: MEDIUM");
    } else if (botLevel == 2){
        botLevel = 3;
        ui->btn_level->setText("LEVEL: HARD");
    } else if (botLevel == 3){
        botLevel = 1;
        ui->btn_level->setText("LEVEL: EASY");
    }
}

void MainWindow::on_btn_rules_clicked()
{
    Rules rulesWindow;
    rulesWindow.exec();
}
