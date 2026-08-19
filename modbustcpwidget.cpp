#include "modbustcpwidget.h"
#include "ui_modbustcpwidget.h"
#include "modbusassistant.h"

#include <QNetworkInterface>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QtAlgorithms>

ModbusTcpWidget::ModbusTcpWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ModbusTcpWidget)
{
    ui->setupUi(this);
    ui->comboBox_IPAddr->setEditable(true);
    setIPAddr();
    setPort();
    set_gateway_mode(GatewayMode::TcpToRtu);
}

ModbusTcpWidget::~ModbusTcpWidget()
{
    delete ui;
}

void ModbusTcpWidget::on_btn_clear_log_clicked()
{
    emit sig_clear_log_requested();
}

void ModbusTcpWidget::setIPAddr()
{
    QList<QString> ipv4Addresses;
    foreach (QNetworkInterface interface, QNetworkInterface::allInterfaces())
    {
        // Get all IP address entries on this interface
        foreach (QNetworkAddressEntry entry, interface.addressEntries()) {
            if (entry.ip().protocol() == QAbstractSocket::IPv4Protocol) {
                // Add IPv4 addresses to the ComboBox
                ipv4Addresses.append(entry.ip().toString());
            }
        }
    }

#if (QT_VERSION < QT_VERSION_CHECK(5, 15, 0))
    qSort(ipv4Addresses);
#else
    std::sort(ipv4Addresses.begin(), ipv4Addresses.end());
#endif
    ui->comboBox_IPAddr->clear();
    foreach (QString ipv4Address, ipv4Addresses) {
        ui->comboBox_IPAddr->addItem(ipv4Address);
    }
}

void ModbusTcpWidget::setPort()
{
    // Create a regular expression matching port numbers
    QRegularExpression regex(ModbusAssistant::m_regExp4PortNumber);
    // Create a QRegularExpressionValidator to restrict input
    QRegularExpressionValidator *validator = new QRegularExpressionValidator(regex, this);
    // Set the validator on the QLineEdit
    ui->lineEdit_Port->setValidator(validator);
}

QStringList ModbusTcpWidget::get_params() const
{
    QStringList params;
    params << ui->comboBox_IPAddr->currentText()
           << ui->lineEdit_Port->text();
    return params;
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
