#include "mainwindow.hh"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    timer = new QTimer(parent);
    // yhdistetään
    connect(timer, &QTimer::timeout,this,&MainWindow::updateLcd);
    // yhdistetään napit
    connect(ui->startButton, &QPushButton::clicked, this, &MainWindow::startTimer);
    connect(ui->stopButton, &QPushButton::clicked, this, &MainWindow::stopTimer);
    connect(ui->resetButton, &QPushButton::clicked, this, &MainWindow::resetTimer);
    connect(ui->closeButton, &QPushButton::clicked, this, &MainWindow::closeTimer);

    // vaihdetaan värit
    ui->lcdNumberMin->setStyleSheet("background-color: darkCyan");
    ui->lcdNumberSec->setStyleSheet("background-color: cyan");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::startTimer()
{
    // timer käynnistyy, valmiina QTimer kirjastossa
    timer->start(1000);
    time = 0;

}

void MainWindow::stopTimer()
{
    timer->stop();
}

void MainWindow::resetTimer()
{
    // tyhjennetään LCD
    ui->lcdNumberMin->display(0);
    ui->lcdNumberSec->display(0);
    startTimer();
}

void MainWindow::closeTimer()
{
    this->close();
}

void MainWindow::updateLcd()
{
    // haetaan aika
    time++;
    ui->lcdNumberMin->display(time/60);
    ui->lcdNumberSec->display(time%60);
}
