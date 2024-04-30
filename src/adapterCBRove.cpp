


/**
 * @file adapterCBRove.cpp
 * @brief Implementation file for the AdapterCBRove class.
 *
 * This file contains the implementation of the AdapterCBRove class, which is responsible for
 * controlling the communication between the ControleBoard and the adapter.
 */
#include "../include/adapterCBRove.h"




/**
 * @brief Constructs a new instance of the AdapterCBRove class.
 *
 * This constructor initializes a new instance of the AdapterCBRove class.
 * It is responsible for setting up the necessary resources and preparing
 * the adapter for communication with the ControleBoard.
 */
AdapterCBRove::AdapterCBRove()
{
     gpioC=GPIO((uint32_t *)(0x40020800UL));
     gpioB=GPIO((uint32_t *)(0x40020400UL));
     gpioA=GPIO((uint32_t *)(0x40020000UL));
     motorWhitch=WinchMotor(gpioA);
}

/**
 * @brief Destructor for the AdapterCBRove class.
 * 
 * This destructor is responsible for cleaning up any resources
 * allocated by the AdapterCBRove class.
 */
AdapterCBRove::~AdapterCBRove()
{
    
}


/**
 * Initializes the AdapterCBRove object.
 */
void AdapterCBRove::init()
{
    

     gpioB.setPinMode(PIN_LED_ARRIERE,OUTPUT).setPinOutputType(PIN_LED_ARRIERE, PUSH_PULL).setPinSpeed(PIN_LED_ARRIERE, MEDIUM_SPEED).setPinPad(PIN_LED_ARRIERE,PULL_UP);
	gpioC.setPinMode(PIN_LED_AVANT,OUTPUT).setPinOutputType(PIN_LED_AVANT, PUSH_PULL).setPinSpeed(PIN_LED_AVANT, MEDIUM_SPEED).setPinPad(PIN_LED_AVANT,PULL_UP);
	gpioA.setPinMode(PIN_STROBE,OUTPUT).setPinOutputType(PIN_STROBE, PUSH_PULL).setPinSpeed(PIN_STROBE, MEDIUM_SPEED).setPinPad(PIN_STROBE,PULL_UP);
	gpioC.setPinMode(PIN_GPIO_1,OUTPUT).setPinOutputType(PIN_GPIO_1, PUSH_PULL).setPinSpeed(PIN_GPIO_1, MEDIUM_SPEED).setPinPad(PIN_GPIO_1,PULL_UP);
	gpioC.setPinMode(PIN_GPIO_2,OUTPUT).setPinOutputType(PIN_GPIO_2, PUSH_PULL).setPinSpeed(PIN_GPIO_2, MEDIUM_SPEED).setPinPad(PIN_GPIO_2,PULL_UP);
	gpioC.setPinMode(PIN_GPIO_3,OUTPUT).setPinOutputType(PIN_GPIO_3, PUSH_PULL).setPinSpeed(PIN_GPIO_3, MEDIUM_SPEED).setPinPad(PIN_GPIO_3,PULL_UP);

	motorWhitch.init(PIN_WINCH_DIR1, PIN_WINCH_DIR2, PIN_WINCH_LOCK1, PIN_WINCH_LOCK2);



     gpioB.writePin(PIN_LED_ARRIERE, 0);
     gpioC.writePin(PIN_LED_AVANT, 0);
     gpioA.writePin(PIN_STROBE, 0);

     gpioC.writePin(PIN_GPIO_1, 0);
     gpioC.writePin(PIN_GPIO_2, 0);
     gpioC.writePin(PIN_GPIO_3, 0);

     setMinMaxServoX(4095, -4095);

     //@todo: set min max servo Y
     setMinMaxServoY(4095, -4095);


     mInitialized = true;

}



/**
 * Executes the task for the AdapterCBRove class.
 * This function is responsible for performing the necessary operations for the AdapterCBRove class.
 * It should be called periodically to ensure the proper functioning of the class.
 */
