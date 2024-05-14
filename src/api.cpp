
#include "api.h"
#include "adapterCBRove.h"

#define TRY_RET(ret, ...)           \
    try                             \
    {                               \
        __VA_ARGS__                 \
    }                               \
    catch (const std::exception &e) \
    {                               \
        return ret;                 \
    }
#define TRY_DEF(...) TRY_RET({}, __VA_ARGS__)
#define TRY_RES(...) TRY_RET({false}, __VA_ARGS__)

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
    try
    {
        return {AdapterCBRove.setServoPosition(pos.x, pos.y)};
    }
    catch (const std::exception &e)
    {
        return {false};
    }
}

Vector2D getServoPosition(Void _)
{
    try
    {
        return {AdapterCBRove.getServoPositionX(), AdapterCBRove.getServoPositionY()};
    }
    catch (const std::exception &e)
    {
        return {};
    }
}

Bool_ setServoPositionZero(Void _)
{
    try
    {
        return {AdapterCBRove.setServoPositionZero()};
    }
    catch (const std::exception &e)
    {
        return {false};
    }
}

Bool_ setServoSpeed(Vector2D speed)
{
    try
    {
        return {AdapterCBRove.setServoSpeed(speed.x, speed.y)};
    }
    catch (const std::exception &e)
    {
        return {false};
    }
}

Vector2D getServoSpeed(Void _)
{
    try
    {
        return {AdapterCBRove.getServoSpeedX(), AdapterCBRove.getServoSpeedY()};
    }
    catch (const std::exception &e)
    {
        return {};
    }
}

Short getServoPositionX(Void _)
{
    try
    {
        return {AdapterCBRove.getServoPositionX()};
    }
    catch (const std::exception &e)
    {
        return {};
    }
}

Short getServoPositionY(Void _)
{
    try
    {
        return {AdapterCBRove.getServoPositionY()};
    }
    catch (const std::exception &e)
    {
        return {};
    }
}

Short getServoSpeedX(Void _)
{
    try
    {
        return {AdapterCBRove.getServoSpeedX()};
    }
    catch (const std::exception &e)
    {
        return {};
    }
}

Short getServoSpeedY(Void _)
{
    try
    {
        return {AdapterCBRove.getServoSpeedY()};
    }
    catch (const std::exception &e)
    {
        return {};
    }
}

Bool_ setServoXAcc(Byte acc)
{
    try
    {
        return {AdapterCBRove.setServoAccX(acc.b)};
    }
    catch (const std::exception &e)
    {
        return {false};
    }
}

Bool_ setServoYAcc(Byte acc)
{
    try
    {
        return {AdapterCBRove.setServoAccY(acc.b)};
    }
    catch (const std::exception &e)
    {
        return {false};
    }
}

Byte getServoXAcc(Void _)
{
    try
    {
        return {AdapterCBRove.getServoAccX()};
    }
    catch (const std::exception &e)
    {
        return {};
    }
}

Byte getServoYAcc(Void _)
{
    try
    {
        return {AdapterCBRove.getServoAccY()};
    }
    catch (const std::exception &e)
    {
        return {};
    }
}

Bool_ setLEDFront(Bool_ state)
{
    try
    {
        return {AdapterCBRove.setLEDFront(state.b)};
    }
    catch (const std::exception &e)
    {
        return {false};
    }
}

Bool_ setLEDBack(Bool_ state)
{
    try
    {
        return {AdapterCBRove.setLEDBack(state.b)};
    }
    catch (const std::exception &e)
    {
        return {false};
    }
}

Bool_ setLEDStrobe(Bool_ state)
{
    try
    {
        return {AdapterCBRove.setLEDStrobe(state.b)};
    }
    catch (const std::exception &e)
    {
        return {false};
    }
}

Bool_ getLEDFront(Void _)
{
    try
    {
        return {AdapterCBRove.getLEDFront()};
    }
    catch (const std::exception &e)
    {
        return {false};
    }
}

Bool_ getLEDBack(Void _)
{
    try
    {
        return {AdapterCBRove.getLEDBack()};
    }
    catch (const std::exception &e)
    {
        return {false};
    }
}

Bool_ getLEDStrobe(Void _)
{
    try
    {
        return {AdapterCBRove.getLEDStrobe()};
    }
    catch (const std::exception &e)
    {
        return {false};
    }
}

Report getReport(Void _)
{
    return {getServoPosition({}), AdapterCBRove.getStatusCode(), AdapterCBRove.getErrorCodes()};
}

UShort getWinchMode(Void _)
{
    try
    {
        return {AdapterCBRove.getWinchState()};
    }
    catch (const std::exception &e)
    {
        return {};
    }
}

Bool_ setWinchMode(UShort mode)
{
    try
    {
        return {AdapterCBRove.setWinchState((WinchMode)mode.s)};
    }
    catch (const std::exception &e)
    {
        return {false};
    }
}

Bool_ getWinchLock(Void _)
{
    try
    {
        return {false};
    }
    catch (const std::exception &e)
    {
        return {false};
    }
}

Bool_ setWinchLock(Void _)
{
    try
    {
        return {false};
    }
    catch (const std::exception &e)
    {
        return {false};
    }
}

UShort getServoControlMode(Void _)
{
    try
    {
        return {AdapterCBRove.getControlMode()};
    }
    catch (const std::exception &e)
    {
        return {};
    }
}

Bool_ setServoControlMode(UShort mode)
{
    try
    {
        return {AdapterCBRove.setControlMode((ServoControlMode)mode.s)};
    }
    catch (const std::exception &e)
    {
        return {false};
    }
}

Bool_ getGPIO1(Void _)
{
    try
    {
        return {AdapterCBRove.getGPIO1()};
    }
    catch (const std::exception &e)
    {
        return {};
    }
}

Bool_ setGPIO1(Bool_ state)
{
    try
    {
        return {AdapterCBRove.setGPIO1(state.b)};
    }
    catch (const std::exception &e)
    {
        return {false};
    }
}

Bool_ getGPIO2(Void _)
{
    try
    {
        return {AdapterCBRove.getGPIO2()};
    }
    catch (const std::exception &e)
    {
        return {false};
    }
}

Bool_ setGPIO2(Bool_ state)
{
    try
    {
        return {AdapterCBRove.setGPIO2(state.b)};
    }
    catch (const std::exception &e)
    {
        return {};
    }
}

Bool_ getGPIO3(Void _)
{
    try
    {
        return {AdapterCBRove.getGPIO3()};
    }
    catch (const std::exception &e)
    {
        return {false};
    }
}

Bool_ setGPIO3(Bool_ state)
{
    try
    {
        return {AdapterCBRove.setGPIO3(state.b)};
    }
    catch (const std::exception &e)
    {
        return {false};
    }
}

RGB getRGBLed(Int led)
{
    try
    {
        return AdapterCBRove.getRGBLed(led.i);
    }
    catch (const std::exception &e)
    {
        return {};
    }
}

Bool_ setRGBLed(RGBLed led)
{
    try
    {
        return {AdapterCBRove.setRGBLed(led.index, led.rgb)};
    }
    catch (const std::exception &e)
    {
        return {false};
    }
}

Bool_ configure(Configuration config)
{
    try
    {
        AdapterCBRove.setMinMaxServoX(config.xBounds.lower, config.xBounds.upper);
        AdapterCBRove.setMinMaxServoY(config.yBounds.lower, config.yBounds.upper);
        AdapterCBRove.setStatusCode(StatusCode::STConfigured);
        return {true};
    }
    catch (const std::exception &e)
    {
        return {false};
    }
}
