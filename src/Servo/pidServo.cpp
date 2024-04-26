
#include "../include/Servo/pidServo.h"

void PIDController::init(double kp, double ki, double kd)
{
    Kp = kp;
    Ki = ki;
    Kd = kd;
    integral = 0;
    previousError = 0;
}

double PIDController::calculate(int input, double dt) 
{
    double error = goalPosition - input;
    integral += error * dt;
    double derivative = (error - previousError) / dt;
    double output = Kp * error + Ki * integral + Kd * derivative;
    previousError = error;
    return output;
}

void PIDController::setGoalPosition(int position)
{
    if(position >= maxPosition)
        position = maxPosition;
    else if(position <= minPosition)
        position = minPosition;
    goalPosition = position;
}

void PIDController::setMaxMinPosition(int max, int min)
{
    maxPosition = max;
    minPosition = min;
}

void PIDController::reset()
{
    integral = 0;
    previousError = 0;
}