void AdapterCBRove::task()
{
     if (!checkInitialized()) return;
     mServoPositions[0] = st.ReadPos(mIDs[0]);
     mServoPositions[1] = st.ReadPos(mIDs[1]);
     
     setPositions[0] = mSetPositions[0];
     setPositions[1] = mSetPositions[1];
     setSpeeds[0] = mSetSpeeds[0];
     setSpeeds[1] = mSetSpeeds[1];
     setAccs[0] = mSetAccs[0];
     setAccs[1] = mSetAccs[1];

     switch (mControlMode)
     {
     case 0:
          if(mServoPositions[0]<=mSetPositions[0]+5 && mServoPositions[0]>=mSetPositions[0]-5
          && mServoPositions[1]<=mSetPositions[1]+5 && mServoPositions[1]>=mSetPositions[1]-5);
          else
               st.WritePosEx(mIDs[0], setPositions[0], setSpeeds[0], setAccs[0]);
          break;
     case 1:

          if(mServoPositions[0] >= mMaxPosX[0] || mServoPositions[0] <= mMaxPosX[1])
          {
               setSpeeds[0] = 0;
               setAccs[0] = 0;
          }
          st.WriteSpe(mIDs[0], setSpeeds[0], setAccs[0]);
          if(mServoPositions[1] >= mMaxPosY[0] || mServoPositions[1] <= mMaxPosY[1])
          {
               setSpeeds[1] = 0;
               setAccs[1] = 0;
          }
          st.WriteSpe(mIDs[1], setSpeeds[1], setAccs[1]);
          break;
     default:
          break;
     } 

     

}


/**
 * @brief Sets the minimum and maximum values for the servo X.
 *
 * This function sets the minimum and maximum values for the servo X.
 *
 * @param max The maximum value for the servo X.
 * @param min The minimum value for the servo X.
 */
void AdapterCBRove::setMinMaxServoX(s16 max, s16 min)
{
     mMaxPosX[0] = max;
     mMaxPosX[1] = min;
}


/**
 * @brief Sets the minimum and maximum values for the servo Y.
 *
 * This function sets the minimum and maximum values for the servo Y.
 *
 * @param max The maximum value for the servo Y.
 * @param min The minimum value for the servo Y.
 */
void AdapterCBRove::setMinMaxServoY(s16 max, s16 min)
{
     mMaxPosY[0] = max;
     mMaxPosY[1] = min;
}


/**
 * Sets the position of the servo motor.
 *
 * This function sets the position of the servo motor connected to the AdapterCBRove object.
 * The position is specified by the `positionX` and `positionY` parameters.
 *
 * @param positionX The desired position of the servo motor along the X-axis.
 * @param positionY The desired position of the servo motor along the Y-axis.
 * @return True if the servo position was set successfully, false otherwise.
 */
bool AdapterCBRove::setServoPosition(int positionX, int positionY)
{
     if (!checkInitialized()) return false;
     if(positionX >= mMaxPosX[0] )
          mSetPositions[0]=mMaxPosX[0];
     else if(positionX < mMaxPosX[1])
          mSetPositions[0]= mMaxPosX[1];
     else
          mSetPositions[0] = positionX;
     
     if(positionY >=  mMaxPosY[0] )     
          mSetPositions[1]=mMaxPosY[0];
     else if(positionY < mMaxPosY[1])
          mSetPositions[1]=mMaxPosY[1];
     else
          mSetPositions[1] = positionY;

     return true;
}


/**
 * Sets the servo position to zero.
 *
 * This function sets the position of the servo to zero, which is the neutral position.
 * It can be used to reset the servo to its initial position.
 *
 * @return true if the servo position was successfully set to zero, false otherwise.
 */
bool AdapterCBRove::setServoPositionZero()
{
     return setServoPosition(0,0);
}


/**
 * Sets the speed of the servo motors.
 *
 * This function sets the speed of the servo motors in the X and Y directions.
 *
 * @param speedX The speed value for the X direction.
 * @param speedY The speed value for the Y direction.
 * @return True if the speed was set successfully, false otherwise.
 */
bool AdapterCBRove::setServoSpeed(int speedX, int speedY)
{
     if (!checkInitialized()) return false;
     mSetSpeeds[0] = speedX;
     mSetSpeeds[1] = speedY;
     return true;
}


