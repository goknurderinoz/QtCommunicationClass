#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <string>
#include <QWidget>
#include <QObject>
#include <QDebug>
#include <QNetworkAccessManager>        // to make request
#include <QNetworkReply>                // to handle reply
#include <QTcpSocket>                   // TCP Socket
#include <QTcpServer>                   // TCP Server
#include "QtWebSockets/qwebsocketserver.h"            // Web Socket

// JSON
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <QDataStream>
#include <string>

//WebSocket
#include <QtCore/QObject>
#include <QtCore/QList>
#include <QtCore/QByteArray>
#include <QtWebSockets/QWebSocketServer>
#include <QtWebSockets/QWebSocket>

#include <QPushButton>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

Q_SIGNALS:
    void closed();

private slots:
//TCP Client
    void startTcpConnection();
    void getHTTP();
    void onfinish(QNetworkReply *rep);
//TCP Server
    void openTCPServer();
    void newTCPConnection();
    void readDataFromTCPSocket();
    void on_error(QAbstractSocket::SocketError);
//WebSocket Server
    void openWSServer(quint16 port);
    void onNewConnection();
    void socketDisconnected();
    void processTextMessage(QString message);
//Buttons and Display Slots
    void clearDisplay();
    void ClickedButtons();

private:
    Ui::MainWindow *ui;
    QTcpSocket *socket;
    QTcpServer *server;
    QNetworkAccessManager *manager;
    QNetworkReply *reply;
    QWebSocketServer *m_pWebSocketServer;
    QList<QWebSocket *> m_clients;
    bool m_debug;
};

#endif // MAINWINDOW_H
