/**
 * @file GPIO.cpp
 * @author Philippe Desbiens
 * @version 1
 * @date 2022-05-19
 * @brief classe des GPIO pour les controlers
 */


#include "GPIO.h"

GPIO::GPIO()
{
	_isInit=false;
}

GPIO::GPIO(volatile uint32_t* gpioBase)
{
	_gpioBase = gpioBase;
	RCC_AHB1ENR |= (1 << (((uint32_t)(gpioBase )- (uint32_t)(0x40020000UL))/(uint32_t)(0x400UL)));

	GPIO_MODER = gpioBase;
	GPIO_OTYPER= gpioBase +1;
	GPIO_OSPEEDR = gpioBase +2;
	GPIO_PUPDR = gpioBase + 3;
	GPIO_IDR = gpioBase +4;
	GPIO_ODR = gpioBase + 5;
	GPIO_BSRR = gpioBase + 6;
	GPIO_LCKR = gpioBase + 7;
	GPIO_AFR[1] = gpioBase + 8;
	GPIO_AFR[2] = gpioBase + 9;
	_isInit=true;
}
GPIO::~GPIO() {}

GPIO::GPIO(const GPIO& gpio)
{
	_gpioBase = gpio._gpioBase;
	RCC_AHB1ENR |= (1 << (((uint32_t)(gpio._gpioBase )- (uint32_t)(0x40020000UL))/(uint32_t)(0x400UL)));

	GPIO_MODER = gpio.GPIO_MODER;
	GPIO_OTYPER = gpio.GPIO_OTYPER;
	GPIO_OSPEEDR = gpio.GPIO_OSPEEDR;
	GPIO_PUPDR = gpio.GPIO_PUPDR;
	GPIO_IDR = gpio.GPIO_IDR;
	GPIO_ODR = gpio.GPIO_ODR;
	GPIO_BSRR = gpio.GPIO_BSRR;
	GPIO_LCKR = gpio.GPIO_LCKR;
	GPIO_AFR[1] = gpio.GPIO_AFR[1];
	GPIO_AFR[2] = gpio.GPIO_AFR[2];
	_isInit=true;
}	

GPIO& GPIO::operator=(const GPIO& gpio)
{
	_gpioBase = gpio._gpioBase;
	RCC_AHB1ENR |= (1 << (((uint32_t)(gpio._gpioBase )- (uint32_t)(0x40020000UL))/(uint32_t)(0x400UL)));

	GPIO_MODER = gpio.GPIO_MODER;
	GPIO_OTYPER = gpio.GPIO_OTYPER;
	GPIO_OSPEEDR = gpio.GPIO_OSPEEDR;
	GPIO_PUPDR = gpio.GPIO_PUPDR;
	GPIO_IDR = gpio.GPIO_IDR;
	GPIO_ODR = gpio.GPIO_ODR;
	GPIO_BSRR = gpio.GPIO_BSRR;
	GPIO_LCKR = gpio.GPIO_LCKR;
	GPIO_AFR[1] = gpio.GPIO_AFR[1];
	GPIO_AFR[2] = gpio.GPIO_AFR[2];
	_isInit=true;
	return *this;
}	

GPIO& GPIO::setPinMode (uint32_t pinNum , Mode mode)
{
	if(_isInit)
	{
		if (pinNum >= MIN_PIN_NUM && pinNum <= MAX_PIN_NUM)
		{
			*GPIO_MODER &= ~(3 << 2*pinNum);
			*GPIO_MODER |= (mode << 2*pinNum);
		}
	}
	return *this;
}
GPIO& GPIO::setPinOutputType (uint32_t pinNum , OutputType type)
{
	if(_isInit)
	{
		if (pinNum >= MIN_PIN_NUM && pinNum <= MAX_PIN_NUM)
		{
			*GPIO_OTYPER &= ~(1 <<pinNum);
			*GPIO_OTYPER |= (type <<pinNum);
		}
	}
	return * this;
}

PinState GPIO::readPin (uint32_t pinNum)
{
	if(_isInit)
	{
		if (pinNum >= MIN_PIN_NUM && pinNum <= MAX_PIN_NUM)
		{
			return ((*GPIO_IDR & (1 <<pinNum )) >> pinNum ) ?
					HIGH : LOW;
		}
	}
	return ERR;
}


GPIO& GPIO::setPinSpeed (uint32_t pinNum , Speed speed)
{
	if (pinNum >= MIN_PIN_NUM && pinNum <= MAX_PIN_NUM)
	{
		*GPIO_OSPEEDR &= ~(3 << 2*pinNum);
		*GPIO_OSPEEDR |= (speed << 2*pinNum);
	}
	return * this;
}

GPIO& GPIO::setPinPad (uint32_t pinNum , Pad pad)
{
	if(_isInit)
	{
		if (pinNum >= MIN_PIN_NUM && pinNum <= MAX_PIN_NUM)
		{
			*GPIO_PUPDR &= ~(3 << 2*pinNum);
			*GPIO_PUPDR |= (pad << 2*pinNum);
		}
	}
	return * this;
}

GPIO& GPIO::writePin (uint32_t pinNum , bool value)
{
	if(_isInit)
	{
		if (pinNum >= MIN_PIN_NUM && pinNum <= MAX_PIN_NUM)
		{
			*GPIO_ODR &= ~(1 <<pinNum);
			*GPIO_ODR |= (value <<pinNum);
		}
	}
	return * this;
}