/**
 * Sets the acceleration value for the X-axis servo.
 *
 * @param acc The acceleration value to set.
 * @return True if the acceleration value was set successfully, false otherwise.
 */
bool AdapterCBRove::setServoAccX(u8 acc)
{
     if (!checkInitialized()) return false;
     mSetAccs[0] = acc;
     return true;
}


/**
 * Sets the acceleration value for the Y-axis servo.
 *
 * @param acc The acceleration value to set.
 * @return True if the acceleration value was set successfully, false otherwise.
 */
bool AdapterCBRove::setServoAccY(u8 acc)
{
     if (!checkInitialized()) return false;
     mSetAccs[1] = acc;
     return true;
}



/**
 * @brief Sets the servo mode of the AdapterCBRove.
 *
 * This function is used to set the servo mode of the AdapterCBRove. The servo mode determines
 * whether the AdapterCBRove is in servo mode or not.
 *
 * @param mode The servo mode to set. True for servo mode, false for non-servo mode.
 * @return True if the servo mode was successfully set, false otherwise.
 */
bool AdapterCBRove::setServoMode(bool mode)
{
     if (!checkInitialized()) return false;
     st.WheelMode(SERVO_X, mode);
     st.WheelMode(SERVO_Y, mode);
     mServoModes[0] = mode;
     mServoModes[1] = mode;
     return true;
}


/**
 * @brief Gets the servo mode.
 * 
 * This function returns the current servo mode.
 * 
 * @return true if the servo mode is enabled, false otherwise.
 */
bool AdapterCBRove::getServoMode()
{
     if (!checkInitialized()) return false;
     return mServoModes[0] && mServoModes[1];
}


/**
 * Retrieves the position of the servo in the X-axis.
 *
 * @return The position of the servo in the X-axis.
 */
int AdapterCBRove::getServoPositionX()
{
     if (!checkInitialized()) return 0;
     return mServoPositions[0];
}

/**
 * Retrieves the position of the servo in the Y-axis.
 *
 * @return The position of the servo in the Y-axis.
 */
int AdapterCBRove::getServoPositionY()
{
     if (!checkInitialized()) return 0;
     return mServoPositions[1];
}


/**
 * Retrieves the speed of the X-axis servo.
 *
 * @return The speed of the X-axis servo.
 */
int AdapterCBRove::getServoSpeedX()
{
     if (!checkInitialized()) return 0;
     return mSetSpeeds[0];
}


/**
 * Retrieves the servo speed in the Y-axis.
 *
 * @return The servo speed in the Y-axis.
 */
int AdapterCBRove::getServoSpeedY()
{
     if (!checkInitialized()) return 0;
     return mSetSpeeds[1];
}


/**
 * Retrieves the acceleration value of the servo in the X-axis.
 *
 * @return The acceleration value of the servo in the X-axis.
 */
u8 AdapterCBRove::getServoAccX()
{
    if (!checkInitialized()) return 0;
    return mSetAccs[0];
}


/**
 * Retrieves the acceleration value of the servo in the Y-axis.
 *
 * @return The acceleration value of the servo in the Y-axis.
 */
u8 AdapterCBRove::getServoAccY()
{
    if (!checkInitialized()) return 0;
    return mSetAccs[1];
}


/**
 * Sets the state of the front LED.
 *
 * @param state The desired state of the front LED. True for ON, false for OFF.
 * @return True if the state was successfully set, false otherwise.
 */
bool AdapterCBRove::setLEDFront(bool state)
{
     if (!checkInitialized()) return false;
     gpioC.writePin(PIN_LED_AVANT, state);
     return true;
}


/**
 * Sets the state of the LED backlight.
 *
 * @param state The desired state of the LED backlight.
 * @return True if the LED backlight state was successfully set, false otherwise.
 */
bool AdapterCBRove::setLEDBack(bool state)
{
     if (!checkInitialized()) return false;
     gpioB.writePin(PIN_LED_ARRIERE, state);
     return true;
}


/**
 * Sets the state of the LED strobe.
 *
 * This function is used to control the LED strobe on the ControleBoard.
 * The state parameter determines whether the LED strobe should be turned on or off.
 *
 * @param state The desired state of the LED strobe. Set to true to turn it on, false to turn it off.
 * @return true if the LED strobe state was successfully set, false otherwise.
 */
