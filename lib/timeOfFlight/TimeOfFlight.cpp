#include <Wire.h>
#include <VL53L1X.h>
#include "Thresholds.h"

struct timeOfFlightVal
{
    int left;
    bool leftStatus;
    int right;
    bool rightStatus;

    bool overallStatus;
};

// The number of sensors in your system.
const uint8_t sensorCount = 1; // TODO: make 2

// The Arduino pin connected to the XSHUT pin of each sensor.
const uint8_t xshutPins[sensorCount] = {7}; // TODO: make {6, 7}

VL53L1X sensors[sensorCount];

void setUpTimeOfFlight()
{
    Wire.begin();
    Wire.setClock(400000); // use 400 kHz I2C

    // Disable/reset all sensors by driving their XSHUT pins low.
    for (uint8_t i = 0; i < sensorCount; i++)
    {
        pinMode(xshutPins[i], OUTPUT);
        digitalWrite(xshutPins[i], LOW);
    }

    // Enable, initialize, and start each sensor, one by one.
    for (uint8_t i = 0; i < sensorCount; i++)
    {
        // Stop driving this sensor's XSHUT low. This should allow the carrier
        // board to pull it high. (We do NOT want to drive XSHUT high since it is
        // not level shifted.) Then wait a bit for the sensor to start up.
        pinMode(xshutPins[i], INPUT);
        delay(10);

        sensors[i].setTimeout(500);
        if (!sensors[i].init())
        {
            Serial.print("Failed to detect and initialize sensor ");
            Serial.println(i);
            while (1)
                ;
        }

        // Each sensor must have its address changed to a unique value other than
        // the default of 0x29 (except for the last one, which could be left at
        // the default). To make it simple, we'll just count up from 0x2A.
        sensors[i].setAddress(0x2A + i);
        // sensors[i].setDistanceMode(VL53L1X::Short);
        // sensors[i].setMeasurementTimingBudget(20000); // 20 msec
        sensors[i].startContinuous(50);
    }
}

timeOfFlightVal readTimeOfFlight()
{
    timeOfFlightVal result;

    // read each of the sensor values and determine if they are above threshold (detects object)
    result.right = sensors[0].timeoutOccurred() ? 0 : sensors[0].read();
    result.rightStatus = timeOfFlightThresh >= result.right;
    result.left = sensors[1].timeoutOccurred() ? 0 : sensors[1].read();
    result.leftStatus = timeOfFlightThresh >= result.left;

    result.overallStatus = result.leftStatus || result.rightStatus;

    return result;
}
