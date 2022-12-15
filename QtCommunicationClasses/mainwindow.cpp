#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QtWebSockets/qwebsocketserver.h"
#include "QtWebSockets/qwebsocket.h"
#include <QtCore/QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    startTcpConnection();
    openWSServer(1998, false);

}

MainWindow::~MainWindow()
{
    delete ui;
}

//TCP CLIENT
void MainWindow::startTcpConnection()
{
     socket = new QTcpSocket(this);

     ui -> terminalLineEdit -> clear();
     socket -> connectToHost("127.0.0.1", 1234);
     connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));

     if(socket -> waitForConnected(1000)){

         ui -> terminalLineEdit -> appendPlainText("Connected to host!");

         QJsonObject json;
         json.insert("Request", "Get");
         QJsonDocument doc(json);
         QString jsonString = doc.toJson(QJsonDocument::Indented);
         qDebug() << jsonString.toStdString().c_str();

         char myChar[1023];
         memset(myChar, '\0',sizeof(myChar)+1);
         memcpy(myChar, jsonString.toUtf8(), jsonString.length());
         socket->write(myChar);
         socket->flush();
         socket->waitForBytesWritten(50);
         ui->terminalLineEdit->appendPlainText("Request send to Node-RED : " );
     }
     else
     {
         ui -> terminalLineEdit -> appendPlainText("Not connected to host!");
     }

}

// HTTP GET
void MainWindow::on_httpButton_clicked()
{
    manager = new QNetworkAccessManager(this);
    QNetworkAccessManager * mgr = new QNetworkAccessManager(this);
    connect(mgr,SIGNAL(finished(QNetworkReply*)),this,SLOT(onfinish(QNetworkReply*)));
    mgr->get(QNetworkRequest(QUrl("http://127.0.0.1:1880/qt-example")));

}


void MainWindow::onfinish(QNetworkReply *rep)
{
    QString answer = rep->readAll();
    qDebug() << answer;
    ui->terminalLineEdit_2->appendPlainText("Get HTTP: "  + answer);
}


//TCP Server
void MainWindow::openTCPServer()
{
    server = new QTcpServer(this);
    connect(server, SIGNAL(newConnection()), this, SLOT(newTCPConnection()));
    connect(server, SIGNAL(acceptError(QAbstractSocket::SocketError)), this, SLOT(on_error(QAbstractSocket::SocketError)));
    if(!server->listen(QHostAddress::Any, 1234))
    {
        qDebug() << "Server could not start";
    }
    else
    {
        qDebug() << "Server started!";
    }
}

void MainWindow::newTCPConnection()
{
    // need to grab the socket
    qDebug() << "Server has a new Connection ";
    socket = server->nextPendingConnection();
    connect(socket, SIGNAL(readyRead()), SLOT(on_readDataFromSocket_clicked()));
}


void MainWindow::on_readDataFromSocket_clicked()
{
    QByteArray data;

    if (socket->bytesAvailable())
    {
        data = socket->readAll();
        qDebug() << "Incoming socket data: " << data;
        socket->flush();
        socket->waitForBytesWritten(50);
        ui->terminalLineEdit_2->appendPlainText("Received Data : " + data);
    }
}

void MainWindow::on_clearButton_clicked()
{
    ui->terminalLineEdit_2->clear();
}

void MainWindow::on_error(QAbstractSocket::SocketError){
    qDebug() << "server error";
    server->close();
}


//WebSocket Server

void MainWindow::openWSServer(quint16 port, bool debug)
{
    m_pWebSocketServer=new QWebSocketServer(QStringLiteral("Echo Server"),
                                            QWebSocketServer::NonSecureMode, this);
    port = 1998;
    if (m_pWebSocketServer->listen(QHostAddress::Any, port)) {

            qDebug() << "Echoserver listening on port" << port;
        connect(m_pWebSocketServer, &QWebSocketServer::newConnection,
                this, &MainWindow::onNewConnection);
        connect(m_pWebSocketServer, &QWebSocketServer::closed, this, &MainWindow::closed);

    }
}

void MainWindow::onNewConnection()
{
    QWebSocket *pSocket = m_pWebSocketServer->nextPendingConnection();

    connect(pSocket, &QWebSocket::textMessageReceived, this, &MainWindow::processTextMessage);
    connect(pSocket, &QWebSocket::disconnected, this, &MainWindow::socketDisconnected);

}


void MainWindow::on_processTextMessage_clicked(QString message)
{
    ui->terminalLineEdit_2->appendPlainText("Received Data with WS: " + message);

}

void MainWindow::processTextMessage(QString message)
{
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
        qDebug() << "Message received:" << message;
        ui->terminalLineEdit_2->appendPlainText("Received Data with WS: " + message);

}

void MainWindow::socketDisconnected()
{
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    if (m_debug)
        qDebug() << "socketDisconnected:" << pClient;
    if (pClient) {
        m_clients.removeAll(pClient);
        pClient->deleteLater();
    }
}

