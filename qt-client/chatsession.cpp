#include "chatsession.h"

ChatSession::ChatSession(QObject *parent) :
    QObject(parent)
{
    connect(&Socket, SIGNAL(readyRead()), this, SLOT(SocketReadyRead()));
    connect(&Socket, SIGNAL(connected()), this, SLOT(onSocketConnected()));
    connect(&Socket, SIGNAL(disconnected()), this, SLOT(onSocketDisconnected()));
}
void ChatSession::sendMessage(QString Message)
{
    QTextStream text(&Socket);
    text<<Message;
}
void ChatSession::SocketReadyRead()
{
    emit receivedMessage(Socket.readAll());
}
void ChatSession::SocketConnect(QString ip, int port)
{
    Socket.connectToHost(ip,port);
}
void ChatSession::SocketDisconnect()
{
    Socket.close();
}
void ChatSession::onSocketConnected()
{
    emit SocketConnected();
}
void ChatSession::onSocketDisconnected()
{
    emit SocketDisconnected();
}
