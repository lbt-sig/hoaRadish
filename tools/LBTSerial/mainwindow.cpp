#include <QHBoxLayout>
#include <QLabel>
#include "HCrc.h"
#include "HConvert.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    resize(1099,776);
    setWindowFlags(windowFlags()&~Qt::WindowMaximizeButtonHint);    // 禁止最大化按钮
//    setFixedSize(this->width(),this->height());                   // 禁止拖动窗口大小
    spl = new QSplitter(this);
//    spl->setFixedSize(500, 500);

    plot_tabWidget = new HTabWidget();
    plot_tabWidget->resize(871, 335);

    // 设置垂直方向
    spl->setGeometry(220, 22, 871, 600);
    spl->setOrientation(Qt::Vertical);
    this->plot_tabWidget->addTab(ui->plot_widget, "速度波形");
    this->plot_tabWidget->addTab(ui->current_widget, "电流波形");
    spl->addWidget(this->plot_tabWidget);
    spl->addWidget(ui->recv_groupBox);

    // 设置导航箭头
    ui->arrow_pushButton->setStyleSheet("border-image: url(:/img/left.png);");
    m_serialPort = new QSerialPort();

    // 设置pid设置表格
    QStringList sl;
    QVector<QString> vs = {"参数", "值"};
    for(int i = 0;i <vs.size();++i){
        sl.push_back(vs[i]);
    }

    QStandardItemModel* model = new QStandardItemModel();
    model->setHorizontalHeaderLabels(sl);
    model->setItem(0, 0, new QStandardItem(QString("速度环 P").arg(0)));
    model->setItem(1, 0, new QStandardItem(QString("速度环 I").arg(1)));
    model->setItem(2, 0, new QStandardItem(QString("速度环 D").arg(2)));
    model->setItem(3, 0, new QStandardItem(QString("电流环 P").arg(3)));
    model->setItem(4, 0, new QStandardItem(QString("电流环 I").arg(4)));
    model->setItem(5, 0, new QStandardItem(QString("电流环 D").arg(5)));

    for (int i = 0; i < 6;i++)
    {
        auto item = model->item(i, 0);
        item->setEditable(false);
    }
    /* 自适应所有列，让它布满空间 */
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    /* 设置表格视图数据 */
    ui->tableView->setModel(model);
    //设置自动排序
    ui->tableView->setSortingEnabled(true);
    QHeaderView* headerView = ui->tableView->verticalHeader();
    headerView->setHidden(true);

    Init();

    // 测试数据
    ui->recv_textBrowser->setText("01 04 00 00 01 04 00 00 00 01 31 CA 00 01 31 CA 01 04 00 00 01 04 00 00 00 01 31 CA 00 01 31 CA");
    for (char i = 0; i < 20; i++){
        arrX.append(arrX.count());
        arrY.append(qrand()%4096);

        ui->plot_widget->graph(0)->setData(arrX,arrY);
        ui->plot_widget->replot();
    }

    for (char i = 0; i < 20; i++){
        arrX.append(arrX.count());
        arrY.append(qrand()%4096);

        ui->current_widget->graph(0)->setData(arrX,arrY);

        ui->current_widget->replot();
    }

}

MainWindow::~MainWindow()
{
    delete ui;
    delete spl;
    delete plot_tabWidget;
}

void MainWindow::Init(){
    SerialInit();
    BaudRateInit();
    DataBitInit();
    StopBitInit();
    PlotInit();
}

void MainWindow::SetComboBox(QStringList &info, QComboBox *qcm, unsigned char flag){
    qcm->addItems(info);
    flag == 0 ? qcm->setCurrentIndex(0) : qcm->setCurrentIndex(flag);
}

void MainWindow::SerialInit(){
    serial_btn_flag = false;
    //读取串口信息
    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        // 自动读取串口号添加到端口portBox中
        QSerialPort serial;
        serial.setPort(info);
        if(serial.open(QIODevice::ReadWrite))
        {
            ui->serial_no_comboBox->addItem(info.portName());
            serial.close();
        }
    }
}

