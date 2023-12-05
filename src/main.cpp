#include <Wire.h>
#include "LineFollower.h"
#include "TimeOfFlight.h"
#include "UltraSonic.h"
#include "Motors.h"

void setup()
{
    // Setup the sensors
    setUpLineFollower();
    setUpTimeOfFlight();
    setUpUltraSonic();
}

void loop()
{
    // read value for each of the sensors
    ultraSonicVal us = readUltraSonic();
    lineFollwerVal lf = readLineFollower();
    // timeOfFlightVal tof = readTimeOfFlight();

    // Set the robot to move forward
    moveForwards();

    // Check if the line follower detects a line
    if (lf.overallStatus)
    {
        stop();                         // stop motor
        bool turn = us.left > us.right; // check which side detects further object

        // Turn in the direction with the furthest object
        if (turn)
        {
            turnLeft();
        }
        else
        {
            turnRight();
        }

        // Keep turning until no longer detect line
        while (lf.left == 1 || lf.middle == 1 || lf.right == 1)
        {

            lf = readLineFollower();
        }
    }

    // Check if the TOF detects object
    // if (tof.overallStatus)
    // {
    //     stop();                         // stop motor
    //     bool turn = us.left > us.right; // check which side detects further object

    //     if (tof.leftStatus && !tof.rightStatus) // if left sensor detects object
    //     {
    //         turnRight();
    //     }
    //     else if (!tof.leftStatus && tof.rightStatus) // if right sensor detects object
    //     {
    //         turnLeft();
    //     }
    //     else // if both sensors detect object
    //     {
    //         // Turn in the direction with the furthest object
    //         if (turn)
    //         {
    //             turnLeft();
    //         }
    //         else
    //         {
    //             turnRight();
    //         }
    //     }

    //     // Keep turning until TOF doesn't detect an object any longer
    //     while (tof.overallStatus)
    //     {
    //         tof = readTimeOfFlight();
    //     }
    // }

    // Check if the ultrasonic detect object
    if (us.overallStatus)
    {
        stop(); // Stop motor

        if (us.leftStatus && !us.rightStatus) // If left ultrasonic detects object
        {
            turnRight();
        }
        else // If right ultrasonic detects object
        {
            turnLeft();
        }

        // Keep turning until ultrasonic no longer detects object
        while (us.overallStatus)
        {
            us = readUltraSonic();
        }
    }
}
