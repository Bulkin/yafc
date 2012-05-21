#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <vector>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    timer = new QTimer(this);
    connect(&_ChatSession, SIGNAL(receivedMessage(QString)), this, SLOT(printMessage(QString)));
    connect(&_ChatSession, SIGNAL(SocketConnected()), this, SLOT(onSocketConnected()));
    connect(&_ChatSession, SIGNAL(SocketDisconnected()), this, SLOT(onSocketDisconnected()));
    connect(timer, SIGNAL(timeout()), this, SLOT(updateListUser()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateListUser()
{
    QString Message="%list-users\n ";
    _ChatSession.sendMessage(Message.toUtf8());
}

void MainWindow::onSocketConnected()
{

    ui->textBrowserTextRead->append("connect \n");
    QString Message="%name "+ui->lineEditName->text()+"\n";
    _ChatSession.sendMessage(Message.toUtf8());
    ui->ButtonConnect->setEnabled(false);
    ui->lineEditTextSend->setEnabled(true);
    ui->ButtonSend->setEnabled(true);
    ui->ButtonDisconnect->setEnabled(true);
    ui->lineEditTextSend->setFocus();

    timer->start(1000);

}
void MainWindow::onSocketDisconnected()
{

    ui->textBrowserTextRead->append("Disconnect \n");
    ui->ButtonConnect->setEnabled(true);
    ui->lineEditTextSend->setEnabled(false);
    ui->ButtonSend->setEnabled(false);
    ui->ButtonDisconnect->setEnabled(false);
    timer->stop();
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
    QString Message=ui->lineEditTextSend->text()+"\n";
    _ChatSession.sendMessage(Message.toUtf8());
    ui->lineEditTextSend->clear();
}
void MainWindow::printMessage(QString Message)
{

    bool ok;
    if(Message.section(' ',0,0)=="%user-list" )
    {
        Message.section(' ',2,2).toInt(&ok);
        if(!ok)
        {
            Message[0]='>';
            for(int i=1;i<Message.length();i++)
                if(Message[i]==' ') Message[i]='\n';
            ui->textBrowserListUsers->clear();
            ui->textBrowserListUsers->append(Message.toUtf8());
        }
    }
    else
        ui->textBrowserTextRead->append(Message.toUtf8());
}

void MainWindow::on_lineEditTextSend_returnPressed()
{
   ui->ButtonSend->click();
}
