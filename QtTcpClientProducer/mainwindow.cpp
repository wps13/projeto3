#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDateTime>
#include <QTextBrowser>
#include <QString>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow){
    ui->setupUi(this);
    socket = new QTcpSocket(this);
    tcpConnect();

    //faz o tratamento para a ação de clicar no botão 'Start'
    connect(ui->pushButtonStart,
            SIGNAL(clicked(bool)),
            this,
            SLOT(start()));

    //faz o tratamento para a ação de clicar no botão 'Connect'
    connect(ui->pushButtonConnect,
            SIGNAL(clicked(bool)),
            this,
            SLOT(tcpConnect()));

    //faz o tratamento para a ação de clicar no botão 'Disconnect'
    connect(ui->pushButtonDisconnect,
            SIGNAL(clicked(bool)),
            this,
            SLOT(Disconnect()));

    //faz o tratamento para a ação de clicar no botão 'Stop'
    connect(ui->pushButtonStop,
            SIGNAL(clicked(bool)),
            this,
            SLOT(stop()));

}


//faz a conexão do ip dado pelo usuário à porta '1234'
void MainWindow::tcpConnect(){

    socket->connectToHost(ui->textEditIP->toPlainText(),1234);

    if(socket->waitForConnected(3000))
    {
        qDebug() << "Connected";
        ui->textBrowser->append("Connect");
    }

    else
    {
        qDebug() << "Disconnected";
    }
}


void MainWindow::putData(){
    QDateTime datetime;
    QString str;
    qint64 msecdate;
    int min,max;


    min=ui->horizontalSliderMin->value();
    max=ui->horizontalSliderMax->value();

    if(socket->state()== QAbstractSocket::ConnectedState){

        msecdate = QDateTime::currentDateTime().toMSecsSinceEpoch();

        str = "set "+ QString::number(msecdate) + " " + QString::number(qrand()% max + min) + "\r\n";

        qDebug() << str;
        qDebug() << socket->write(str.toStdString().c_str()) << " bytes written";


        ui->textBrowser->append(str);
        if(socket->waitForBytesWritten(3000)){
            qDebug() << "wrote";
        }
    }

}

//desconecta do servidor
void MainWindow::Disconnect()
{
    socket->disconnectFromHost();
}

//inicia o timer
void MainWindow::start()
{
    timer = startTimer(ui->horizontalSliderTiming->value()*1000);
    qDebug ()<< "Timer Started";
}

//NÃO ESTÁ EXECUTANDO

//define o que vai ser feito pelo QTimer durante execução(chamar o putData)
void MainWindow::timerEvent(QTimerEvent *e)
{
    putData();
    qDebug() << "Sending Data";
}

//finaliza o timer,reiniciando-o para evitar erros
void MainWindow::stop()
{
    killTimer(timer);
    timer=0;
    qDebug() << "Timer Killed";
}


//destrutor da MainWindow
MainWindow::~MainWindow(){
    delete socket;
    delete ui;
}
