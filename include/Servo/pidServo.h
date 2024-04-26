#ifndef PID_SERVO_H_
#define PID_SERVO_H_

#include "stdio.h"

class PIDController {
public:
    PIDController();
    PIDController(double kp, double ki, double kd) : Kp(kp), Ki(ki), Kd(kd), integral(0), previousError(0) {}

    void init(double kp, double ki, double kd);

    double calculate(int input, double dt);

    void setGoalPosition(int position);

    void setMaxMinPosition(int max, int min);

    void reset();

private:
    int goalPosition;
    double Kp;              // Proportional gain
    double Ki;              // Integral gain
    double Kd;              // Derivative gain
    double integral;        // Integral term
    double previousError;   // Previous error

    int maxPosition = 4095;
    int minPosition = 0;
};


#endif