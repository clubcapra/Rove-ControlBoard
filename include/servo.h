#ifndef _SERVO_H
#define _SERVO_H

//#include <stdint.h>

/*
#include "cmsis_os2.h"
#include "stm32f4xx_ll_usart.h"
#include "usart.h"
*/

#include "../lib/comServo.hpp"

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


// Commands
#define SERVO_PING_CMD 0x01
#define SERVO_MEM_READ_CMD 0x02
#define SERVO_MEM_WRITE_CMD 0x03

// EEPROM (read only)
#define SERVO_EEPROM_MODEL_L 3
#define SERVO_EEPROM_MODEL_H 4

// EEPROM (read & write)
#define SERVO_EEPROM_ID 5
#define SERVO_EEPROM_BAUD_RATE 6
#define SERVO_EEPROM_MIN_ANGLE_LIMIT_L 9
#define SERVO_EEPROM_MIN_ANGLE_LIMIT_H 10
#define SERVO_EEPROM_MAX_ANGLE_LIMIT_L 11
#define SERVO_EEPROM_MAX_ANGLE_LIMIT_H 12
#define SERVO_EEPROM_CW_DEAD 26
#define SERVO_EEPROM_CCW_DEAD 27
#define SERVO_EEPROM_OFS_L 31
#define SERVO_EEPROM_OFS_H 32
#define SERVO_EEPROM_MODE 33

// SRAM (read & write)
#define SERVO_SRAM_TORQUE_ENABLE 40
#define SERVO_SRAM_ACC 41
#define SERVO_SRAM_GOAL_POSITION_L 42
#define SERVO_SRAM_GOAL_POSITION_H 43
#define SERVO_SRAM_GOAL_TIME_L 44
#define SERVO_SRAM_GOAL_TIME_H 45
#define SERVO_SRAM_GOAL_SPEED_L 46
#define SERVO_SRAM_GOAL_SPEED_H 47
#define SERVO_SRAM_TORQUE_LIMIT_L 48
#define SERVO_SRAM_TORQUE_LIMIT_H 49
#define SERVO_SRAM_LOCK 55

// SRAM (read only)
#define SERVO_SRAM_PRESENT_POSITION_L 56 // 15-bit signed (sign-magnitude), 12 bit value, 0-4095
#define SERVO_SRAM_PRESENT_POSITION_H 57
#define SERVO_SRAM_PRESENT_SPEED_L 58 // 15-bit signed (sign-magnitude)
#define SERVO_SRAM_PRESENT_SPEED_H 59
#define SERVO_SRAM_PRESENT_LOAD_L 60 // 10-bit signed (sign-magnitude) (0~1000, 1000 = 100% max load)
#define SERVO_SRAM_PRESENT_LOAD_H 61
#define SERVO_SRAM_PRESENT_VOLTAGE 62     // 8 bit unsigned
#define SERVO_SRAM_PRESENT_TEMPERATURE 63 // 8 bit unsigned, in celcius
#define SERVO_SRAM_MOVING 66              // bool?
#define SERVO_SRAM_PRESENT_CURRENT_L 69   // 15-bit signed (sign-magnitude)
#define SERVO_SRAM_PRESENT_CURRENT_H 70

// UART parameters
#define SERVO_UART_TIMOUT_ms 100

struct servo_state_t
{
  uint16_t position;
  uint16_t speed;
  uint16_t load;
  uint8_t voltageV;
  uint16_t currentMa;
};

struct servo_handle_t
{
  uint8_t servoId;
  struct servo_state_t state;
};

/*
enum servo_result_t
{
  SERVO_OK = 0x00,
  SERVO_INVALID_CHECKSUM,
  SERVO_MISSING_DATA,
  SERVO_HAL_ERR,
  SERVO_RX_TIMEOUT,
  SERVO_TX_TIMOUT,
};*/




// HAL wrapper code

// Which USART peripheral is connected to the servos
#define SERVO_UART_INSTANCE USART1

// Event flag for DMA RX completed
#define SERVO_DMA_RECEIVED_MSK 0x0001U

// Event Flags object to track when a DMA request is finished
//extern osEventFlagsId_t servo_received_flag;



class CServo
{

private:

servo_handle_t s;
ComServo *comServo = ComServo::getInstance();

uint8_t txBuffer[9] = {
    // Fill tx_buffer with preamble
    0xff,
    0xff,
};
uint8_t rxBuffer[16];


bool servoCommand(uint8_t cmd);
bool servoWriteMemAddr(uint8_t addr, uint8_t value);
bool servoWriteMemAddrU16(uint8_t addr, uint16_t value);
bool servoReadMemAddr( uint8_t addr, uint8_t *value, size_t mem_bytes);


size_t constructMessage(size_t dataBytes, ...);
bool rxChecksumOk(const uint8_t totalMsgLen);


public:
  CServo(uint8_t id, uint8_t acc, uint16_t goalSpeed);

  float servoGetPositionDegrees();
  bool servoEnableWheelMode();
  bool servoEnableServoMode();

  bool servoInit(uint8_t acc, uint16_t goalSpeed);
  bool servoPing();


  //enum servo_result_t servoReadState();

  bool servoReadVoltage();
  bool servoReadCurrent();
  bool servoReadPosition();
  bool servoReadSpeed();
  bool servoReadLoad();

  /**
 * Set servo desired speed, should only be used in wheel mode
 */
bool servoSetSpeed(uint16_t speedU16);

/**
 * Set servo desired position, should only be used in servo mode
 */
bool servoSetServoPosition(uint16_t angleU16);
	




};

#endif //_SERVO_H
