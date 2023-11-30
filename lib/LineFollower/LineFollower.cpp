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
    // setup all pins for Line Follower
    pinMode(LEFT, INPUT);
    pinMode(RIGHT, INPUT);
    pinMode(MIDDLE, INPUT);
}

lineFollwerVal readLineFollower()
{
    lineFollwerVal result;
    // Read values of each sensor
    result.left = digitalRead(LEFT);
    result.right = digitalRead(RIGHT);
    result.middle = digitalRead(MIDDLE);

    // Determine if it detects line
    result.overallStatus = (result.left + result.middle + result.right) >= lineFollowerThresh;

    return result;
}
