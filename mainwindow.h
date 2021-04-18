#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btn_level_clicked();
    void on_btn_startGame_clicked();

private:
    Ui::MainWindow *ui;
    int botLevel = 1;
};
#endif // MAINWINDOW_H
