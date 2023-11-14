#include <DueTimer.h>
#include <Wire.h>
#include "Thresholds.h"

struct ultraSonicVal
{
    int left;
    bool leftStatus;
    int right;
    bool rightStatus;

    bool overallStatus;
};

#define trigPin1 9  // Pin 12 trigger output
#define trigPin2 11 // Pin 12 trigger output

#define echoPin1 10 // Pin 2 Echo input
#define echoPin2 12 // Pin 2 Echo input

volatile long echo_start1 = 0;    // Records start of echo pulse
volatile long echo_end1 = 0;      // Records end of echo pulse
volatile long echo_duration1 = 0; // Duration - difference between end and start
volatile long echo_start2 = 0;    // Records start of echo pulse
volatile long echo_end2 = 0;      // Records end of echo pulse
volatile long echo_duration2 = 0; // Duration - difference between end and start

// ----------------------------------
// setup() routine called first.
// A one time routine executed at power up or reset time.
// Used to initialise hardware.
// ----------------------------------

void goLow()
{
    // sets all the pins to low
    // Serial.println("pin goes low");
    digitalWrite(trigPin1, LOW);
    digitalWrite(trigPin2, LOW);
    Timer7.detachInterrupt(); // detachs the interupt so that it won't go off again until initialized
}

void trigger_pulse()
{
    // sets all pins to high or triggers the ultrasonic
    digitalWrite(trigPin1, HIGH);
    digitalWrite(trigPin2, HIGH);
    Timer7.attachInterrupt(goLow).start(50); // sets a  timer that will make the pulse go low after 50 us
}

void echo_interrupt1()
{
    switch (digitalRead(echoPin1)) // Test to see if the signal is high or low
    {
    case HIGH:                  // High so must be the start of the echo pulse
        echo_end1 = 0;          // Clear the end time
        echo_start1 = micros(); // Save the start time
        break;

    case LOW:                                     // Low so must be the end of the echo pulse
        echo_end1 = micros();                     // Save the end time
        echo_duration1 = echo_end1 - echo_start1; // Calculate the pulse duration
        break;
    }
}

void echo_interrupt2()
{
    switch (digitalRead(echoPin2)) // Test to see if the signal is high or low
    {
    case HIGH:                  // High so must be the start of the echo pulse
        echo_end2 = 0;          // Clear the end time
        echo_start2 = micros(); // Save the start time
        break;

    case LOW:                                     // Low so must be the end of hte echo pulse
        echo_end2 = micros();                     // Save the end time
        echo_duration2 = echo_end2 - echo_start2; // Calculate the pulse duration
        break;
    }
}

void setUpUltraSonic()
{
    pinMode(trigPin1, OUTPUT); // Trigger pin set to output
    pinMode(trigPin2, OUTPUT);
    pinMode(echoPin1, INPUT);
    pinMode(echoPin2, INPUT);                            // Echo pin set to input
    Timer6.attachInterrupt(trigger_pulse).start(250000); // sends a pulse every 250 ms and lasts 50 us

    attachInterrupt(echoPin1, echo_interrupt1, CHANGE); // Attach interrupt to the sensor echo input
    attachInterrupt(echoPin2, echo_interrupt2, CHANGE); // Attach interrupt to the sensor echo input
}