bool AdapterCBRove::setLEDStrobe(bool state)
{
     if (!checkInitialized()) return false;
     gpioA.writePin(PIN_STROBE, state);
     return true;
}


/**
 * Sets the state of the winch.
 *
 * @param state The state to set for the winch.
 * @return True if the winch state was successfully set, false otherwise.
 */
bool AdapterCBRove::setWinchState(uint8_t state)
{
     if (!checkInitialized()) return false;
     mWinchState = state;
     switch (state)
     {
     case 1:
          motorWhitch.winchMotorFreeWheel();
          break;
     case 2:
          motorWhitch.winchMotorBreak();
          break;
     case 3:
          motorWhitch.winchMotorReverse();
          break;
     case 4:
          motorWhitch.winchMotorForward();
          break;
     default:
          break;
     }
     return true;
}


/**
 * Retrieves the state of the winch.
 *
 * @return The state of the winch as a uint8_t value.
 */
uint8_t AdapterCBRove::getWinchState()
{
     if (!checkInitialized()) return 0;
     return mWinchState;
}



/**
 * @brief Sets the lock state of Winch1.
 *
 * This function is used to set the lock state of Winch1. The lock state determines whether Winch1 is locked or unlocked.
 *
 * @param state The lock state to set. `true` for locked, `false` for unlocked.
 * @return `true` if the lock state was successfully set, `false` otherwise.
 */
bool AdapterCBRove::setLockWinch1(bool state)
{
     if (!checkInitialized()) return false;
     motorWhitch.setWinchLock1(state);
     return true;
}


/**
 * @brief Sets the lock state of Winch2.
 *
 * This function sets the lock state of Winch2 to the specified state.
 *
 * @param state The desired lock state of Winch2.
 * @return True if the lock state was successfully set, false otherwise.
 */
bool AdapterCBRove::setLockWinch2(bool state)
{
     if (!checkInitialized()) return false;
     motorWhitch.setWinchLock2(state);
     return true;
}


/**
 * @brief Gets the lock status of Winch 1.
 * 
 * This function returns the lock status of Winch 1. It indicates whether the winch is locked or not.
 * 
 * @return true if Winch 1 is locked, false otherwise.
 */
bool AdapterCBRove::getLockWinch1()
{
     if (!checkInitialized()) return false;
     return motorWhitch.getWinchLock1();
}


/**
 * @brief Gets the lock status of Winch2.
 * 
 * This function returns the lock status of Winch2. It indicates whether Winch2 is locked or not.
 * 
 * @return true if Winch2 is locked, false otherwise.
 */
bool AdapterCBRove::getLockWinch2()
{
     if (!checkInitialized()) return false;
     return motorWhitch.getWinchLock2();
}


/**
 * Retrieves the status of the front LED.
 *
 * @return true if the front LED is turned on, false otherwise.
 */