void MainWindow::BaudRateInit(){
    QStringList baudList; //波特率信息
    baudList<<"1200"<<"2400"<<
              "4800"<<"9600"<<
              "14400"<<"19200"<<
              "38400"<<"56000"<<
              "57600"<<"115200";
    SetComboBox(baudList, ui->baud_comboBox);
}

void MainWindow::DataBitInit(){
    QStringList dataBitsList;
    dataBitsList<<"5"<<"6"<<"7"<<"8";
    SetComboBox(dataBitsList, ui->data_comboBox, 3);
}

void MainWindow::StopBitInit(){
    QStringList stopBitList;
    stopBitList<<"1"<<"2";
    SetComboBox(stopBitList, ui->stop_comboBox);
}


void MainWindow::on_Send_pushButton_clicked()
{
    if(ui->serial_no_comboBox->currentText() == ""){
        QMessageBox::information(this,"warning","没有找到串口");
    }
}

void MainWindow::on_tabWidget_tabBarClicked(int index)
{

}

// 导航箭头点击按钮
void MainWindow::on_arrow_pushButton_clicked()
{
    int delat = 165;
    if (ui->side_widget->isHidden())
    {
        ui->side_widget->show();
        ui->arrow_pushButton->setStyleSheet("border-image: url(:/img/left.png);");
        ui->line->setGeometry(220 , 605, ui->line->width() - delat, ui->line->height());
        this->spl->setGeometry(220 , 22, this->spl->width() - delat,this->spl->height());
        ui->send_groupBox_2->setGeometry(220, 630, ui->send_groupBox_2->width() - delat, ui->send_groupBox_2->height());
        ui->recv_textBrowser->resize(ui->recv_textBrowser->width() - delat, ui->recv_textBrowser->height());
        ui->send_plainTextEdit->setGeometry(10, 20, ui->send_plainTextEdit->width() - delat, ui->send_plainTextEdit->height());
        ui->Send_pushButton->setGeometry(810, 20, ui->Send_pushButton->width(), ui->Send_pushButton->height());
    }
    else
    {
        ui->side_widget->hide();
        ui->arrow_pushButton->setStyleSheet("border-image: url(:/img/right.png);");
        ui->line->setGeometry(220 -  delat, 605, ui->line->width() + delat, ui->line->height());
        ui->recv_textBrowser->resize(ui->recv_textBrowser->width() + delat, ui->recv_textBrowser->height());
        this->spl->setGeometry(220 - delat , 22, this->spl->width() + delat,this->spl->height());
        ui->send_groupBox_2->setGeometry(220 - delat, 630, ui->send_groupBox_2->width() + delat, ui->send_groupBox_2->height());
        ui->send_plainTextEdit->setGeometry(10, 20, ui->send_plainTextEdit->width() + delat, ui->send_plainTextEdit->height());
        ui->Send_pushButton->setGeometry(810 + delat, 20, ui->Send_pushButton->width(), ui->Send_pushButton->height());
    }

}

void MainWindow::on_other_setting_pushButton_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_2);
}

void MainWindow::on_serial_setting_pushButton_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page);
}

void MainWindow::PlotInit(){
    // 速度波形
    ui->plot_widget->addGraph();
    ui->plot_widget->graph(0)->setPen(QPen(Qt::blue));
    ui->plot_widget->graph(0)->setBrush(QBrush(QColor(255,255,0)));
    ui->plot_widget->xAxis->setLabel("x");
    ui->plot_widget->yAxis->setLabel("y");

    ui->plot_widget->xAxis->setRange(0,60);
    ui->plot_widget->yAxis->setRange(0,5000);

    // 电流波形
    ui->current_widget->addGraph();
    ui->current_widget->graph(0)->setPen(QPen(Qt::blue));
    ui->current_widget->graph(0)->setBrush(QBrush(QColor(255,255,0)));
    ui->current_widget->xAxis->setLabel("x");
    ui->current_widget->yAxis->setLabel("y");

    ui->current_widget->xAxis->setRange(0,60);
    ui->current_widget->yAxis->setRange(0,5000);
}

