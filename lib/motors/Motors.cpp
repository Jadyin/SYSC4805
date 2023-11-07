#include "CytronMotorDriver.h"

const int SPEED_RIGHT = 3;
const int DIR_RIGHT = 2;

const int SPEED_LEFT = 5;
const int DIR_LEFT = 4;

CytronMD motorRight(PWM_DIR, SPEED_RIGHT, DIR_RIGHT);
CytronMD motorLeft(PWM_DIR, SPEED_LEFT, DIR_LEFT);

void moveForwards()
{
    motorLeft.setSpeed(255);
    motorRight.setSpeed(-255);
}

void stop()
{
    motorLeft.setSpeed(0);
    motorRight.setSpeed(0);
}