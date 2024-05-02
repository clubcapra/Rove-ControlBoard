/**
 * @file GPIO.h
 * @author Philippe Desbiens
 * @version 1
 * @date 2022-05-19
 * @brief classe des GPIO pour les controlers
 */


#ifndef GPIO_H_
#define GPIO_H_

#include <stdint.h>

#define MIN_PIN_NUM 0
#define MAX_PIN_NUM 15
#define RCC_AHB1ENR *((volatile char*) (uint32_t*)(0x40023800UL) + 0x30)
enum PinState {HIGH = 1, LOW = 0, ERR = -1};
enum Mode {INPUT = 0b00, OUTPUT = 0b01, ALTERNATE = 0b10, ANALOG = 0b11};
enum OutputType {PUSH_PULL = 0, OPEN_DRAIN = 1};
enum Speed {LOW_SPEED = 0b00, MEDIUM_SPEED = 0b01, FAST_SPEED = 0b10, HIGH_SPEED = 0b11};
enum Pad {NO_PUPD = 0b00, PULL_UP = 0b01, PULL_DOWN = 0b10};

class GPIO
{
public:
	 GPIO();
	/**
	 * @fn  GPIO(volatile uint32_t*) Initialisation de la GPIO
	 * @param gpioBase addresse de début du gpio désirer
	 */
	GPIO(volatile uint32_t* gpioBase);

	GPIO(const GPIO& gpio);

	GPIO &operator=(const GPIO& gpio);

	/**
	 * @fn  ~GPIO() destructeur du gpio
	 */
	virtual ~ GPIO();

	/**
	 * @fn GPIO setPinMode&(uint32_t, Mode) modification registe de mode
	 * @param pinNum numero de la pin
	 * @param mode désirer pour la pin
	 * @return le gpio
	 */
	GPIO& setPinMode (uint32_t pinNum , Mode mode);

	/**
	 * @fn GPIO setPinOutputType&(uint32_t, OutputType) modification registe du output
	 * @param pinNum numero de la pin
	 * @param type désirer pour la pin
	 * @return le gpio
	 */
	GPIO& setPinOutputType (uint32_t pinNum , OutputType type);

	/**
	 * @fn GPIO setPinSpeed&(uint32_t, Speed) modification registe du speed
	 * @param pinNum numero de la pin
	 * @param speed désirer pour la pin
	 * @return le gpio
	 */
	GPIO& setPinSpeed (uint32_t pinNum , Speed speed);

	/**
	 * @fn GPIO setPinPad&(uint32_t, Pad) modification registe du pad
	 * @param pinNum numero de la pin
	 * @param pad désirer pour la pin
	 * @return le gpio
	 */
	GPIO& setPinPad (uint32_t pinNum , Pad pad);

	/**
	 * @fn PinState readPin(uint32_t) lecture du GPIO
	 * @param pinNum numero de la pin
	 * @return l'etat de la pin
	 */
	PinState readPin (uint32_t pinNum);

	/**
	 * @fn GPIO writePin&(uint32_t, bool) écriture sur le GPIO
	 * @param pinNum numero de la pin
	 * @param value a écrire
	 * @return le gpio
	 */
	GPIO& writePin (uint32_t pinNum , bool value);

protected :
	volatile uint32_t* _gpioBase;
	volatile uint32_t* GPIO_MODER;
	volatile uint32_t* GPIO_OTYPER;
	volatile uint32_t* GPIO_OSPEEDR;
	volatile uint32_t* GPIO_PUPDR;
	volatile uint32_t* GPIO_IDR;
	volatile uint32_t* GPIO_ODR;
	volatile uint32_t* GPIO_BSRR;
	volatile uint32_t* GPIO_LCKR;
	volatile uint32_t* GPIO_AFR[2];
	volatile bool _isInit=false;
};


#endif /* GPIO_H_ */
