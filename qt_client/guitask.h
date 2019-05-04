#ifndef GUITASK_H
#define GUITASK_H

#include <QObject>

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
    void handleMove(MoveDirection direction, uint32_t speed);

signals:
    void changePosition(int32_t x, int32_t y, int32_t angle);

public slots:

private:
    int32_t currentX;
    int32_t currentY;
    int32_t currentAngle;


};

#endif // GUITASK_H
