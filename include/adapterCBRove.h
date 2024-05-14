#ifndef ADAPTER_CB_ROVE_H
#define ADAPTER_CB_ROVE_H

// Include necessary libraries here
#include "GPIO.h"
#include "Servo/Servo.h"
#include "winchMotor.h"
#include "ARGB.h"
#include <string>


#define PIN_LED_AVANT 4
#define PIN_LED_ARRIERE 4
#define PIN_STROBE 15


#define PIN_WINCH_DIR1 3
#define PIN_WINCH_DIR2 4
#define PIN_WINCH_LOCK1 10
#define PIN_WINCH_LOCK2 2


#define PIN_GPIO_1 0
#define PIN_GPIO_2 1
#define PIN_GPIO_3 2

#define SERVO_X 1
#define SERVO_Y 2

#define LED_COUNT 5

//#define MAX_POS_X 4095
//#define MIN_POS_X -4095
//#define MAX_POS_Y 4095
//#define MIN_POS_Y -4095

class ServoNACKException : public std::exception
{
public:
    bool xServo;
    bool yServo;
    ServoNACKException(bool xServo, bool yServo)
    {
        this->xServo = xServo;
        this->yServo = yServo;
    }

    const char* what() const throw()
    {
        if (this->xServo && this->yServo) return "Global servo failed to ack";
        if (this->xServo) return "Servo x failed to ack";
        if (this->yServo) return "Servo y failed to ack";
        return "Unknown servo ack error";
    }
};

class AdapterNotInitializedException : public std::exception
{
public:
    AdapterNotInitializedException(){}
    const char* what() const throw() { return "Adapter not initialized"; }
};

class RGBIndexOutOfRangeException : public std::exception
{
public:
    int index;
    RGBIndexOutOfRangeException(int index)
    {
        this->index = index;
    }
    const char* what() const throw()
    {
        auto res = std::string("Led index out of range. Must be between 0 and ") +
            std::to_string(LED_COUNT) +
            " but was " +
            std::to_string(this->index);
        return res.c_str();
    }
};

class RGBException : public std::exception
{
public:
    ARGB_STATE state;
    RGBException(ARGB_STATE state)
    {
        this->state = state;
    }
    const char* what() const throw()
    {
        switch (state)
        {
        case ARGB_BUSY:
            return "ARGB was busy";
        case ARGB_PARAM_ERR:
            return "ARGB had a parameter error";
        default:
            return "No error, check your code";
        }
    }
};

class AdapterCBRoveClass {
private:

    bool checkInitialized();



    // Add your member variables here
    /*
    uint8_t ID[2] = {SERVO_X, SERVO_Y};
    uint8_t syncWritePosition[2];
    uint8_t syncWriteSpeed[2];
    */

    SMS_STS st; 
    volatile bool mInitialized = false;
    u8 mIDs[2] = {SERVO_X, SERVO_Y};
    s16 mServoPositions[2] = {0};
    
    volatile s16 mSetPositions[2] = {0};
    volatile s16 mSetSpeeds[2] = {0};
    volatile u8 mSetAccs[2] = {0};
    volatile bool mServoModes[2] = {0};

    s16 setPositions[2]={0};
    s16 setSpeeds[2]={1000};
    u8 setAccs[2]={100};

    
    //volatile bool mLastXAck = true;
    
    //volatile bool mLastYAck = true;

    s16 mMaxPosX[2] = {0}; //0:max, 1:min
    s16 mMaxPosY[2] = {0}; //0:max, 1:min

    WinchMotor motorWhitch;
    WinchMode mWinchState = WMFreeWheel;
    GPIO gpioC;
    GPIO gpioB;
    GPIO gpioA;
    //RGB setRGBLed;
    RGB mLeds[LED_COUNT] = {};

    volatile ServoControlMode mControlMode = SCMPosition;
    volatile ErrorCode mError = ErrorCode::ERNone;
    volatile StatusCode mStatus = StatusCode::STNotInitialized;

public:
    // Constructor
    AdapterCBRoveClass();
    AdapterCBRoveClass(const AdapterCBRoveClass&) = delete;
    AdapterCBRoveClass(AdapterCBRoveClass&&) = default;

    AdapterCBRoveClass &operator=(const AdapterCBRoveClass& other);

    // Destructor
    ~AdapterCBRoveClass();

    
    void init(UART_HandleTypeDef *huartServo);

    void task();

    bool setServoPosition(s16 positionX, s16 positionY);
    bool setServoSpeed(s16 speedX, s16 speedY);
    bool setServoPositionZero();
    bool setServoAccX(u8 acc);
    bool setServoAccY(u8 acc);
    void setMinMaxServoX(s16 max, s16 min);
    void setMinMaxServoY(s16 max, s16 min);

    bool setServoMode(bool mode); //1 = wheel mode, 0 = joint mode
    bool getServoMode();

    s16 getServoPositionX();
    s16 getServoPositionY();
    s16 getServoSpeedX();
    s16 getServoSpeedY();
    u8 getServoAccX();
    u8 getServoAccY();

    bool setLEDFront(bool state);
    bool setLEDBack(bool state);
    bool setLEDStrobe(bool state);
    
    bool getLEDFront();
    bool getLEDBack();
    bool getLEDStrobe();

    bool setWinchState(WinchMode state);
    WinchMode getWinchState();
    bool setLockWinch1(bool state);
    bool setLockWinch2(bool state);
    bool getLockWinch1();
    bool getLockWinch2();

    bool setGPIO1(bool state);
    bool setGPIO2(bool state);
    bool setGPIO3(bool state);

    bool getGPIO1();
    bool getGPIO2();
    bool getGPIO3();

    bool setControlMode(ServoControlMode mode);
    ServoControlMode getControlMode();

    RGB getRGBLed(int led);

    bool setRGBLed(int led, RGB color);

    //Servo& getServoX();
    //Servo& getServoY();

    StatusCode getStatusCode() const;
    void setStatusCode(StatusCode status);

    ErrorCode getErrorCodes() const;
    void clearErrorCodes();
    bool getErrorCode(ErrorCode error) const;
    void setErrorCode(ErrorCode error);
    void unsetErrorCode(ErrorCode error);
};

extern AdapterCBRoveClass AdapterCBRove;

#endif // ADAPTER_CB_ROVE_H