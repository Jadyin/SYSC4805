#include "CytronMotorDriver.h"

const int SPEED_RIGHT = 3;
const int DIR_RIGHT = 2;

const int SPEED_LEFT = 5;
const int DIR_LEFT = 4;

// initalize motors' pins
CytronMD motorRight(PWM_DIR, SPEED_RIGHT, DIR_RIGHT);
CytronMD motorLeft(PWM_DIR, SPEED_LEFT, DIR_LEFT);

void moveForwards() // Set wheels to move forward
{
    motorLeft.setSpeed(255);
    motorRight.setSpeed(-255);
}

void stop() // Set wheels to stop
{
    motorLeft.setSpeed(0);
    motorRight.setSpeed(0);
}

void turnLeft() // Set wheels to turn left
{
    motorLeft.setSpeed(255);
    motorRight.setSpeed(255);
}

void turnRight() // Set wheels to turn right
{
    motorLeft.setSpeed(-255);
    motorRight.setSpeed(-255);
}