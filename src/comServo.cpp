

#include "../lib/comServo.hpp"


ComServo::ComServo()
{
    //uart = &huart2;
}

void ComServo::readWriteServo(uint8_t *tx, uint16_t tx_size,uint8_t *rx, uint16_t rx_size)
{   
    HAL_UARTEx_ReceiveToIdle_DMA(uart, rx, 16);   
    HAL_UART_Transmit_DMA(uart, tx, tx_size);
    
}