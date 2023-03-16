#include <QHBoxLayout>
#include <QLabel>
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 创建控件
    QPushButton *sendButton = new QPushButton("Send");
    sendButton->setFixedSize(100, 100);
    receiveTextEdit = new QTextEdit;
    receiveTextEdit->setFixedSize(500, 500);

    QHBoxLayout* layout = new QHBoxLayout;


    layout->addWidget(receiveTextEdit);
    layout->addWidget(sendButton);
    QWidget *centralWidget = new QWidget;
    centralWidget->setLayout(layout);
    setCentralWidget(centralWidget);
    // 初始化串口
    serialPort = new QSerialPort(this);
    serialPort->setPortName("COM1");
    serialPort->setBaudRate(QSerialPort::Baud9600);
    serialPort->setDataBits(QSerialPort::Data8);
    serialPort->setParity(QSerialPort::NoParity);
    serialPort->setStopBits(QSerialPort::OneStop);

    // 连接发送按钮的clicked信号到发送数据的槽函数
    connect(sendButton, &QPushButton::clicked, this, &MainWindow::sendData);

    // 连接串口对象的readyRead信号到接收数据的槽函数
    connect(serialPort, &QSerialPort::readyRead, this, &MainWindow::receiveData);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::sendData()
{
    // 发送数据
    QByteArray data = "Hello, world!";
    serialPort->write(data);
}

void MainWindow:: receiveData()
{
    // 接收数据
    QByteArray data = serialPort->readAll();
    receiveTextEdit->append(data);
}
