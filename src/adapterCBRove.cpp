


#include "../include/adapterCBRove.h"








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

     setMinMaxServoX(4095, -4095);

     //@todo: set min max servo Y
     setMinMaxServoY(4095, -4095);


     mInitialized = true;

}


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

void AdapterCBRove::setMinMaxServoX(s16 max, s16 min)
{
     mMaxPosX[0] = max;
     mMaxPosX[1] = min;
}

void AdapterCBRove::setMinMaxServoY(s16 max, s16 min)
{
     mMaxPosY[0] = max;
     mMaxPosY[1] = min;
}

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

bool AdapterCBRove::setServoPositionZero()
{
     return setServoPosition(0,0);
}

bool AdapterCBRove::setServoSpeed(int speedX, int speedY)
{
     if (!checkInitialized()) return false;
     mSetSpeeds[0] = speedX;
     mSetSpeeds[1] = speedY;
     return true;
}

bool AdapterCBRove::setServoAccX(u8 acc)
{
     if (!checkInitialized()) return false;
     mSetAccs[0] = acc;
     return true;
}

bool AdapterCBRove::setServoAccY(u8 acc)
{
     if (!checkInitialized()) return false;
     mSetAccs[1] = acc;
     return true;
}

bool AdapterCBRove::setServoMode(bool modeX,bool modeY)
{
     if (!checkInitialized()) return false;
     st.WheelMode(SERVO_X, modeX);
     st.WheelMode(SERVO_Y, modeY);
     mServoModes[0] = modeX;
     mServoModes[1] = modeY;
     return true;
}

bool AdapterCBRove::getServoMode()
{
     if (!checkInitialized()) return false;
     return mServoModes[0] && mServoModes[1];
}


int AdapterCBRove::getServoPositionX()
{
     if (!checkInitialized()) return 0;
     return mServoPositions[0];
}

int AdapterCBRove::getServoPositionY()
{
     if (!checkInitialized()) return 0;
     return mServoPositions[1];
}

int AdapterCBRove::getServoSpeedX()
{
     if (!checkInitialized()) return 0;
     return mSetSpeeds[0];
}

int AdapterCBRove::getServoSpeedY()
{
     if (!checkInitialized()) return 0;
     return mSetSpeeds[1];
}

u8 AdapterCBRove::getServoAccX()
{
    if (!checkInitialized()) return 0;
    return mSetAccs[0];
}
u8 AdapterCBRove::getServoAccY()
{
    if (!checkInitialized()) return 0;
    return mSetAccs[1];
}


bool AdapterCBRove::setLEDFront(bool state)
{
     if (!checkInitialized()) return false;
     gpioC.writePin(PIN_LED_AVANT, state);
     return true;
}
bool AdapterCBRove::setLEDBack(bool state)
{
     if (!checkInitialized()) return false;
     gpioB.writePin(PIN_LED_ARRIERE, state);
     return true;
}
bool AdapterCBRove::setLEDStrobe(bool state)
{
     if (!checkInitialized()) return false;
     gpioA.writePin(PIN_STROBE, state);
     return true;
}

bool AdapterCBRove::setLockWinch(bool state)
{
     if (!checkInitialized()) return false;
     gpioA.writePin(PIN_wINCH_lOCK, state);
     return true;
}

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

bool AdapterCBRove::getLockWinch()
{
     if (!checkInitialized()) return false;
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



bool AdapterCBRove::setGPIO1(bool state)
{
     if (!checkInitialized()) return false;
     gpioC.writePin(PIN_GPIO_1, state);
     return true;
}
bool AdapterCBRove::setGPIO2(bool state)
{
     if (!checkInitialized()) return false;
     gpioC.writePin(PIN_GPIO_2, state);
     return true;
}
bool AdapterCBRove::setGPIO3(bool state)
{
     if (!checkInitialized()) return false;
     gpioC.writePin(PIN_GPIO_3, state);
     return true;
}
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

bool AdapterCBRove::setControlMode(uint32_t mode)
{
    if (!checkInitialized()) return false;
    mControlMode = mode;
    switch (mControlMode)
    {
     case 0:
          setServoMode(0,0);
          break;
     case 1:
          setServoMode(1,1);
          break;
     
     default:
          break;
    }
    return true;
}

uint32_t AdapterCBRove::getControlMode()
{
    if (!checkInitialized()) return 0;
    return mControlMode;
}
Servo &AdapterCBRove::getServoX()
{
    return mServoX;
}

Servo &AdapterCBRove::getServoY()
{
    return mServoY;
}

bool AdapterCBRove::checkInitialized()
{
     if (mInitialized) 
          return true;
     else
          return false;
}