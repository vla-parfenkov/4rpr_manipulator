#include "mcuconnection.h"

MCUConnection::MCUConnection(QObject *parent) : QObject(parent)
{
    PortSettings settings = {BAUD9600, DATA_8, PAR_NONE, STOP_1, FLOW_OFF, 10};
    port = new QextSerialPort("COM3", settings, QextSerialPort::EventDriven);
}

void MCUConnection::connect()
{

    if (!port->open(QIODevice::ReadWrite))
        throw std::logic_error("Not connected");
}

void MCUConnection::sendCmd(const QString &cmd)
{
    QByteArray data = (cmd + '\n').toLatin1();
    port->write(data.data());
}
