


#include "../include/adapterCBRove.h"

//@TODO: Test mode servo wheel and if we can see the position
//@TODO: Test PID controller for the servo
//@TODO: Recast and assing the speed to the servo after PID
//@TODO: Set speed to the servo after test of mode wheel
//@TODO: ADD mode selection if needed for servo between wheel and position mode


AdapterCBRove::AdapterCBRove()
{
     gpioC=GPIO((uint32_t *)(0x40020800UL));
     gpioB=GPIO((uint32_t *)(0x40020400UL));
     gpioA=GPIO((uint32_t *)(0x40020000UL));
     
}

AdapterCBRove::~AdapterCBRove()
{
    
}

void AdapterCBRove::init()
{
     gpioB.setPinMode(PIN_LED_ARRIERE,OUTPUT).setPinOutputType(PIN_LED_ARRIERE, PUSH_PULL).setPinSpeed(PIN_LED_ARRIERE, MEDIUM_SPEED).setPinPad(PIN_LED_ARRIERE,PULL_UP);
	gpioC.setPinMode(PIN_LED_AVANT,OUTPUT).setPinOutputType(PIN_LED_AVANT, PUSH_PULL).setPinSpeed(PIN_LED_AVANT, MEDIUM_SPEED).setPinPad(PIN_LED_AVANT,PULL_UP);
	gpioA.setPinMode(PIN_STROBE,OUTPUT).setPinOutputType(PIN_STROBE, PUSH_PULL).setPinSpeed(PIN_STROBE, MEDIUM_SPEED).setPinPad(PIN_STROBE,PULL_UP);
	gpioA.setPinMode(PIN_wINCH_lOCK,OUTPUT).setPinOutputType(PIN_wINCH_lOCK, PUSH_PULL).setPinSpeed(PIN_wINCH_lOCK, MEDIUM_SPEED).setPinPad(PIN_wINCH_lOCK,PULL_UP);
	gpioC.setPinMode(PIN_GPIO_1,OUTPUT).setPinOutputType(PIN_GPIO_1, PUSH_PULL).setPinSpeed(PIN_GPIO_1, MEDIUM_SPEED).setPinPad(PIN_GPIO_1,PULL_UP);
	gpioC.setPinMode(PIN_GPIO_2,OUTPUT).setPinOutputType(PIN_GPIO_2, PUSH_PULL).setPinSpeed(PIN_GPIO_2, MEDIUM_SPEED).setPinPad(PIN_GPIO_2,PULL_UP);
	gpioC.setPinMode(PIN_GPIO_3,OUTPUT).setPinOutputType(PIN_GPIO_3, PUSH_PULL).setPinSpeed(PIN_GPIO_3, MEDIUM_SPEED).setPinPad(PIN_GPIO_3,PULL_UP);
	



     gpioB.writePin(PIN_LED_ARRIERE, 0);
     gpioC.writePin(PIN_LED_AVANT, 0);
     gpioA.writePin(PIN_STROBE, 0);
     gpioA.writePin(PIN_wINCH_lOCK, 0);

     gpioC.writePin(PIN_GPIO_1, 0);
     gpioC.writePin(PIN_GPIO_2, 0);
     gpioC.writePin(PIN_GPIO_3, 0);

     //@TODO: Initialize the servo and test PID and if we can see the position in wheel mode
     //@TODO: Initialize the PID controller for the servo
     //@TODO: Set MAX and MIN position for the servo

}


void AdapterCBRove::task()
{
     servoPositionX = servo.ReadPos(SERVO_X);
     //servoSpeedX=
     pidX.calculate(servoPositionX, 0.1);
     //@TODO: Set speed to the servo after test of mode wheel
     servoPositionY = servo.ReadPos(SERVO_Y);
     //servoSpeedY=
     pidX.calculate(servoPositionX, 0.1);
     //@TODO: Set speed to the servo after test of mode wheel

}

void AdapterCBRove::setServoPosition(int positionX, int positionY)
{
     pidX.setGoalPosition(positionX);
     //pidX.reset();
     pidY.setGoalPosition(positionY);
     //pidY.reset();
}

void AdapterCBRove::setServoSpeed(int speedX, int speedY)
{
     //@TODO: ASK if we need to set mode from the jetson
     /*
     servo.WheelMode(SERVO_X, true);
     servo.WheelMode(SERVO_Y, true);
     servo.WriteSpe(SERVO_X, speedX);
     servo.WriteSpe(SERVO_Y, speedY);
     */
}

void AdapterCBRove::setServoPositionZero()
{
     pidX.setGoalPosition(0);
     //pidX.reset();
     pidY.setGoalPosition(0);
     //pidY.reset();
}

int AdapterCBRove::getServoPositionX()
{
     return servoPositionX;
}

int AdapterCBRove::getServoPositionY()
{
     return servoPositionY;
}

int AdapterCBRove::getServoSpeedX()
{
     return servoSpeedX;
}

int AdapterCBRove::getServoSpeedY()
{
     return servoSpeedY;
}


void AdapterCBRove::setLEDFront(bool state)
{
     gpioC.writePin(PIN_LED_AVANT, state);
}
void AdapterCBRove::setLEDBack(bool state)
{
     gpioB.writePin(PIN_LED_ARRIERE, state);
}
void AdapterCBRove::setLEDStrobe(bool state)
{
     gpioA.writePin(PIN_STROBE, state);
}

void AdapterCBRove::setLockWinch(bool state)
{
     gpioA.writePin(PIN_wINCH_lOCK, state);
}

bool AdapterCBRove::getLEDFront()
{
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
bool AdapterCBRove::getLEDBack()
{
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
bool AdapterCBRove::getLEDStrobe()
{
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

bool AdapterCBRove::getLockWinch()
{
    switch (gpioA.readPin(PIN_wINCH_lOCK))
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



void AdapterCBRove::setGPIO1(bool state)
{
     gpioC.writePin(PIN_GPIO_1, state);
}
void AdapterCBRove::setGPIO2(bool state)
{
     gpioC.writePin(PIN_GPIO_2, state);
}
void AdapterCBRove::setGPIO3(bool state)
{
     gpioC.writePin(PIN_GPIO_3, state);
}
bool AdapterCBRove::getGPIO1()
{
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
bool AdapterCBRove::getGPIO2()
{
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
bool AdapterCBRove::getGPIO3()
{
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