bool AdapterCBRove::getLEDFront()
{
     if (!checkInitialized()) return false;
     switch (gpioC.readPin(PIN_LED_AVANT))
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
 * Retrieves the status of the LED backlight.
 *
 * @return true if the LED backlight is on, false otherwise.
 */
bool AdapterCBRove::getLEDBack()
{
     if (!checkInitialized()) return false;
     switch (gpioB.readPin(PIN_LED_ARRIERE))
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
 * Retrieves the current state of the LED strobe.
 *
 * @return true if the LED strobe is enabled, false otherwise.
 */
bool AdapterCBRove::getLEDStrobe()
{
     if (!checkInitialized()) return false;
     switch (gpioA.readPin(PIN_STROBE))
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
 * @brief Sets the state of GPIO1.
 *
 * This function sets the state of GPIO1 to the specified value.
 *
 * @param state The state to set GPIO1 to.
 * @return True if the state was set successfully, false otherwise.
 */
bool AdapterCBRove::setGPIO1(bool state)
{
     if (!checkInitialized()) return false;
     gpioC.writePin(PIN_GPIO_1, state);
     return true;
}


/**
 * Sets the state of GPIO2.
 *
 * This function sets the state of GPIO2 to the specified value.
 *
 * @param state The state to set GPIO2 to.
 * @return True if the state was successfully set, false otherwise.
 */
bool AdapterCBRove::setGPIO2(bool state)
{
     if (!checkInitialized()) return false;
     gpioC.writePin(PIN_GPIO_2, state);
     return true;
}


/**
 * @brief Sets the state of GPIO3.
 *
 * This function sets the state of GPIO3 to the specified value.
 *
 * @param state The desired state of GPIO3. True for high (on) state, false for low (off) state.
 * @return True if the state of GPIO3 was successfully set, false otherwise.
 */
bool AdapterCBRove::setGPIO3(bool state)
{
     if (!checkInitialized()) return false;
     gpioC.writePin(PIN_GPIO_3, state);
     return true;
}

/**
 * @brief Gets the state of GPIO1.
 * 
 * This function checks if the AdapterCBRove is initialized and reads the state of GPIO1.
 * If GPIO1 is HIGH, it returns true. If GPIO1 is LOW or an invalid state, it returns false.
 * 
 * @return The state of GPIO1. True if HIGH, false if LOW or invalid state.
 */
bool AdapterCBRove::getGPIO1()
{
     if (!checkInitialized()) return false;
     switch (gpioC.readPin(PIN_GPIO_1))
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
 * @brief Gets the value of GPIO2.
 *
 * This function retrieves the current value of GPIO2.
 *
 * @return The value of GPIO2.
 */
bool AdapterCBRove::getGPIO2()
{
     if (!checkInitialized()) return false;
     switch (gpioC.readPin(PIN_GPIO_2))
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
 * @brief Gets the value of GPIO3.
 *
 * This function retrieves the current value of GPIO3.
 *
 * @return The value of GPIO3.
 */
bool AdapterCBRove::getGPIO3()
{
     if (!checkInitialized()) return false;
     switch (gpioC.readPin(PIN_GPIO_3))
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
 * @brief Sets the control mode for the AdapterCBRove.
 *
 * This function sets the control mode of the AdapterCBRove to the specified mode.
 *
 * @param mode The control mode to set.
 * @return True if the control mode was set successfully, false otherwise.
 */
bool AdapterCBRove::setControlMode(uint32_t mode)
{
    if (!checkInitialized()) return false;
    mControlMode = mode;
    switch (mControlMode)
    {
     case 0:
          setServoMode(0);
          break;
     case 1:
          setServoMode(1);
          break;
     
     default:
          break;
    }
    return true;
}

/**
 * Retrieves the control mode of the AdapterCBRove.
 *
 * @return The control mode as a uint32_t value.
 */
uint32_t AdapterCBRove::getControlMode()
{
    if (!checkInitialized()) return 0;
    return mControlMode;
}

/**
 * @brief Returns a reference to the Servo object for controlling the X-axis servo.
 *
 * This function returns a reference to the Servo object that is responsible for controlling the X-axis servo.
 * The Servo object allows you to set the position and other parameters of the servo.
 *
 * @return A reference to the Servo object for the X-axis servo.
 */
Servo &AdapterCBRove::getServoX()
{
    return mServoX;
}


/**
 * @brief Returns a reference to the Servo object for controlling the Y-axis servo motor.
 *
 * This function returns a reference to the Servo object that is responsible for controlling
 * the Y-axis servo motor in the ControleBoard Rove adapter. The Servo object can be used to
 * set the position of the servo motor using the `write()` function, or to read the current
 * position of the servo motor using the `read()` function.
 *
 * @return A reference to the Servo object for controlling the Y-axis servo motor.
 */
Servo &AdapterCBRove::getServoY()
{
    return mServoY;
}


/**
 * @brief Checks if the AdapterCBRove object has been initialized.
 * 
 * This function checks whether the AdapterCBRove object has been properly initialized
 * before performing any operations on it. It returns a boolean value indicating whether
 * the object is initialized or not.
 * 
 * @return true if the AdapterCBRove object is initialized, false otherwise.
 */
bool AdapterCBRove::checkInitialized()
{
     if (mInitialized) 
          return true;
     else
          return false;
}

