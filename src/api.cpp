
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

Short getServoPositionX(Void _)
{
    return { AdapterCBRove.getServoPositionX() };
}

Short getServoPositionY(Void _)
{
    return { AdapterCBRove.getServoPositionY() };
}

Short getServoSpeedX(Void _)
{
    return { AdapterCBRove.getServoSpeedX() };
}

Short getServoSpeedY(Void _)
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
    return { getServoPosition({}), AdapterCBRove.getStatusCode(), AdapterCBRove.getErrorCode() };
}

UShort getWinchMode(Void _)
{
    return { AdapterCBRove.getWinchState() };
}

Bool_ setWinchMode(UShort mode)
{
    return {AdapterCBRove.setWinchState((WinchMode)mode.s)};
}

Bool_ getWinchLock(Void _)
{
    //return { AdapterCBRove.getWinchLock() };
    return { false };
}

Bool_ setWinchLock(Void _)
{
    //return { AdapterCBRove.setWinchLock() };
    return { false };
}

UShort getServoControlMode(Void _)
{
    return { AdapterCBRove.getControlMode() };
}

Bool_ setServoControlMode(UShort mode)
{
    return {AdapterCBRove.setControlMode((ServoControlMode)mode.s)};
}

Bool_ getGPIO1(Void _)
{
    return { AdapterCBRove.getGPIO1() };
}

Bool_ setGPIO1(Bool_ state)
{
    AdapterCBRove.setGPIO1(state.b);
    return { true };
}

Bool_ getGPIO2(Void _)
{
    return { AdapterCBRove.getGPIO2() };
}

Bool_ setGPIO2(Bool_ state)
{
    AdapterCBRove.setGPIO2(state.b);
    return { true };
}

Bool_ getGPIO3(Void _)
{
    return { AdapterCBRove.getGPIO3() };
}

Bool_ setGPIO3(Bool_ state)
{
    AdapterCBRove.setGPIO3(state.b);
    return { true };
}

RGB getRGBLed(Int led)
{
    return {AdapterCBRove.getRGBLed(led)};
}

Bool_ setRGBLed(RGBLed color)
{
    return { AdapterCBRove.setRGBLed(color) };
}

Bool_ configure(Configuration config)
{
    AdapterCBRove.setMinMaxServoX(config.xBounds.lower, config.xBounds.upper);
    AdapterCBRove.setMinMaxServoY(config.yBounds.lower, config.yBounds.upper);
    AdapterCBRove.setStatusCode(StatusCode::STConfigured);
    return {true};
}

