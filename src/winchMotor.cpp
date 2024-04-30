
#include "../include/winchMotor.h"


WinchMotor::WinchMotor()
{
    
}
WinchMotor::WinchMotor(const WinchMotor& winchMotor)
{
    gpio = winchMotor.gpio;
}
WinchMotor &WinchMotor::operator=(const WinchMotor& winchMotor)
{
    gpio = winchMotor.gpio;
    return *this;
}

WinchMotor::WinchMotor(GPIO gpioWinch)
{
    gpio = gpioWinch;
}
void WinchMotor::init(uint8_t dirPin1, uint8_t dirPin2, uint8_t pinWinchLock1, uint8_t pinWinchLock2)
{
    dirP1 = dirPin1;
    dirP2 = dirPin2;
    pWinchLock1 = pinWinchLock1;
    pWinchLock2 = pinWinchLock2;
    gpio.setPinMode(dirP1,OUTPUT).setPinOutputType(dirP1, PUSH_PULL).setPinSpeed(dirP1, MEDIUM_SPEED).setPinPad(dirP1,PULL_UP);
    gpio.setPinMode(dirP2,OUTPUT).setPinOutputType(dirP2, PUSH_PULL).setPinSpeed(dirP2, MEDIUM_SPEED).setPinPad(dirP2,PULL_UP);
    gpio.setPinMode(pWinchLock1,OUTPUT).setPinOutputType(pWinchLock1, PUSH_PULL).setPinSpeed(pWinchLock1, MEDIUM_SPEED).setPinPad(pWinchLock1,PULL_UP);
    gpio.setPinMode(pWinchLock2,OUTPUT).setPinOutputType(pWinchLock2, PUSH_PULL).setPinSpeed(pWinchLock2, MEDIUM_SPEED).setPinPad(pWinchLock2,PULL_UP);
    setWinchLock1(1);
    setWinchLock2(1);
    winchMotorBreak();

}
    
void WinchMotor::winchMotorForward()
{
    gpio.writePin(dirP1, 1).writePin(dirP2, 0);
    
}

void WinchMotor::winchMotorReverse()
{
    gpio.writePin(dirP1, 0).writePin(dirP2, 1);
    
}
void WinchMotor::winchMotorBreak()
{
    gpio.writePin(dirP1, 1).writePin(dirP2, 1);
    
}
void WinchMotor::winchMotorFreeWheel()
{
    gpio.writePin(dirP1, 0).writePin(dirP2, 0);
}

void WinchMotor::setWinchLock1(bool state)
{
    gpio.writePin(pWinchLock1, state);
}

void WinchMotor::setWinchLock2(bool state)
{
    gpio.writePin(pWinchLock2, state);
}

bool WinchMotor::getWinchLock1()
{
    switch (gpio.readPin(pWinchLock1))
     {
     case HIGH:
          return true;
          break;
     case LOW:
          return false;
          break;
     default:
          break;
     }
    return false;
}

bool WinchMotor::getWinchLock2()
{
    switch (gpio.readPin(pWinchLock2))
     {
     case HIGH:
          return true;
          break;
     case LOW:
          return false;
          break;
     default:
          break;
     }
    return false;
    
}