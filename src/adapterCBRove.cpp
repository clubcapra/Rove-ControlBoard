


#include "../include/adapterCBRove.h"
#include "api.h"
#include "adapterCBRove.h"


AdapterCBRoveClass::AdapterCBRoveClass()
{
}

AdapterCBRoveClass::~AdapterCBRoveClass()
{
    
}

void AdapterCBRoveClass::init(u8 startID, GPIO& gpio, uint32_t frontLEDPin, uint32_t backLEDPin, uint32_t strobeLEDPin, SMS_STS& st)
{
    // Initialize ids
    for (u8 i = 0, id = startID; i < 2; ++i, ++id)
    {
        mIDs[i] = id;
    }
    mST = &st;
    mFrontLEDPin = frontLEDPin;
    mBackLEDPin = backLEDPin;
    mStrobePin = strobeLEDPin;
    mGPIO = &gpio;
    mInitialized = true;
}

void AdapterCBRoveClass::task()
{
    if (!checkInitialized()) return;
    // Read values
    mServoPositions[0] = mST->ReadPos(mIDs[0]);
    mServoPositions[1] = mST->ReadPos(mIDs[1]);
    
    u8 IDs[2] = {mIDs[0], mIDs[1]};
    s16 SetPositions[2] = {mSetPositions[0], mSetPositions[1]};
    s16 SetSpeeds[2] = {mSetSpeeds[0], mSetSpeeds[1]};
    u16 SetAbsSpeeds[2] = {abs(mSetSpeeds[0]), abs(mSetSpeeds[1])};
    u8 SetAccs[2] = {mSetAccs[0], mSetAccs[1]};

    // Write values
    switch (mControlMode)
    {
    case SCMPosition:
        mST->SyncWritePosEx(IDs, 2, SetPositions, SetAbsSpeeds, SetAccs);
        break;
    case SCMSpeed:
        mLastXAck = mST->WriteSpe(mIDs[0], mSetSpeeds[0], mSetAccs[0]);
        mLastYAck = mST->WriteSpe(mIDs[1], mSetSpeeds[1], mSetAccs[1]);
        break;
    default:
        break;
    }
}

bool AdapterCBRoveClass::setServoPosition(int positionX, int positionY)
{
    if (!checkInitialized()) return false;
    mSetPositions[0] = positionX;
    mSetPositions[1] = positionY;
    return mLastXAck && mLastYAck;
}
bool AdapterCBRoveClass::setServoSpeed(int speedX, int speedY)
{
    if (!checkInitialized()) return false;
    mSetSpeeds[0] = speedX;
    mSetSpeeds[1] = speedY;
    return mLastXAck && mLastYAck;
}
bool AdapterCBRoveClass::setServoPositionZero()
{
    return setServoPosition(0,0);
}
bool AdapterCBRoveClass::setServoAccX(u8 acc)
{
    if (!checkInitialized()) return false;
    mSetAccs[0] = acc;
    return mLastXAck;
}
bool AdapterCBRoveClass::setServoAccY(u8 acc)
{
    if (!checkInitialized()) return false;
    mSetAccs[1] = acc;
    return mLastYAck;
}
int AdapterCBRoveClass::getServoPositionX()
{
    if (!checkInitialized()) return 0;
    return mServoPositions[0];
}
int AdapterCBRoveClass::getServoPositionY()
{
    if (!checkInitialized()) return 0;
    return mServoPositions[1];
}
int AdapterCBRoveClass::getServoSpeedX()
{
    if (!checkInitialized()) return 0;
    return mSetSpeeds[0];
}
int AdapterCBRoveClass::getServoSpeedY()
{
    if (!checkInitialized()) return 0;
    return mSetSpeeds[1];
}
u8 AdapterCBRoveClass::getServoAccX()
{
    if (!checkInitialized()) return 0;
    return mSetAccs[0];
}
u8 AdapterCBRoveClass::getServoAccY()
{
    if (!checkInitialized()) return 0;
    return mSetAccs[1];
}
bool AdapterCBRoveClass::setLEDFront(bool state)
{
    if (!checkInitialized()) return false;
    if (state == mFrontLEDState) return true;
    mFrontLEDState = state;
    mGPIO->writePin(mFrontLEDPin, state);
    return true;
}
bool AdapterCBRoveClass::setLEDBack(bool state)
{
    if (!checkInitialized()) return false;
    if (state == mBackLEDState) return true;
    mBackLEDState = state;
    mGPIO->writePin(mBackLEDPin, state);
    return true;
}
bool AdapterCBRoveClass::setLEDStrobe(bool state)
{
    if (!checkInitialized()) return false;
    if (state == mStrobeState) return true;
    mStrobeState = state;
    mGPIO->writePin(mStrobePin, state);
    return true;
}
bool AdapterCBRoveClass::getLEDFront()
{
    if (!checkInitialized()) return false;
    return mFrontLEDState;
}
bool AdapterCBRoveClass::getLEDBack()
{
    if (!checkInitialized()) return false;
    return mBackLEDState;
}
bool AdapterCBRoveClass::getLEDStrobe()
{
    if (!checkInitialized()) return false;
    return mStrobeState;
}

bool AdapterCBRoveClass::setControlMode(uint32_t mode)
{
    if (!checkInitialized()) return false;
    mControlMode = mode;
    return true;
}

uint32_t AdapterCBRoveClass::getControlMode()
{
    if (!checkInitialized()) return SCMNone;
    return mControlMode;
}

Servo &AdapterCBRoveClass::getServoX()
{
    return mServoX;
}

Servo &AdapterCBRoveClass::getServoY()
{
    return mServoY;
}

uint32_t AdapterCBRoveClass::getError()
{
    return mError;
}

uint32_t AdapterCBRoveClass::getStatus()
{
    return 0;
}

uint32_t AdapterCBRoveClass::bitsSet(uint32_t val, uint32_t bits)
{
    return val | bits;
}

uint32_t AdapterCBRoveClass::bitsReset(uint32_t val, uint32_t bits)
{
    return val & ~bits;
}

bool AdapterCBRoveClass::checkInitialized()
{
    if (mInitialized) 
    {
        mError = bitsReset(mError, ERAdapterNotInit);
        return true;
    }
    mError = bitsSet(mError, ERAdapterNotInit);
    return false;
}

AdapterCBRoveClass AdapterCBRove = AdapterCBRoveClass();