#ifndef ADAPTER_CB_ROVE_H
#define ADAPTER_CB_ROVE_H

// Include necessary libraries here
#include "../include/GPIO.h"
#include "../include/LED.h"
#include "../include/Servo/servo.h"
#include "../include/winchMotor.h"


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

//#define MAX_POS_X 4095
//#define MIN_POS_X -4095
//#define MAX_POS_Y 4095
//#define MIN_POS_Y -4095



class AdapterCBRove {
public:
    // Constructor
    AdapterCBRove();

    // Destructor
    ~AdapterCBRove();

    // Add your member functions here
    void init();

    void task();

    bool setServoPosition(int positionX, int positionY);
    bool setServoSpeed(int speedX, int speedY);
    bool setServoPositionZero();
    bool setServoAccX(u8 acc);
    bool setServoAccY(u8 acc);

    bool setServoMode(bool modeX,bool modeY); //1 = wheel mode, 0 = joint mode
    bool getServoMode();

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

    bool setWinchState(uint8_t state);
    uint8_t getWinchState();
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

    bool setControlMode(uint32_t mode);
    uint32_t getControlMode();

    Servo& getServoX();
    Servo& getServoY();
private:

    bool checkInitialized();
    void setMinMaxServoX(s16 max, s16 min);
    void setMinMaxServoY(s16 max, s16 min);

private:


    // Add your member variables here
    /*
    uint8_t ID[2] = {SERVO_X, SERVO_Y};
    uint8_t syncWritePosition[2];
    uint8_t syncWriteSpeed[2];
    */

    SMS_STS st; 
    volatile bool mInitialized = false;
    volatile u8 mIDs[2] = {1, 2};
    volatile int mServoPositions[2] = {0};
    
    volatile s16 mSetPositions[2] = {0};
    volatile s16 mSetSpeeds[2] = {0};
    volatile u8 mSetAccs[2] = {0};
    volatile bool mServoModes[2] = {0};

    volatile s16 setPositions[2]={0};
    volatile s16 setSpeeds[2]={0};
    volatile u8 setAccs[2]={0};

    Servo mServoX = {mIDs[0], mServoPositions[0], mSetPositions[0], mSetSpeeds[0], mSetAccs[0]};
    //volatile bool mLastXAck = true;
    Servo mServoY = {mIDs[1], mServoPositions[1], mSetPositions[1], mSetSpeeds[1], mSetAccs[1]};
    //volatile bool mLastYAck = true;

    s16 mMaxPosX[2] = {0}; //0:max, 1:min
    s16 mMaxPosY[2] = {0}; //0:max, 1:min

    WinchMotor motorWhitch;
    uint8_t mWinchState = 0;
    GPIO gpioC;
    GPIO gpioB;
    GPIO gpioA;
    volatile uint32_t mControlMode = 0;
    volatile uint32_t mError = 0;
        

    

  
};



#endif // ADAPTER_CB_ROVE_H