#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "config.h"
#include "QtWebSockets/qwebsocketserver.h"
#include "QtWebSockets/qwebsocket.h"
#include <QtCore/QDebug>
#include <QSettings>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    configReader();
    ClickedButtons();
    startTcpConnection();
    openWSServer();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::ClickedButtons()
{
    connect(ui->clearButton, &QPushButton::clicked, this, &MainWindow::clearDisplay);
    connect(ui->getTCP, &QPushButton::clicked, this, &MainWindow::readDataFromTCPSocket);
    connect(ui->httpButton, &QPushButton::clicked, this, &MainWindow::getHTTP);
}

//Read Port and IP from ini file
void MainWindow::configReader()
{
    QString qstrConfigPath = QApplication::applicationDirPath();
    qstrConfigPath.append("/Config/QtCommunication_Config.ini");
    QSettings QtCommunication_settings(qstrConfigPath,QSettings::IniFormat);
    QtCommunication_settings.setIniCodec("UTF-8");

    QtCommunication_settings.beginGroup("COMMUNICATION_SETUP_VALUES");
    QList<QString> keys_list;
    config::IP_ADRESS = QtCommunication_settings.value("ip_adress").toString();
    config::TCP_PORT_NUMBER=QtCommunication_settings.value("tcp_port").toInt();
    config::WS_PORT_NUMBER=QtCommunication_settings.value("ws_port").toInt();
    QtCommunication_settings.endGroup();

    qDebug()<<"CURRENT IP:"<<config::IP_ADRESS;
    qDebug()<<"CURRENT TCP PORT:"<<config::TCP_PORT_NUMBER;
    qDebug()<<"CURRENT WS PORT:"<<config::WS_PORT_NUMBER;

}

//TCP CLIENT
void MainWindow::startTcpConnection()
{
     socket = new QTcpSocket(this);

     ui -> terminalLineEdit -> clear();
     socket -> connectToHost(config::IP_ADRESS, config::TCP_PORT_NUMBER);
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

//TCP Server
void MainWindow::openTCPServer()
{
    server = new QTcpServer(this);
    connect(server, SIGNAL(newConnection()), this, SLOT(newTCPConnection()));
    connect(server, SIGNAL(acceptError(QAbstractSocket::SocketError)), this, SLOT(on_error(QAbstractSocket::SocketError)));
    if(!server->listen(QHostAddress::Any, config::TCP_PORT_NUMBER))
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


void MainWindow::readDataFromTCPSocket()
{
    QByteArray data;

    if (socket->bytesAvailable())
    {
        data = socket->readAll();
        qDebug() << "Incoming socket data: " << data;
        socket->flush();
        socket->waitForBytesWritten(50);
        ui->terminalLineEdit_2->appendPlainText("Get TCP : " + data);
    }
}
void MainWindow::on_error(QAbstractSocket::SocketError){
    qDebug() << "server error";
    server->close();
}


// HTTP GET
void MainWindow::getHTTP()
{
    manager = new QNetworkAccessManager(this);
    QNetworkAccessManager * mgr = new QNetworkAccessManager(this);
    connect(mgr,SIGNAL(finished(QNetworkReply*)),this,SLOT(onfinish(QNetworkReply*)));
    mgr->get(QNetworkRequest(QUrl("http://127.0.0.1:1882/qt-example")));

}


void MainWindow::onfinish(QNetworkReply *rep)
{
    QString answer = rep->readAll();
    qDebug() << answer;
    ui->terminalLineEdit_2->appendPlainText("Get HTTP: "  + answer);
}

//Clear Terminal
void MainWindow::clearDisplay()
{
    ui->terminalLineEdit_2->clear();
    ui->terminalLineEdit_3->clear();
}

//WebSocket Server

void MainWindow::openWSServer()
{
    m_pWebSocketServer=new QWebSocketServer(QStringLiteral("Echo Server"),
                                            QWebSocketServer::NonSecureMode, this);
    if (m_pWebSocketServer->listen(QHostAddress::Any, config::WS_PORT_NUMBER)) {

            qDebug() << "Echoserver listening on port" << config::WS_PORT_NUMBER;
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

void MainWindow::processTextMessage(QString message)
{
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    qDebug() << "Message received:" << message;
    ui->terminalLineEdit_3->appendPlainText("Received Data with WS: " + message);
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