void MainWindow::receiveInfo(){
    qDebug()<<"receiveInfo()";
    QByteArray info = m_serialPort->readAll();

    QString strReceiveData = "";
    if(ui->recv_hex_checkBox->isChecked())
    {
        QByteArray hexData = info.toHex();
        strReceiveData = hexData.toUpper();

        for(int i=0; i<strReceiveData.size(); i+=2+1)
            strReceiveData.insert(i, QLatin1String(" "));
        strReceiveData.remove(0, 1);

        ui->recv_textBrowser->append(strReceiveData);

        //解析接收到的串口数据
        uint8_t pCharData[15] = { 0x00 };
        uint8_t crc16Low, crc16High;

        for (int i=0;i<info.size();i++)
        {
            pCharData[i] = static_cast<uint8_t>(info.at(i));
        }

        uint16_t crc16 = chkcrc(pCharData, 5);
        crc16Low = static_cast<uint8_t>(crc16 & 0xff);             //校验位低8位
        crc16High = static_cast<uint8_t>((crc16 >> 8) & 0xff);     //校验位高8位

        if(crc16High == static_cast<uint8_t>(info.at(5)) && crc16Low == static_cast<uint8_t>(info.at(6)))
        {
            if(static_cast<uint8_t>(info.at(0)) == 0x01 && static_cast<uint8_t>(info.at(1) == 0x04))
            {
                uint16_t nSoundIntensity = static_cast<uint8_t>(info.at(3))*256 + static_cast<uint8_t>(info.at(4));
                arrX.append(arrX.count());
                arrY.append(nSoundIntensity);

                //传入数据，setData的两个参数类型为double
                ui->plot_widget->graph(0)->setData(arrX,arrY);
                //曲线重绘
                ui->plot_widget->replot();
            }
        }
    }
    else
    {
        strReceiveData = info;

        //避免中文乱码
        QTextCodec *tc = QTextCodec::codecForName("GBK");
        QString tmpQStr = tc->toUnicode(info);

        ui->recv_textBrowser->append(tmpQStr);
    }
}

void MainWindow::on_checkBox_open_clicked()
{
    if (this->serial_btn_flag == false) {
        if(m_serialPort->isOpen())
        {
            m_serialPort->clear();
            m_serialPort->close();
        }

        //设置串口名字 假设我们上面已经成功获取到了 并且使用第一个
        m_serialPort->setPortName(ui->serial_no_comboBox->currentText());

        if(!m_serialPort->open(QIODevice::ReadWrite))//用ReadWrite 的模式尝试打开串口
        {
            QMessageBox::information(this,"warning","打开串口失败！");
            return;
        }

        //打开成功
        m_serialPort->setBaudRate(ui->baud_comboBox->currentText().toInt(),QSerialPort::AllDirections);//设置波特率和读写方向
        m_serialPort->setDataBits(QSerialPort::Data8);              //数据位为8位
        m_serialPort->setFlowControl(QSerialPort::NoFlowControl);   //无流控制
        m_serialPort->setParity(QSerialPort::NoParity);             //无校验位
        m_serialPort->setStopBits(QSerialPort::OneStop);            //一位停止位

        ui->checkBox_open->setToolTip("关闭串口");
        this->serial_btn_flag = true;
        //连接信号槽 当下位机发送数据QSerialPortInfo 会发送个 readyRead 信号,我们定义个槽void receiveInfo()解析数据
        connect(m_serialPort,SIGNAL(readyRead()),this,SLOT(receiveInfo()));
    }
    else
    {
        this->serial_btn_flag = false;
        ui->checkBox_open->setToolTip("打开串口");
        m_serialPort->close();
    }
}
