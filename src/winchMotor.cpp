
#include "winchMotor.h"


/**
 * \brief Default constructor for the WinchMotor class.
 *
 * This constructor initializes a new instance of the WinchMotor class.
 */
WinchMotor::WinchMotor()
{
    
}


/**
 * @brief Constructs a new WinchMotor object by copying another WinchMotor object.
 *
 * @param winchMotor The WinchMotor object to be copied.
 */
WinchMotor::WinchMotor(const WinchMotor& winchMotor)
{
    gpio = winchMotor.gpio;
}


/**
 * @brief Assignment operator for the WinchMotor class.
 *
 * This operator assigns the values of the given `WinchMotor` object to the current object.
 *
 * @param winchMotor The `WinchMotor` object to be assigned.
 * @return A reference to the current `WinchMotor` object after assignment.
 */
WinchMotor &WinchMotor::operator=(const WinchMotor& winchMotor)
{
    gpio = winchMotor.gpio;
    return *this;
}

/**
 * @brief Constructs a WinchMotor object.
 *
 * This constructor initializes a WinchMotor object with the specified GPIO pin.
 *
 * @param gpioWinch The GPIO pin used for controlling the winch motor.
 */
WinchMotor::WinchMotor(GPIO gpioWinch)
{
    gpio = gpioWinch;
}


/**
 * @brief Initializes the WinchMotor object.
 * 
 * This function sets up the WinchMotor object by configuring the direction pins and winch lock pins.
 * 
 * @param dirPin1 The pin number for direction control 1.
 * @param dirPin2 The pin number for direction control 2.
 * @param pinWinchLock1 The pin number for winch lock control 1.
 * @param pinWinchLock2 The pin number for winch lock control 2.
 */
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
    
/**
 * Moves the winch motor forward.
 */
void WinchMotor::winchMotorForward()
{
    gpio.writePin(dirP1, 1).writePin(dirP2, 0);
    
}

/**
 * Reverses the direction of the winch motor.
 */
void WinchMotor::winchMotorReverse()
{
    gpio.writePin(dirP1, 0).writePin(dirP2, 1);
    
}

/**
 * Stops the winch motor.
 */
void WinchMotor::winchMotorBreak()
{
    gpio.writePin(dirP1, 1).writePin(dirP2, 1);
    
}

/**
 * Freewheels the winch motor.
 */
void WinchMotor::winchMotorFreeWheel()
{
    gpio.writePin(dirP1, 0).writePin(dirP2, 0);
}

/**
 * Sets the state of the winch lock 1.
 *
 * @param state The state to set the winch lock 1 to.
 */
void WinchMotor::setWinchLock1(bool state)
{
    gpio.writePin(pWinchLock1, state);
}

/**
 * Sets the state of the winch lock.
 *
 * @param state The state to set the winch lock to.
 */
void WinchMotor::setWinchLock2(bool state)
{
    gpio.writePin(pWinchLock2, state);
}

/**
 * @brief Gets the status of Winch Lock 1.
 * 
 * This function returns the status of Winch Lock 1.
 * 
 * @return The status of Winch Lock 1.
 */
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

/**
 * @brief Gets the status of the winch lock 2.
 * 
 * @return true if the winch lock 2 is engaged, false otherwise.
 */
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