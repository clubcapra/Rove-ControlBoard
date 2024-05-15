


/**
 * @file adapterCBRove.cpp
 * @brief Implementation file for the AdapterCBRoveClass class.
 *
 * This file contains the implementation of the AdapterCBRoveClass class, which is responsible for
 * controlling the communication between the ControleBoard and the adapter.
 */

#include "api.h"
#include "adapterCBRove.h"
#include "ARGB.h"


/**
 * @brief Constructs a new instance of the AdapterCBRoveClass class.
 *
 * This constructor initializes a new instance of the AdapterCBRoveClass class.
 * It is responsible for setting up the necessary resources and preparing
 * the adapter for communication with the ControleBoard.
 */
AdapterCBRoveClass::AdapterCBRoveClass()
{
     gpioC=GPIO((uint32_t *)(0x40020800UL));
     gpioB=GPIO((uint32_t *)(0x40020400UL));
     gpioA=GPIO((uint32_t *)(0x40020000UL));
     motorWhitch=WinchMotor(gpioA);
}

AdapterCBRoveClass& AdapterCBRoveClass::operator=(const AdapterCBRoveClass& other)
{
     if (this == &other) return *this;
     gpioA = other.gpioA;
     gpioB = other.gpioB;
     gpioC = other.gpioC;
     motorWhitch = other.motorWhitch;

     return *this;
}

/**
 * @brief Destructor for the AdapterCBRoveClass class.
 * 
 * This destructor is responsible for cleaning up any resources
 * allocated by the AdapterCBRoveClass class.
 */
AdapterCBRoveClass::~AdapterCBRoveClass()
{
    
}


/**
 * @brief Initializes the AdapterCBRoveClass object.
 * 
 * @param huartServo 
 * @throws ServoNACKException If the servos are unable to ack
 */
void AdapterCBRoveClass::init(UART_HandleTypeDef *huartServo)
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

     st.pSerial=huartServo;
     // setMinMaxServoX((4095*2), (-4095*2));

     //@todo: set min max servo Y
     // setMinMaxServoY(4095, -4095);
     mInitialized = true;
     setStatusCode(StatusCode::STInitialized);
}



/**
 * @brief Executes the updateServo for the AdapterCBRoveClass class.
 * This function is responsible for performing the necessary operations for the AdapterCBRoveClass class.
 * It should be called periodically to ensure the proper functioning of the class.
 * 
 * @throws AdapterNotInitializedException If adapter is not initialized
 * @throws ServoNACKException If the servo(s) are unable to ack
 */
