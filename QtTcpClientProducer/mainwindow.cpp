#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDateTime>


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
    connect(ui->pushButtonConnect,SIGNAL(clicked(bool)),this,SLOT(tcpConnect()));

    //faz o tratamento para a ação de clicar no botão 'Disconnect'
    connect(ui->pushButtonDisconnect,SIGNAL(clicked(bool)),this,SLOT(Disconnect()));

    //faz o tratamento para a ação de clicar no botão 'Stop'
    connect(ui->pushButtonStop,SIGNAL(clicked(bool)),this,SLOT(stop()));

}


//faz a conexão do ip dado pelo usuário à porta '1234'
void MainWindow::tcpConnect(){

    socket->connectToHost(ui->textEditIP->toPlainText(),1234);

    if(socket->waitForConnected(3000))
    {
        qDebug() << "Connected";
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

    if(socket->state()== QAbstractSocket::ConnectedState){

        msecdate = QDateTime::currentDateTime().toMSecsSinceEpoch();


        str = "set "+ QString::number(msecdate) + " " + QString::number(qrand()% ui->horizontalSliderMax->value()
                                                                        + ui->horizontalSliderMin->value())+"\r\n";

        qDebug() << str;
        qDebug() << socket->write(str.toStdString().c_str()) << " bytes written";
        if(socket->waitForBytesWritten(3000)){
            qDebug() << "wrote";
        }
    }
}

//desconecta do servidor
void MainWindow::Disconnect()
{
    socket->disconnectFromHost();

    //criar debug para testar se realmente foi desconectado
}

//inicia o timer
void MainWindow::start()
{
    timer = startTimer(ui->horizontalSliderTiming->value()*1000);
    qDebug ()<< "Timer Started";
}

//define o que vai ser feito pelo QTimer durante execução(chamar o putData)
void MainWindow::timerEvent(QTimer *e)
{
    putData();
    qDebug() << "Sending Data";
}

//finaliza o timer
void MainWindow::stop()
{
    killTimer(timer);
    timer=0;
    qDebug() << "Timer Killed";
}




MainWindow::~MainWindow(){
    delete socket;
    delete ui;
}
