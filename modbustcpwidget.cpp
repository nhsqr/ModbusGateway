#include "modbustcpwidget.h"
#include "ui_modbustcpwidget.h"
#include "modbusassistant.h"

#include <QNetworkInterface>
#include <QtAlgorithms>

ModbusTcpWidget::ModbusTcpWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ModbusTcpWidget)
{
    ui->setupUi(this);
    ui->comboBox_IPAddr->setEditable(true);
    ui->comboBox_IPAddr->clear();

    const QList<QNetworkInterface> interfaces = QNetworkInterface::allInterfaces();
    for (const QNetworkInterface &iface : interfaces) {
        // Get all IP address entries on this interface
        const QList<QNetworkAddressEntry> entries = iface.addressEntries();
        for (const QNetworkAddressEntry &entry : entries) {
            const QHostAddress ip = entry.ip();
            if (ip.protocol() == QAbstractSocket::IPv4Protocol) {
                // Add IPv4 addresses to the ComboBox
                ui->comboBox_IPAddr->addItem(ip.toString());
            }
        }
    }

    // qSort() is deprecated since Qt 5.15; use std::sort()
    // Use std::sort() on Qt 5.15+

    // Create a regular expression matching port numbers
    QRegularExpression re(QStringLiteral("^([1-9]|[1-9]\\d{1,3}|[1-5]\\d{4}|6[0-4]\\d{3}|65[0-4]\\d{2}|655[0-2]\\d|6553[0-5])$"));
    // Create a QRegularExpressionValidator to restrict input
    QRegularExpressionValidator *validator = new QRegularExpressionValidator(re, this);
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
    if (mode == GatewayMode::RtuToTcp) {
        ui->label_IPAddr->setText(tr("Target IP:"));
        ui->label_Port->setText(tr("Target Port:"));
    } else {
        ui->label_IPAddr->setText(tr("Listen IP:"));
        ui->label_Port->setText(tr("Listen Port:"));
    }
}

QStringList ModbusTcpWidget::get_params() const
{
    QStringList params;
    params << ui->comboBox_IPAddr->currentText()
           << ui->lineEdit_Port->text();
    return params;
}
