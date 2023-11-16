#include <Wire.h>
#include "Thresholds.h"

#define LEFT 22
#define RIGHT 24
#define MIDDLE 26

struct lineFollwerVal
{
    int left;
    int middle;
    int right;

    bool overallStatus;
};

void setUpLineFollower()
{
    pinMode(LEFT, INPUT);
    pinMode(RIGHT, INPUT);
    pinMode(MIDDLE, INPUT);
}

lineFollwerVal readLineFollower()
{
    lineFollwerVal result;
    result.left = digitalRead(LEFT);
    result.right = digitalRead(RIGHT);
    result.middle = digitalRead(MIDDLE);

    result.overallStatus = (result.left + result.middle + result.right) >= lineFollowerThresh;

    return result;
}
