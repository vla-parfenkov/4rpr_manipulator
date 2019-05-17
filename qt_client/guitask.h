#ifndef GUITASK_H
#define GUITASK_H

#include <QObject>
#include <memory>
#include "mcuconnection.h"

namespace GUITaskConstant
{
  static const uint32_t diskret = 2;
}

enum MoveDirection
{
    MoveUp = 0,
    MoveDown = 1,
    MoveLeft = 2,
    MoveRight = 3,
    MoveAngleLeft = 4,
    MoveAngleRight = 5
};

class GUITask : public QObject
{
    Q_OBJECT
public:
    explicit GUITask(QObject *parent = nullptr);
    void handleMove(MoveDirection direction, uint32_t speed);
    void connectToMCU();
    void handleGoToPosition();

signals:
    void changePosition(int32_t x, int32_t y, int32_t angle);
    void errorTask(const QString &err);

public slots:

private:
    int32_t currentX;
    int32_t currentY;
    int32_t currentAngle;
    std::unique_ptr<MCUConnection> conn;


};

#endif // GUITASK_H
