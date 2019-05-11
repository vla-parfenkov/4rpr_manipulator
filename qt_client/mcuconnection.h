#ifndef MCUCONNECTION_H
#define MCUCONNECTION_H

#include <QObject>
#include "qextserialport.h"


class MCUConnection : public QObject
{
    Q_OBJECT
public:
    explicit MCUConnection(QObject *parent = nullptr);
    void connect();
    void  sendCmd(const QString &cmd);
private:
    QextSerialPort *port;

signals:

public slots:
};

#endif // MCUCONNECTION_H
