#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    startTcpConnection();

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






