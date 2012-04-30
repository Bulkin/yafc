#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(&_ChatSession, SIGNAL(receivedMessage(QString)), this, SLOT(printMessage(QString)));
    connect(&_ChatSession, SIGNAL(SocketConnected()), this, SLOT(onSocketConnected()));
    connect(&_ChatSession, SIGNAL(SocketDisconnected()), this, SLOT(onSocketDisconnected()));
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::onSocketConnected()
{

    ui->textBrowserTextRead->append("connect \n");
    QString Message="%username " +ui->lineEditName->text();
    _ChatSession.sendMessage(Message.toUtf8());
    ui->ButtonConnect->setEnabled(false);
    ui->lineEditTextSend->setEnabled(true);
    ui->ButtonSend->setEnabled(true);
    ui->ButtonDisconnect->setEnabled(true);
    ui->lineEditTextSend->setFocus();
}
void MainWindow::onSocketDisconnected()
{

    ui->textBrowserTextRead->append("Disconnect \n");
    ui->ButtonConnect->setEnabled(true);
    ui->lineEditTextSend->setEnabled(false);
    ui->ButtonSend->setEnabled(false);
    ui->ButtonDisconnect->setEnabled(false);
}

void MainWindow::on_ButtonConnect_clicked()
{
    _ChatSession.SocketConnect(ui->lineEditIP->text(),ui->spinBoxPort->value());
}

void MainWindow::on_ButtonDisconnect_clicked()
{
    _ChatSession.SocketDisconnect();
}

void MainWindow::on_ButtonSend_clicked()
{
    _ChatSession.sendMessage(ui->lineEditTextSend->text().toUtf8());
    ui->lineEditTextSend->clear();
}
void MainWindow::printMessage(QString Message)
{
    ui->textBrowserTextRead->append(Message.toUtf8());

}

void MainWindow::on_lineEditTextSend_returnPressed()
{
   ui->ButtonSend->click();
}
