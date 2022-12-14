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


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
//TCP Client
    void startTcpConnection();
    void on_httpButton_clicked();
    void onfinish(QNetworkReply *rep);
//TCP Server
    void openTCPServer();
    void newTCPConnection();
    void on_readDataFromSocket_clicked();
    void on_error(QAbstractSocket::SocketError);
    void on_clearButton_clicked();

    void on_ChatServer_clicked();

private:
    Ui::MainWindow *ui;
    QTcpSocket *socket;
    QTcpServer *server;
    QNetworkAccessManager *manager;
    QNetworkReply *reply;

};

#endif // MAINWINDOW_H
