#include "guitask.h"

GUITask::GUITask(QObject *parent) :
    QObject(parent),
    currentX(0),
    currentY(0),
    currentAngle(0)
{

}

void GUITask::handleMove(MoveDirection direction, uint32_t speed)
{
    switch (direction)
    {
        case MoveUp:
        {
            int32_t newXpossition = currentX + static_cast<int32_t>(speed * GUITaskConstant::diskret);
            //sendCmd
            currentX = newXpossition;
            break;
        }
    case MoveDown:
    {
        int32_t newXpossition = currentX - static_cast<int32_t>(speed * GUITaskConstant::diskret);
        //sendCmd
        currentX = newXpossition;
        break;
    }
    case MoveLeft:
    {
        int32_t newYpossition = currentY - static_cast<int32_t>(speed * GUITaskConstant::diskret);
        //sendCmd
        currentY = newYpossition;
        break;
    }
    case MoveRight:
    {
        int32_t newYpossition = currentY + static_cast<int32_t>(speed * GUITaskConstant::diskret);
        //sendCmd
        currentY = newYpossition;
        break;
    }
    case MoveAngleRight:
    {
        int32_t newAnglePosition = currentAngle + static_cast<int32_t>(speed * GUITaskConstant::diskret);
        //sendCmd
        currentAngle = newAnglePosition;
        break;
    }
    case MoveAngleLeft:
    {
        int32_t newAnglePosition = currentAngle - static_cast<int32_t>(speed * GUITaskConstant::diskret);
        //sendCmd
        currentAngle = newAnglePosition;
        break;
    }
    default:
        return;
    }
    emit changePosition(currentX, currentY, currentAngle);
}
