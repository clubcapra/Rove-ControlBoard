#ifndef ADAPTER_CB_ROVE_H
#define ADAPTER_CB_ROVE_H

// Include necessary libraries here
#include "../include/GPIO.h"
#include "../include/LED.h"
#include "../include/Servo/SMS_STS.h"

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
    bool getLEDFront();
    bool getLEDBack();
    bool getLEDStrobe();

    private:
        // Add your member variables here
        //GPIO gpioC;
        //GPIO gpioB;

    

  
};

#endif // ADAPTER_CB_ROVE_H