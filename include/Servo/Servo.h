#ifndef SERVO_H
#define SERVO_H

#include "SMS_STS.h"
#include "INST.h"

struct Servo
{
    u8 &id;
    int &position;
    s16 &setPosition;
    s16 &setSpeed;
    u8 &setAcc;
};

#endif // SERVO_H