void AdapterCBRoveClass::updateServo()
{
     if (!checkInitialized()) return;

     setPositions[0] = mSetPositions[0];
     setPositions[1] = mSetPositions[1];
     setSpeeds[0] = mSetSpeeds[0];
     setSpeeds[1] = mSetSpeeds[1];
     setAccs[0] = mSetAccs[0];
     setAccs[1] = mSetAccs[1];

     switch (mControlMode)
     {
     case SCMPosition:
          st.SyncWritePosEx(mIDs,2,setPositions,(u16*)setSpeeds,setAccs);
          if (st.getErr())
          {
               setErrorCode(ErrorCode::ERServoNACK);
               throw ServoNACKException(true, true);
          }
          else
          {
               unsetErrorCode(ErrorCode::ERServoNACK);
          }
     //st.WritePosEx(mIDs[0], setPositions[0], setSpeeds[0], setAccs[0]);
     /*
          if(mServoPositions[0]<=mSetPositions[0]+5 && mServoPositions[0]>=mSetPositions[0]-5
          && mServoPositions[1]<=mSetPositions[1]+5 && mServoPositions[1]>=mSetPositions[1]-5);
          else
               st.WritePosEx(mIDs[0], setPositions[0], setSpeeds[0], setAccs[0]);
     */          
          break;
     case SCMSpeed:

          if(mServoPositions[0] >= mMaxPosX[0] || mServoPositions[0] <= mMaxPosX[1])
          {
               setSpeeds[0] = 0;
               setAccs[0] = 0;
          }

          if (!st.WriteSpe(mIDs[0], setSpeeds[0], setAccs[0]))
               setErrorCode(ErrorCode::ERServoXNACK);
          else
               unsetErrorCode(ErrorCode::ERServoXNACK);

          if(mServoPositions[1] >= mMaxPosY[0] || mServoPositions[1] <= mMaxPosY[1])
          {
               setSpeeds[1] = 0;
               setAccs[1] = 0;
          }

          if (!st.WriteSpe(mIDs[1], setSpeeds[1], setAccs[1]))
               setErrorCode(ErrorCode::ERServoYNACK);
          else
               unsetErrorCode(ErrorCode::ERServoYNACK);
          
          if (getErrorCode(ErrorCode::ERServoXNACK) || getErrorCode(ErrorCode::ERServoYNACK))
          {
               throw ServoNACKException(getErrorCode(ErrorCode::ERServoXNACK), getErrorCode(ErrorCode::ERServoYNACK));
          }
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
void AdapterCBRoveClass::setMinMaxServoX(s16 max, s16 min)
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
void AdapterCBRoveClass::setMinMaxServoY(s16 max, s16 min)
{
     mMaxPosY[0] = max;
     mMaxPosY[1] = min;
}


/**
 * Sets the position of the servo motor.
 *
 * This function sets the position of the servo motor connected to the AdapterCBRoveClass object.
 * The position is specified by the `positionX` and `positionY` parameters.
 *
 * @param positionX The desired position of the servo motor along the X-axis.
 * @param positionY The desired position of the servo motor along the Y-axis.
 * @return True if the servo position was set successfully, false otherwise.
 * @throws AdapterNotInitializedException If not initialized
 */
bool AdapterCBRoveClass::setServoPosition(s16 positionX, s16 positionY)
{
     if (!checkInitialized()) return false;
     if(positionX >= mMaxPosX[0] )
          mSetPositions[0]=mMaxPosX[0];
     else if(positionX <= mMaxPosX[1])
          mSetPositions[0]= mMaxPosX[1];
     else
          mSetPositions[0] = positionX;
     
     if(positionY >=  mMaxPosY[0] )     
          mSetPositions[1]=mMaxPosY[0];
     else if(positionY < mMaxPosY[1])
          mSetPositions[1]=mMaxPosY[1];
     else
          mSetPositions[1] = positionY;


     updateServo();
     return true;
}


/**
 * Sets the servo position to zero.
 *
 * This function sets the position of the servo to zero, which is the neutral position.
 * It can be used to reset the servo to its initial position.
 *
 * @return true if the servo position was successfully set to zero, false otherwise.
 * @throws AdapterNotInitializedException If not initialized
 */
bool AdapterCBRoveClass::setServoPositionZero()
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
 * @throws AdapterNotInitializedException If not initialized
 */
bool AdapterCBRoveClass::setServoSpeed(s16 speedX, s16 speedY)
{
     if (!checkInitialized()) return false;
     mSetSpeeds[0] = speedX;
     mSetSpeeds[1] = speedY;

     updateServo();
     return true;
}


/**
 * Sets the acceleration value for the X-axis servo.
 *
 * @param acc The acceleration value to set.
 * @return True if the acceleration value was set successfully, false otherwise.
 * @throws AdapterNotInitializedException If not initialized
 */
bool AdapterCBRoveClass::setServoAccX(u8 acc)
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
 * @throws AdapterNotInitializedException If not initialized
 */
bool AdapterCBRoveClass::setServoAccY(u8 acc)
{
     if (!checkInitialized()) return false;
     mSetAccs[1] = acc;
     return true;
}



/**
 * @brief Sets the servo mode of the AdapterCBRoveClass.
 *
 * This function is used to set the servo mode of the AdapterCBRoveClass. The servo mode determines
 * whether the AdapterCBRoveClass is in servo mode or not.
 *
 * @param mode The servo mode to set. True for servo mode, false for non-servo mode.
 * @return True if the servo mode was successfully set, false otherwise.
 * @throws AdapterNotInitializedException If not initialized
 * @throws ServoNACKException If servo(s) are unable to ack
 */
bool AdapterCBRoveClass::setServoMode(bool mode)
{
     if (!checkInitialized()) return false;
     int xAck = st.WheelMode(SERVO_X, mode);
     int yAck = st.WheelMode(SERVO_Y, mode);

     if (!xAck) setErrorCode(ErrorCode::ERServoXNACK);
     if (!yAck) setErrorCode(ErrorCode::ERServoYNACK);
     if (!xAck || !yAck) throw ServoNACKException(!xAck, !yAck);
     
     
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
 * @throws AdapterNotInitializedException If not initialized
 */
bool AdapterCBRoveClass::getServoMode()
{
     if (!checkInitialized()) return false;
     return mServoModes[0] && mServoModes[1];
}


/**
 * Retrieves the position of the servo in the X-axis.
 *
 * @return The position of the servo in the X-axis.
 * @throws AdapterNotInitializedException If not initialized
 */
s16 AdapterCBRoveClass::getServoPositionX()
{
     if (!checkInitialized()) return 0;
     mServoPositions[(SERVO_X -1)] = st.ReadPos(mIDs[(SERVO_X -1)]);
     return mServoPositions[(SERVO_X -1)];
}

/**
 * Retrieves the position of the servo in the Y-axis.
 *
 * @return The position of the servo in the Y-axis.
 * @throws AdapterNotInitializedException If not initialized
 */
s16 AdapterCBRoveClass::getServoPositionY()
{
     if (!checkInitialized()) return 0;
     mServoPositions[(SERVO_Y -1)] = st.ReadPos(mIDs[(SERVO_Y -1)]);
     return mServoPositions[(SERVO_Y -1)];
}


/**
 * Retrieves the speed of the X-axis servo.
 *
 * @return The speed of the X-axis servo.
 * @throws AdapterNotInitializedException If not initialized
 */
s16 AdapterCBRoveClass::getServoSpeedX()
{
     if (!checkInitialized()) return 0;
     return mSetSpeeds[0];
}


/**
 * Retrieves the servo speed in the Y-axis.
 *
 * @return The servo speed in the Y-axis.
 * @throws AdapterNotInitializedException If not initialized
 */
s16 AdapterCBRoveClass::getServoSpeedY()
{
     if (!checkInitialized()) return 0;
     return mSetSpeeds[1];
}


/**
 * Retrieves the acceleration value of the servo in the X-axis.
 *
 * @return The acceleration value of the servo in the X-axis.
 * @throws AdapterNotInitializedException If not initialized
 */
u8 AdapterCBRoveClass::getServoAccX()
{
    if (!checkInitialized()) return 0;
    return mSetAccs[0];
}


/**
 * Retrieves the acceleration value of the servo in the Y-axis.
 *
 * @return The acceleration value of the servo in the Y-axis.
 * @throws AdapterNotInitializedException If not initialized
 */
u8 AdapterCBRoveClass::getServoAccY()
{
    if (!checkInitialized()) return 0;
    return mSetAccs[1];
}


/**
 * Sets the state of the front LED.
 *
 * @param state The desired state of the front LED. True for ON, false for OFF.
 * @return True if the state was successfully set, false otherwise.
 * @throws AdapterNotInitializedException If not initialized
 */
bool AdapterCBRoveClass::setLEDFront(bool state)
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
 * @throws AdapterNotInitializedException If not initialized
 */
bool AdapterCBRoveClass::setLEDBack(bool state)
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
 * @throws AdapterNotInitializedException If not initialized
 */
bool AdapterCBRoveClass::setLEDStrobe(bool state)
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
 * @throws AdapterNotInitializedException If not initialized
 */
bool AdapterCBRoveClass::setWinchState(WinchMode state)
{
     if (!checkInitialized()) return false;
     mWinchState = state;
     switch (state)
     {
     case WMFreeWheel:
          motorWhitch.winchMotorFreeWheel();
          break;
     case WMBrake:
          motorWhitch.winchMotorBrake();
          break;
     case WMReverse:
          motorWhitch.winchMotorReverse();
          break;
     case WMForward:
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
 * @throws AdapterNotInitializedException If not initialized
 */
WinchMode AdapterCBRoveClass::getWinchState()
{
     if (!checkInitialized()) return WinchMode::WMFreeWheel;
     return mWinchState;
}



/**
 * @brief Sets the lock state of Winch1.
 *
 * This function is used to set the lock state of Winch1. The lock state determines whether Winch1 is locked or unlocked.
 *
 * @param state The lock state to set. `true` for locked, `false` for unlocked.
 * @return `true` if the lock state was successfully set, `false` otherwise.
 * @throws AdapterNotInitializedException If not initialized
 */
bool AdapterCBRoveClass::setLockWinch1(bool state)
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
 * @throws AdapterNotInitializedException If not initialized
 */
bool AdapterCBRoveClass::setLockWinch2(bool state)
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
 * @throws AdapterNotInitializedException If not initialized
 */
bool AdapterCBRoveClass::getLockWinch1()
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
 * @throws AdapterNotInitializedException If not initialized
 */
bool AdapterCBRoveClass::getLockWinch2()
{
     if (!checkInitialized()) return false;
     return motorWhitch.getWinchLock2();
}


/**
 * Retrieves the status of the front LED.
 *
 * @return true if the front LED is turned on, false otherwise.
 * @throws AdapterNotInitializedException If not initialized
 */
bool AdapterCBRoveClass::getLEDFront()
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
 * @throws AdapterNotInitializedException If not initialized
 */
bool AdapterCBRoveClass::getLEDBack()
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
 * @throws AdapterNotInitializedException If not initialized
 */
bool AdapterCBRoveClass::getLEDStrobe()
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
 * @throws AdapterNotInitializedException If not initialized
 */
bool AdapterCBRoveClass::setGPIO1(bool state)
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
 * @throws AdapterNotInitializedException If not initialized
 */
bool AdapterCBRoveClass::setGPIO2(bool state)
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
 * @throws AdapterNotInitializedException If not initialized
 */
bool AdapterCBRoveClass::setGPIO3(bool state)
{
     if (!checkInitialized()) return false;
     gpioC.writePin(PIN_GPIO_3, state);
     return true;
}

/**
 * @brief Gets the state of GPIO1.
 * 
 * This function checks if the AdapterCBRoveClass is initialized and reads the state of GPIO1.
 * If GPIO1 is HIGH, it returns true. If GPIO1 is LOW or an invalid state, it returns false.
 * 
 * @return The state of GPIO1. True if HIGH, false if LOW or invalid state.
 * @throws AdapterNotInitializedException If not initialized
 */
bool AdapterCBRoveClass::getGPIO1()
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
 * @throws AdapterNotInitializedException If not initialized
 */
bool AdapterCBRoveClass::getGPIO2()
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
 * @throws AdapterNotInitializedException If not initialized
 */
bool AdapterCBRoveClass::getGPIO3()
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
 * @brief Sets the control mode for the AdapterCBRoveClass.
 *
 * This function sets the control mode of the AdapterCBRoveClass to the specified mode.
 *
 * @param mode The control mode to set.
 * @return True if the control mode was set successfully, false otherwise.
 * @throws AdapterNotInitializedException If not initialized
 * @throws ServoNACKException If servo(s) are unable to ack
 */
bool AdapterCBRoveClass::setControlMode(ServoControlMode mode)
{
    if (!checkInitialized()) return false;
    mControlMode = mode;
    switch (mControlMode)
    {
     case SCMPosition:
          setServoMode(0);
          break;
     case SCMSpeed:
          setServoMode(1);
          break;
     
     default:
          break;
    }
    return true;
}

/**
 * Retrieves the control mode of the AdapterCBRoveClass.
 *
 * @return The control mode as a uint32_t value.
 * @throws AdapterNotInitializedException If not initialized
 */
ServoControlMode AdapterCBRoveClass::getControlMode()
{
    if (!checkInitialized()) return SCMNone;
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
/*
Servo &AdapterCBRoveClass::getServoX()
{
    return mServoX;
}*/


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
/*
Servo &AdapterCBRoveClass::getServoY()
{
    return mServoY;
}*/


/**
 * @brief Checks if the AdapterCBRoveClass object has been initialized.
 * 
 * This function checks whether the AdapterCBRoveClass object has been properly initialized
 * before performing any operations on it. It returns a boolean value indicating whether
 * the object is initialized or not.
 * 
 * @return true if the AdapterCBRoveClass object is initialized, false otherwise.
 * @throws AdapterNotInitializedException If not initialized
 */
bool AdapterCBRoveClass::checkInitialized()
{
     if (mInitialized) 
     {
          if (getErrorCode(ErrorCode::ERAdapterNotInit)) unsetErrorCode(ErrorCode::ERAdapterNotInit);
          return true;
     }
     else
     {
          setErrorCode(ErrorCode::ERAdapterNotInit);
          throw AdapterNotInitializedException();
     }
}

/**
 * @brief Gets the color of an led
 * 
 * @param led Led index
 * @return RGB The color of the led
 * 
 * @throws AdapterNotInitializedException If not initialized
 * @throws RGBIndexOutOfRangeException If led index is out of range
 */
RGB AdapterCBRoveClass::getRGBLed(int led)
{
     if(!checkInitialized()) return {};
     if (led < 0 || led >= LED_COUNT)
     {
          setErrorCode(ErrorCode::ERRGBInvalidIndex);
          throw RGBIndexOutOfRangeException(led);
     }
     return mLeds[led];
}

/**
 * @brief Gets the color of an led
 * 
 * @param led Led index
 * @param RGB Color to assign to the led
 * @return bool true if successful
 * 
 * @throws AdapterNotInitializedException If not initialized
 * @throws RGBIndexOutOfRangeException If led index is out of range
 * @throws RGBException If failed to send the color
 */
bool AdapterCBRoveClass::setRGBLed(int led, RGB color)
{
     if(!checkInitialized()) return false;
     if (led < 0 || led >= LED_COUNT)
     {
          setErrorCode(ErrorCode::ERRGBInvalidIndex);
          throw RGBIndexOutOfRangeException(led);
     }
     else { unsetErrorCode(ErrorCode::ERRGBInvalidIndex); }
     ARGB_SetRGB(led, color.r, color.g, color.b);
     auto status = ARGB_Show();
     if (status == ARGB_STATE::ARGB_PARAM_ERR) 
     {
          setErrorCode(ErrorCode::ERRGBParam);
          throw RGBException(status);
     }
     else { unsetErrorCode(ErrorCode::ERRGBParam); }
     return true;
}

StatusCode AdapterCBRoveClass::getStatusCode() const
{
    return mStatus;
}

void AdapterCBRoveClass::setStatusCode(StatusCode status)
{
     mStatus = status;
}

bool AdapterCBRoveClass::getErrorCode(ErrorCode error) const
{
     return (mError & error) == error;
}

ErrorCode AdapterCBRoveClass::getErrorCodes() const
{
    return mError;
}

void AdapterCBRoveClass::clearErrorCodes()
{
     mError = ErrorCode::ERNone;
}

void AdapterCBRoveClass::setErrorCode(ErrorCode error)
{
     mError = (ErrorCode)(mError | error);
}

void AdapterCBRoveClass::unsetErrorCode(ErrorCode error)
{
     mError = (ErrorCode)(mError & ~error);
}

AdapterCBRoveClass AdapterCBRove{};