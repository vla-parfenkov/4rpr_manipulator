#include "guitask.h"

GUITask::GUITask(QObject *parent) :
    QObject(parent),
    currentX(0),
    currentY(0),
    currentAngle(0)
{
    conn =  std::make_unique<MCUConnection>();
}

void GUITask::handleMove(MoveDirection direction, uint32_t speed)
{
    switch (direction)
    {
        case MoveUp:
        {
            int32_t newXpossition = currentX + static_cast<int32_t>(speed * GUITaskConstant::diskret);
            QString cmd = "[" + QString::number(newXpossition) + ",0,0,"
                    + QString::number(speed) + ",0,0," + QString::number(GUITaskConstant::diskret * 1000) + "]";
            conn->sendCmd(cmd);
            currentX = newXpossition;
            break;
        }
    case MoveDown:
    {
        int32_t newXpossition = currentX - static_cast<int32_t>(speed * GUITaskConstant::diskret);
        QString cmd = "[" + QString::number(newXpossition) + ",0,0,"
                + QString::number(speed) + ",0,0," + QString::number(GUITaskConstant::diskret * 1000) + "]";
        conn->sendCmd(cmd);
        currentX = newXpossition;
        break;
    }
    case MoveLeft:
    {
        int32_t newYpossition = currentY - static_cast<int32_t>(speed * GUITaskConstant::diskret);
        QString cmd = "[0," + QString(newYpossition) + ",0,0,"
                + QString(speed) + ",0," + QString(GUITaskConstant::diskret) + "]";
        conn->sendCmd(cmd);
        currentY = newYpossition;
        break;
    }
    case MoveRight:
    {
        int32_t newYpossition = currentY + static_cast<int32_t>(speed * GUITaskConstant::diskret);
        QString cmd = "[0," + QString(newYpossition) + ",0,0,"
                + QString(speed) + ",0," + QString(GUITaskConstant::diskret) + "]";
        conn->sendCmd(cmd);
        currentY = newYpossition;
        break;
    }
    case MoveAngleRight:
    {
        int32_t newAnglePosition = currentAngle + static_cast<int32_t>(speed * GUITaskConstant::diskret);
        QString cmd = "[0,0," + QString(newAnglePosition) + ",0,0,"
                + QString(speed) + "," + QString(GUITaskConstant::diskret) + "]";
        conn->sendCmd(cmd);
        currentAngle = newAnglePosition;
        break;
    }
    case MoveAngleLeft:
    {
        int32_t newAnglePosition = currentAngle - static_cast<int32_t>(speed * GUITaskConstant::diskret);
        QString cmd = "[0,0," + QString(newAnglePosition) + ",0,0,"
                + QString(speed) + "," + QString(GUITaskConstant::diskret) + "]";
        conn->sendCmd(cmd);
        currentAngle = newAnglePosition;
        break;
    }
    default:
        return;
    }
    emit changePosition(currentX, currentY, currentAngle);
}

void GUITask::connectToMCU()
{
    try {
        conn->connect();
    } catch (const std::logic_error &ex)
    {
        emit errorTask(ex.what());
    }

}

void GUITask::handleGoToPosition()
{
    conn->sendCmd("TestUsart");
}
