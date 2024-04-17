#ifndef ADAPTER_CB_ROVE_H
#define ADAPTER_CB_ROVE_H

// Include necessary libraries here
#include "../include/GPIO.h"
#include "../include/LED.h"
#include "../include/Servo/SMS_STS.h"
#include "../include/Servo/Servo.h"



class AdapterCBRoveClass {
public:
    // Constructor
    AdapterCBRoveClass();

    // Destructor
    ~AdapterCBRoveClass();

    void init(u8 startID, GPIO& gpio, uint32_t frontLEDPin, uint32_t backLEDPin, uint32_t strobeLEDPin, SMS_STS& st);

    void task();

    bool setServoPosition(int positionX, int positionY);
    bool setServoSpeed(int speedX, int speedY);
    bool setServoPositionZero();
    bool setServoAccX(u8 acc);
    bool setServoAccY(u8 acc);

    int getServoPositionX();
    int getServoPositionY();
    int getServoSpeedX();
    int getServoSpeedY();
    u8 getServoAccX();
    u8 getServoAccY();

    bool setLEDFront(bool state);
    bool setLEDBack(bool state);
    bool setLEDStrobe(bool state);
    bool getLEDFront();
    bool getLEDBack();
    bool getLEDStrobe();

    bool setControlMode(uint32_t mode);
    uint32_t getControlMode();

    Servo& getServoX();
    Servo& getServoY();

    uint32_t getError();
    uint32_t getStatus();

private:
    uint32_t bitsSet(uint32_t val, uint32_t bits);
    uint32_t bitsReset(uint32_t val, uint32_t bits);
    bool checkInitialized();

private:
    volatile bool mInitialized = false;
    volatile u8 mIDs[2] = {0};
    volatile int mServoPositions[2] = {0};
    volatile s16 mSetPositions[2] = {0};
    volatile u16 mSetSpeeds[2] = {0};
    volatile u8 mSetAccs[2] = {0};
    Servo mServoX = {mIDs[0], mServoPositions[0], mSetPositions[0], mSetSpeeds[0], mSetAccs[0]};
    volatile bool mLastXAck = true;
    Servo mServoY = {mIDs[1], mServoPositions[1], mSetPositions[1], mSetSpeeds[1], mSetAccs[1]};
    volatile bool mLastYAck = true;
    SMS_STS *mST;

    GPIO *mGPIO;
    uint32_t mFrontLEDPin;
    volatile bool mFrontLEDState;
    uint32_t mBackLEDPin;
    volatile bool mBackLEDState;
    uint32_t mStrobePin;
    volatile bool mStrobeState;
    volatile uint32_t mStatus = 0;
    volatile uint32_t mError = 0;
    volatile uint32_t mControlMode = 0;
};

extern AdapterCBRoveClass AdapterCBRove;

#endif // ADAPTER_CB_ROVE_H