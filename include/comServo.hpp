#ifndef COMSERVO_HPP
#define COMSERVO_HPP

#include "singleton.hpp"
#include "stm32f4xx_hal.h"

/**
 * @class ComServo
 * @brief Represents a communication interface for controlling servos.
 * 
 * This class is a singleton and provides methods for initializing the UART communication,
 * as well as reading from and writing to the servo.
 */
class ComServo : public TSingleton<ComServo>
{
    friend class TSingleton<ComServo>;

private:
   
    ComServo(void);

    UART_HandleTypeDef *huart6; /**< UART handle for communication. */

public:
   bool flagRx;
    
    void readWriteServo(uint8_t *tx, uint16_t tx_size, uint8_t *rx, uint16_t rx_size);

    void setHuart(UART_HandleTypeDef *huart);
};

#endif // COMSERVO_HPP
