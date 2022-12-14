#include "chatserver.h"
#include "QtWebSockets/QWebSocketServer"
#include "QtWebSockets/QWebSocket"
#include <QtCore/QDebug>

QT_USE_NAMESPACE

//! [constructor]
ChatServer::ChatServer(quint16 port, QObject *parent) :
    QObject(parent),
    m_pWebSocketServer(Q_NULLPTR),
    m_clients()
{
    qDebug ()<< "openServerSocket 1";
    m_pWebSocketServer = new QWebSocketServer(QStringLiteral("Chat Server"),
                                              QWebSocketServer::NonSecureMode,
                                              this);
    port = 1880;
    if (m_pWebSocketServer->listen(QHostAddress::Any, port))
    {
        qDebug() << "Chat Server listening on port" << port;
        connect(m_pWebSocketServer, &QWebSocketServer::newConnection,
                this, &ChatServer::onNewConnection);
    }
    qDebug ()<< "openServerSocket 3"<<m_pWebSocketServer->isListening();
}

ChatServer::~ChatServer()
{
    //m_pWebSocketServer->close();
    // qDeleteAll(m_clients.begin(), m_clients.end());
}

void ChatServer::closeServerSocket()
{
    qDebug ()<< "closeServerSocket 1";
    qDebug() << "m_clients size :" << m_clients.size();


    m_pWebSocketServer->close();
    qDebug ()<< "closeServerSocket 2";
    m_clients.clear();
    //qDeleteAll(m_clients.begin(), m_clients.end());
}
                //! [constructor]

                //! [onNewConnection]
                void ChatServer::onNewConnection()
    {
                QWebSocket *pSocket = m_pWebSocketServer->nextPendingConnection();

                connect(pSocket, &QWebSocket::textMessageReceived, this, &ChatServer::processMessage);
                connect(pSocket, &QWebSocket::disconnected, this, &ChatServer::socketDisconnected);

                m_clients << pSocket;
}
                //! [onNewConnection]

                //! [processMessage]
                void ChatServer::processMessage(QString message)
    {           qDebug() << "/*- message before emitted:" << message;
                emit newMessage(message);
                qDebug() << "Message received:" << message;
                QWebSocket *pSender = qobject_cast<QWebSocket *>(sender());
                Q_FOREACH (QWebSocket *pClient, m_clients)
    {
                if (pClient != pSender) //don't echo message back to sender
    {
                pClient->sendTextMessage(message);
}
}
}
                //! [processMessage]

                //! [socketDisconnected]
                void ChatServer::socketDisconnected()
    {
                QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
                if (pClient)
    {
                m_clients.removeAll(pClient);
                pClient->deleteLater();
}
}
                //! [socketDisconnected]
