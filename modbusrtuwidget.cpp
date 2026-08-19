#include "modbusrtuwidget.h"
#include "ui_modbusrtuwidget.h"

#include <QSerialPortInfo>

ModbusRtuWidget::ModbusRtuWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ModbusRtuWidget)
{
    ui->setupUi(this);

    const QList<QPair<QSerialPort::Parity, QString>> parity_list = {
        { QSerialPort::NoParity, tr("None") },
        { QSerialPort::EvenParity, tr("Even") },
        { QSerialPort::OddParity, tr("Odd") },
        { QSerialPort::SpaceParity, tr("Space") },
        { QSerialPort::MarkParity, tr("Mark") }
    };
    const QList<QPair<QSerialPort::FlowControl, QString>> flow_list = {
        { QSerialPort::NoFlowControl, tr("None") },
        { QSerialPort::HardwareControl, tr("Hardware (RTS/CTS)") },
        { QSerialPort::SoftwareControl, tr("Software (XON/XOFF)") }
    };

    for(const auto &p : parity_list){
        ui->comboBox_Parity->addItem(p.second, static_cast<int>(p.first));
    }
    for(const auto &f : flow_list){
        ui->comboBox_FlowCtrl->addItem(f.second, static_cast<int>(f.first));
    }

    ui->comboBox_DataBits->addItem("5", QSerialPort::Data5);
    ui->comboBox_DataBits->addItem("6", QSerialPort::Data6);
    ui->comboBox_DataBits->addItem("7", QSerialPort::Data7);
    ui->comboBox_DataBits->addItem("8", QSerialPort::Data8);
    ui->comboBox_DataBits->setCurrentIndex(3);

    ui->comboBox_StopBits->addItem("1", QSerialPort::OneStop);
    ui->comboBox_StopBits->addItem("1.5", QSerialPort::OneAndHalfStop);
    ui->comboBox_StopBits->addItem("2", QSerialPort::TwoStop);

    const QList<qint32> baud_rates = QSerialPortInfo::standardBaudRates();
    for(qint32 baud : baud_rates){
        ui->comboBox_BaudRate->addItem(QString::number(baud), baud);
    }
    int idx = ui->comboBox_BaudRate->findData(9600);
    if(idx >= 0){
        ui->comboBox_BaudRate->setCurrentIndex(idx);
    }

    refresh_serial_ports();
    connect(ui->btn_clear_log, &QPushButton::clicked, this, &ModbusRtuWidget::sig_clear_log_requested);
}

ModbusRtuWidget::~ModbusRtuWidget()
{
    delete ui;
}

void ModbusRtuWidget::refresh_serial_ports()
{
    ui->comboBox_SerialName->clear();
    const auto ports = QSerialPortInfo::availablePorts();
    for(const QSerialPortInfo &info : ports){
        ui->comboBox_SerialName->addItem(info.portName());
    }
}

modbus_rtu_worker::serial_params ModbusRtuWidget::get_params() const
{
    modbus_rtu_worker::serial_params p;
    p.port_name = ui->comboBox_SerialName->currentText();
    p.baud_rate = ui->comboBox_BaudRate->currentData().toInt();
    p.data_bits = static_cast<QSerialPort::DataBits>(ui->comboBox_DataBits->currentData().toInt());
    p.stop_bits = static_cast<QSerialPort::StopBits>(ui->comboBox_StopBits->currentData().toInt());
    p.parity = static_cast<QSerialPort::Parity>(ui->comboBox_Parity->currentData().toInt());
    p.flow_control = static_cast<QSerialPort::FlowControl>(ui->comboBox_FlowCtrl->currentData().toInt());
    return p;
}
