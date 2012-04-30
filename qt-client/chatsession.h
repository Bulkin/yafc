#ifndef CHATSESSION_H
#define CHATSESSION_H

#include <QObject>
#include <QtNetwork>
#include <QTcpSocket>
#include <QByteArray>
#include <QDebug>

class ChatSession : public QObject
{
    Q_OBJECT
    QTcpSocket Socket; //сокет
public:
    explicit ChatSession(QObject *parent = 0);
    
signals:
  void   receivedMessage(QString Message);
  void   SocketConnected();
  void   SocketDisconnected();
private slots:
  void   SocketReadyRead();
  void   onSocketConnected();
  void   onSocketDisconnected();
public slots:
  void   sendMessage(QString Message);
  void   SocketConnect(QString ip,int port);
  void   SocketDisconnect();

};

#endif // CHATSESSION_H
