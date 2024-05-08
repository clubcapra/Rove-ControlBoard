
#include "api.h"
#include "adapterCBRove.h"

Int ping(Int i)
{
    return {i.i};
}

ULong hashCheck(Void _)
{
    return {API_HASH};
}

Bool_ setServoPosition(Vector2D pos)
{
    return { AdapterCBRove.setServoPosition(pos.x, pos.y) };
}

Vector2D getServoPosition(Void _)
{
    return { AdapterCBRove.getServoPositionX(), AdapterCBRove.getServoPositionY() };
}

Bool_ setServoPositionZero(Void _)
{
    return { AdapterCBRove.setServoPositionZero() };
}

Bool_ setServoSpeed(Vector2D speed)
{
    return { AdapterCBRove.setServoSpeed(speed.x, speed.y) };
}

Vector2D getServoSpeed(Void _)
{
    return { AdapterCBRove.getServoSpeedX(), AdapterCBRove.getServoSpeedY() };
}

Int getServoPositionX(Void _)
{
    return { AdapterCBRove.getServoPositionX() };
}

Int getServoPositionY(Void _)
{
    return { AdapterCBRove.getServoPositionY() };
}

Int getServoSpeedX(Void _)
{
    return { AdapterCBRove.getServoSpeedX() };
}

Int getServoSpeedY(Void _)
{
    return { AdapterCBRove.getServoSpeedY() };
}

Bool_ setServoXAcc(Byte acc)
{
    return { AdapterCBRove.setServoAccX(acc.b)};
}

Bool_ setServoYAcc(Byte acc)
{
    return { AdapterCBRove.setServoAccY(acc.b)};
}

Byte getServoXAcc(Void _)
{
    return { AdapterCBRove.getServoAccX() };
}

Byte getServoYAcc(Void _)
{
    return { AdapterCBRove.getServoAccY() };
}

Bool_ setLEDFront(Bool_ state)
{
    AdapterCBRove.setLEDFront(state.b);
    return { true };
}

Bool_ setLEDBack(Bool_ state)
{
    AdapterCBRove.setLEDBack(state.b);
    return { true };
}

Bool_ setLEDStrobe(Bool_ state)
{
    AdapterCBRove.setLEDStrobe(state.b);
    return { true };
}

Bool_ getLEDFront(Void _)
{
    return { AdapterCBRove.getLEDFront() };
}

Bool_ getLEDBack(Void _)
{
    return { AdapterCBRove.getLEDBack() };
}

Bool_ getLEDStrobe(Void _)
{
    return { AdapterCBRove.getLEDStrobe() };
}

Report getReport(Void _)
{
    return { getServoPosition({}), 0, 0 };
}


