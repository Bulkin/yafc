#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork>
#include <QTcpSocket>
#include <QObject>
#include <QByteArray>
#include <QDebug>
#include <chatsession.h>
#include <QString>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

    void onSocketConnected();
    void onSocketDisconnected();
    void on_ButtonConnect_clicked();
    void on_ButtonDisconnect_clicked();
    void on_ButtonSend_clicked();
    void on_lineEditTextSend_returnPressed();
    void printMessage(QString Message);
public slots:


private:
    Ui::MainWindow *ui;
    ChatSession _ChatSession;

};

#endif // MAINWINDOW_H
