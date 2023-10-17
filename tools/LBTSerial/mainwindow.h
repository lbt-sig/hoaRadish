#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QComboBox>
#include <QSplitter>
#include <QMainWindow>
#include <QMessageBox>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QPushButton>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

#include "HtabWidget.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    // 主初始化函数
    void Init();

    // 设置combobox
    void SetComboBox(QStringList &info, QComboBox * qcm, unsigned char flag = 0);

    // 初始化串口信息
    void SerialInit();

    // 初始化波特率
    void BaudRateInit();

    // 初始化数据位
    void DataBitInit();

    // 初始化停止位
    void StopBitInit();

    // 波形初始化
    void PlotInit();

private slots:
    void on_Send_pushButton_clicked();

    void on_tabWidget_tabBarClicked(int index);

    void on_arrow_pushButton_clicked();

    void on_other_setting_pushButton_clicked();

    void on_serial_setting_pushButton_clicked();

    void receiveInfo();
    void on_checkBox_open_clicked();

private:
    bool serial_btn_flag;
    QVector<double> arrX;
    QVector<double> arrY;
    QSerialPort* m_serialPort;

    Ui::MainWindow *ui;
    HTabWidget  *plot_tabWidget;
    QSplitter *spl;
};
#endif // MAINWINDOW_H
