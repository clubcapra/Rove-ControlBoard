#ifndef ADAPTER_CB_ROVE_H
#define ADAPTER_CB_ROVE_H

// Include necessary libraries here
#include "../include/GPIO.h"
#include "../include/LED.h"
#include "../include/Servo/SMS_STS.h"
#include "../include/Servo/pidServo.h"

#define PIN_LED_AVANT 4
#define PIN_LED_ARRIERE 4
#define PIN_STROBE 15
#define PIN_wINCH_lOCK 9

#define PIN_GPIO_1 0
#define PIN_GPIO_2 1
#define PIN_GPIO_3 2

#define SERVO_X 1
#define SERVO_Y 2




class AdapterCBRove {
public:
    // Constructor
    AdapterCBRove();

    // Destructor
    ~AdapterCBRove();

    // Add your member functions here
    void init();

    void task();

    void setServoPosition(int positionX, int positionY);
    void setServoSpeed(int speedX, int speedY);
    void setServoPositionZero();

    int getServoPositionX();
    int getServoPositionY();
    int getServoSpeedX();
    int getServoSpeedY();

    void setLEDFront(bool state);
    void setLEDBack(bool state);
    void setLEDStrobe(bool state);
    void setLockWinch(bool state);
    bool getLEDFront();
    bool getLEDBack();
    bool getLEDStrobe();
    bool getLockWinch();

    void setGPIO1(bool state);
    void setGPIO2(bool state);
    void setGPIO3(bool state);
    bool getGPIO1();
    bool getGPIO2();
    bool getGPIO3();

    private:
        // Add your member variables here
        /*
        uint8_t ID[2] = {SERVO_X, SERVO_Y};
        uint8_t syncWritePosition[2];
        uint8_t syncWriteSpeed[2];
        */

        SMS_STS servo; 
        GPIO gpioC;
        GPIO gpioB;
        GPIO gpioA;
        PIDController pidX;
        PIDController pidY;
        int servoPositionX;
        int servoPositionY;
        int servoSpeedX;
        int servoSpeedY;

        

    

  
};

#endif // ADAPTER_CB_ROVE_H