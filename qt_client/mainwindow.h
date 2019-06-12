#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <memory>
#include "guitask.h"
#include "mcuconnection.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void on_position_change(int32_t x, int32_t y, int32_t angle);
    void on_errorTask(const QString &err);

private slots:
    void on_connectButton_clicked();

    void on_buttonUp_clicked();

    void on_buttoDown_clicked();

    void on_buttonRight_clicked();

    void on_buttonLeft_clicked();

    void on_angleSlide_valueChanged(int value);

    void on_goToButton_clicked();

    void on_setZero_clicked();

private:
    Ui::MainWindow *ui;
    std::unique_ptr<GUITask> handler;
    uint32_t speedAngleSlider;
};

#endif // MAINWINDOW_H
