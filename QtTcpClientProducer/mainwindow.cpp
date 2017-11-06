#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDateTime>
#include <QTextBrowser>
#include <QString>

/**
 * @brief MainWindow::MainWindow Janela Principal, onde tudo será executado
 * @param parent
 */
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow){
    ui->setupUi(this);
    socket = new QTcpSocket(this);
    tcpConnect();


    /**
     * @brief connect faz o tratamento para a ação de clicar no botão 'Start'
     */
    connect(ui->pushButtonStart,
            SIGNAL(clicked(bool)),
            this,
            SLOT(start()));


    /**
     * @brief connect faz o tratamento para a ação de clicar no botão 'Connect'
     */
    connect(ui->pushButtonConnect,
            SIGNAL(clicked(bool)),
            this,
            SLOT(tcpConnect()));


    /**
     * @brief connect faz o tratamento para a ação de clicar no botão 'Disconnect'
     */
    connect(ui->pushButtonDisconnect,
            SIGNAL(clicked(bool)),
            this,
            SLOT(Disconnect()));


    /**
     * @brief connect faz o tratamento para a ação de clicar no botão 'Stop'
     */
    connect(ui->pushButtonStop,
            SIGNAL(clicked(bool)),
            this,
            SLOT(stop()));

}


/**
 * @brief MainWindow::tcpConnect faz a conexão do ip dado pelo usuário à porta '1234'
 */
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

/**
 * @brief MainWindow::putData Envia os dados ao Servidor
 */
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

/**
 * @brief MainWindow::Disconnect Desconecta o IP do servidor
 */
void MainWindow::Disconnect()
{
    socket->disconnectFromHost();
}

/**
 * @brief MainWindow::start Inicia o timer
 */
void MainWindow::start()
{
    timer = startTimer(ui->horizontalSliderTiming->value()*1000);
    qDebug ()<< "Timer Started";
}


/**
 * @brief MainWindow::timerEvent Define o que vai ser feito pelo QTimer durante execução(chamar o putData)
 * @param e
 */
void MainWindow::timerEvent(QTimerEvent *e)
{
    putData();
    qDebug() << "Sending Data";
}


/**
 * @brief MainWindow::stop finaliza o timer,reiniciando-o para evitar erros
 */
void MainWindow::stop()
{
    killTimer(timer);
    timer=0;
    qDebug() << "Timer Killed";
}

/**
 * @brief MainWindow::~MainWindow Destrutor da MainWindow
 */
MainWindow::~MainWindow(){
    delete socket;
    delete ui;
}
