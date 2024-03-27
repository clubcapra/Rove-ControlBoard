/**
 * @file LED.h
 * @author Philippe Desbiens
 * @version 1
 * @date 2022-05-19
 * @brief Controle d<un led sur les gpio
 */

#include "GPIO.h"
#include <stdint.h>

#ifndef LED_H_
#define LED_H_


class LED: private GPIO
{
private:
	uint32_t _pinNum=0;

public:
	/**
	 * @fn  LED(volatile uint32_t*, uint32_t) intitialisation de la led
	 * @param gpioBase Addresse de d/but du gpio déssirer
	 * @param pinNum le numéro de la pin de la led
	 */
	LED(volatile uint32_t* gpioBase, uint32_t pinNum);

	/**
	 * @fn LED ledOn&() allume la led
	 * @return la led
	 */
	LED& ledOn();

	/**
	 * @fn LED ledOff&() étaint la led
	 * @return la led
	 */
	LED& ledOff();

	/**
	 * @fn LED ledToggle&() toggle de la led
	 * @return la led
	 */
	LED& ledToggle();

	/**
	 * @fn  ~LED() destructeur de la led
	 */
	virtual ~LED();

};

#endif /* LED_H_ */
