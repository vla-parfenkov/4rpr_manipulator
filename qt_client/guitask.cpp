#include "guitask.h"
#include <QTextStream>

GUITask::GUITask(QObject *parent) :
    QObject(parent),
    currentX(0),
    currentY(0),
    currentAngle(0)
{
    conn =  std::make_unique<MCUConnection>();
}

void GUITask::handleMove(MoveDirection direction, float speed)
{
    float speedX = 0, speedY = 0, speedThetta = 0;
    switch (direction)
    {
        case MoveRight:
        {
            float newXpossition = currentX + (speed * GUITaskConstant::diskret);
            speedX = speed;
            currentX = newXpossition;
            break;
        }
    case MoveLeft:
    {
        float newXpossition = currentX - (speed * GUITaskConstant::diskret);
        speedX = speed * (-1);
        currentX = newXpossition;
        break;
    }
    case MoveDown:
    {
        float newYpossition = currentY - (speed * GUITaskConstant::diskret);
        speedY = speed * (-1);
        currentY = newYpossition;
        break;
    }
    case MoveUp:
    {
        float newYpossition = currentY + (speed * GUITaskConstant::diskret);
        speedY = speed;
        currentY = newYpossition;
        break;
    }
    case MoveAngleRight:
    {
        float newAnglePosition = currentAngle + (speed * GUITaskConstant::diskret);
        speedThetta = speed;
        currentAngle = newAnglePosition;
        break;
    }
    case MoveAngleLeft:
    {
        float newAnglePosition = currentAngle - (speed * GUITaskConstant::diskret);
        speedThetta = speed * (-1);
        currentAngle = newAnglePosition;
        break;
    }
    default:
        return;
    }
    QString cmd;
    QTextStream out(&cmd);
    out << "[" << currentX << "," << currentY << "," << currentAngle
        << "," << speedX << "," << speedY << ","  << speedThetta
        << "," << QString::number(GUITaskConstant::diskret * 1000) << "]";

    conn->sendCmd(cmd);

    emit changePosition(currentX, currentY, currentAngle);
}

void GUITask::setZero(float X, float Y, float thetta)
{
    currentX = X;
    currentY = Y;
    currentAngle = thetta;
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
