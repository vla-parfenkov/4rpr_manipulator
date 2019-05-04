#ifndef MCUCONNECTION_H
#define MCUCONNECTION_H

#include <QObject>

class MCUConnection : public QObject
{
    Q_OBJECT
public:
    explicit MCUConnection(QObject *parent = nullptr);
    void connect();
    void  sendCmd();

signals:

public slots:
};

#endif // MCUCONNECTION_H
