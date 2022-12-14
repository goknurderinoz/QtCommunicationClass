#ifndef CHATSERVER_H
#define CHATSERVER_H

#include <QtCore/QObject>
#include <QtCore/QList>
#include <QtCore/QByteArray>
#include <QtWebSockets/QWebSocketServer>
#include <QtWebSockets/QWebSocket>

class ChatServer : public QObject
{
    Q_OBJECT
public:
    explicit ChatServer(quint16 port, QObject *parent = Q_NULLPTR);
    virtual ~ChatServer();
public slots:
    void closeServerSocket();

private Q_SLOTS:
    void onNewConnection();
    void processMessage(QString message);
    void socketDisconnected();

private:
    QWebSocketServer *m_pWebSocketServer;
    QList<QWebSocket *> m_clients;
signals:
    void newMessage(QString);
};

#endif //CHATSERVER_H
