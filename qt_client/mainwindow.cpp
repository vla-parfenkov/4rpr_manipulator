#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow())
{
    conn =  std::make_unique<MCUConnection>();
    handler = std::make_unique<GUITask>();
    QObject::connect(handler.get(), SIGNAL(changePosition(int32_t, int32_t, int32_t)),
              this, SLOT(on_position_change(int32_t, int32_t, int32_t)));
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_position_change(int32_t x, int32_t y, int32_t angle)
{
    ui->xPosition->display(static_cast<int>(x));
    ui->yPosition->display(static_cast<int>(y));
    ui->axisPosition->display(static_cast<int>(angle));

}

void MainWindow::on_connectButton_clicked()
{
    conn->connect();
}



void MainWindow::on_buttonUp_clicked()
{
    handler->handleMove(MoveUp, static_cast<uint32_t>(ui->ySpeed->value()));
}

void MainWindow::on_buttoDown_clicked()
{
     handler->handleMove(MoveDown, static_cast<uint32_t>(ui->ySpeed->value()));
}




void MainWindow::on_buttonRight_clicked()
{
     handler->handleMove(MoveRight, static_cast<uint32_t>(ui->xSpeed->value()));
}

void MainWindow::on_buttonLeft_clicked()
{
     handler->handleMove(MoveLeft, static_cast<uint32_t>(ui->xSpeed->value()));
}


void MainWindow::on_angleSlide_valueChanged(int value)
{
    if(ui->axisPosition->value() <= value)
    {
        handler->handleMove(MoveAngleRight, value - ui->axisPosition->value());
    } else {
        handler->handleMove(MoveAngleLeft, ui->axisPosition->value() - value);
    }

}
