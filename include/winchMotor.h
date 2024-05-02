// winchMotor.h

#ifndef WINCHMOTOR_H
#define WINCHMOTOR_H
#include "stdio.h"
#include "GPIO.h"


class WinchMotor {
public:
    WinchMotor();
    WinchMotor(const WinchMotor& winchMotor);
    WinchMotor &operator=(const WinchMotor& winchMotor);
    WinchMotor(GPIO gpioWinch);

    void init(uint8_t dirPin1, uint8_t dirPin2, uint8_t pinWinchLock1, uint8_t pinWinchLock2);
    void winchMotorForward();
    void winchMotorReverse();
    void winchMotorBreak();
    void winchMotorFreeWheel();

    void setWinchLock1(bool state);
    void setWinchLock2(bool state);
    bool getWinchLock1();
    bool getWinchLock2();

private:
    
    uint8_t dirP1;
    uint8_t dirP2;
    uint8_t pWinchLock1;
    uint8_t pWinchLock2;
    GPIO gpio;
};

#endif