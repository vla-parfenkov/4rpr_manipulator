#ifndef GUITASK_H
#define GUITASK_H

#include <QObject>
#include <memory>
#include "mcuconnection.h"

namespace GUITaskConstant
{
  static const uint32_t diskret = 1;
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
    void handleMove(MoveDirection direction, float speed);
    void setZero(float X, float Y, float thetta);
    void connectToMCU();
    void handleGoToPosition();

signals:
    void changePosition(float x, float y, float angle);
    void errorTask(const QString &err);

public slots:

private:
    float currentX;
    float currentY;
    float currentAngle;
    std::unique_ptr<MCUConnection> conn;

};

#endif // GUITASK_H
