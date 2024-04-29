#ifndef SERVO_H
#define SERVO_H

#include "SMS_STS.h"
#include "INST.h"

struct Servo
{
    volatile u8 &id;
    volatile int &position;
    volatile s16 &setPosition;
    volatile s16 &setSpeed;
    volatile u8 &setAcc;
};

#endif // SERVO_H