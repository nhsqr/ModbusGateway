#include "modbustcpwidget.h"
#include "ui_modbustcpwidget.h"

#include <QNetworkInterface>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <algorithm>

ModbusTcpWidget::ModbusTcpWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ModbusTcpWidget)
{
    ui->setupUi(this);

    QStringList ip_list;
    const QList<QNetworkInterface> interfaces = QNetworkInterface::allInterfaces();
    for(const QNetworkInterface &iface : interfaces){
        // Get all IP address entries on this interface
        const QList<QNetworkAddressEntry> entries = iface.addressEntries();
        for(const QNetworkAddressEntry &entry : entries){
            const QHostAddress ip = entry.ip();
            if(ip.protocol() == QAbstractSocket::IPv4Protocol && !ip.isLoopback()){
                // Add IPv4 addresses to the ComboBox
                ip_list << ip.toString();
            }
        }
    }
    ip_list << "127.0.0.1" << "0.0.0.0";
    ip_list.removeDuplicates();
    std::sort(ip_list.begin(), ip_list.end());
    ui->comboBox_IPAddr->addItems(ip_list);
    int idx = ui->comboBox_IPAddr->findText("0.0.0.0");
    if(idx >= 0){
        ui->comboBox_IPAddr->setCurrentIndex(idx);
    }

    // Create a regular expression matching port numbers
    QRegularExpression re("^([1-9][0-9]{0,3}|[1-5][0-9]{4}|6[0-4][0-9]{3}|65[0-4][0-9]{2}|655[0-2][0-9]|6553[0-5])$");
    // Create a QRegularExpressionValidator to restrict input
    auto *validator = new QRegularExpressionValidator(re, this);
    // Set the validator on the QLineEdit
    ui->lineEdit_Port->setValidator(validator);

    connect(ui->btn_clear_log, &QPushButton::clicked, this, &ModbusTcpWidget::sig_clear_log_requested);
}

ModbusTcpWidget::~ModbusTcpWidget()
{
    delete ui;
}

void ModbusTcpWidget::set_gateway_mode(GatewayMode mode)
{
    if(mode == GatewayMode::RtuToTcp){
        ui->label_IPAddr->setText(tr("Target IP:"));
        ui->label_Port->setText(tr("Target Port:"));
        return;
    }
    ui->label_IPAddr->setText(tr("Listen IP:"));
    ui->label_Port->setText(tr("Listen Port:"));
}

modbus_tcp_worker::tcp_params ModbusTcpWidget::get_params() const
{
    modbus_tcp_worker::tcp_params p;
    p.ip = ui->comboBox_IPAddr->currentText();
    p.port = ui->lineEdit_Port->text().toUShort();
    return p;
}
