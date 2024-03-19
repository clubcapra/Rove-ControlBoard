#ifndef COMSERVO_HPP
#define COMSERVO_HPP


#include "singleton.hpp"
#include "stm32f4xx_hal.h"

#define SERVO_BAUD_1M 0
#define SERVO_BAUD_0_5M 1
#define SERVO_BAUD_250K 2
#define SERVO_BAUD_128K 3
#define SERVO_BAUD_115200 4
#define SERVO_BAUD_76800 5
#define SERVO_BAUD_57600 6
#define SERVO_BAUD_38400 7
#define SERVO_BAUD_19200 8
#define SERVO_BAUD_14400 9
#define SERVO_BAUD_9600 10
#define SERVO_BAUD_4800 11



class ComServo : public TSingleton<ComServo>
{
    friend class TSingleton<ComServo>;
    

    private:

        ComServo(void);
        UART_HandleTypeDef *uart;

   public:

    void readWriteServo(uint8_t *tx, uint16_t tx_size,uint8_t *rx, uint16_t rx_size);
    

};

#endif // TPARTAGE_HPP