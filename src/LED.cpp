/**
 * @file LED.cpp
 * @author Philippe Desbiens
 * @version 1
 * @date 2022-05-19
 * @brief Controle d<un led sur les gpio
 */

#include "../include/LED.h"

LED::LED(volatile uint32_t* gpioBase, uint32_t pinNum) : GPIO(gpioBase)
{
	_pinNum=pinNum;
	setPinMode(pinNum, OUTPUT);
	setPinOutputType(pinNum, PUSH_PULL);
	setPinSpeed(pinNum, MEDIUM_SPEED);
	setPinPad(pinNum,PULL_UP);
	writePin(pinNum, 0);

}

LED& LED::ledOn()
{
	writePin(_pinNum, 1);
	return *this;
}

LED& LED::ledOff()
{
	writePin(_pinNum, 0);
	return *this;
}

LED& LED::ledToggle()
{
	PinState valueTmp= readPin(_pinNum);
	bool tmp;
	switch(valueTmp)
	{
	case HIGH:
		tmp=0;
		break;
	case LOW:
		tmp=1;
		break;
	case ERR:
		tmp=0;
		break;
	default:
		tmp=0;
		break;
	}
	writePin(_pinNum,tmp);
	return *this;
}

LED::~LED() {
	// TODO Auto-generated destructor stub
}


