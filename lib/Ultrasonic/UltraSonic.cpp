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

void setUpUltraSonic()
{
    //  Using Timer Unit for both triggering and measuring the Echo

    // -----------Setting Registers for the First Trigger Signal (Pin 2)---------------------
    PMC->PMC_PCER0 |= PMC_PCER0_PID27;                       // TC0 power ON - Timer Counter 0 channel 0 for first sensor
    PIOB->PIO_PDR |= PIO_PDR_P25;                            // Pin 2 is no more driven by GPIO
    PIOB->PIO_ABSR |= PIO_PB25B_TIOA0;                       // Assign B25 to alternative periph_B (TIOA0):
    TC0->TC_CHANNEL[0].TC_CMR = TC_CMR_TCCLKS_TIMER_CLOCK1   // MCK/2 = 42 MHz
                                | TC_CMR_WAVE                // Waveform mode
                                | TC_CMR_WAVSEL_UP_RC        // Count UP mode till RC
                                | TC_CMR_ACPA_CLEAR          // Clear TIOA0 on RA compare match
                                | TC_CMR_ACPC_SET;           // Set TIOA0 on RC compare match
    TC0->TC_CHANNEL[0].TC_RC = 2520000 - 1;                  // Set the frequency to 16.667Hz (Period 60 ms)
    TC0->TC_CHANNEL[0].TC_RA = 420 - 1;                      // Set the duty cycle (Pulse of 10 usec)
    TC0->TC_CHANNEL[0].TC_CCR = TC_CCR_SWTRG | TC_CCR_CLKEN; // Software trigger TC0 channel 0 counter and enable

    //-----------Setting Registers for the First Echo Signal (Pin A7)---------------------
    PMC->PMC_PCER0 |= PMC_PCER0_PID28;                       // Timer Counter 0 channel 1 is TC1, TC1 power ON for first sensor
    TC0->TC_CHANNEL[1].TC_CMR = TC_CMR_TCCLKS_TIMER_CLOCK1   // Capture mode, MCK/2 = 42 MHz
                                | TC_CMR_ABETRG              // TIOA is used as the external trigger
                                | TC_CMR_LDRA_FALLING        // Load RA on falling edge of TIOA
                                | TC_CMR_ETRGEDG_RISING;     // Trigger on rising edge
    TC0->TC_CHANNEL[1].TC_CCR = TC_CCR_SWTRG | TC_CCR_CLKEN; // Reset TC counter and enable

    // -----------Setting Registers for the Second Trigger Signal (Pin 12)---------------------
    PMC->PMC_PCER0 |= PMC_PCER0_PID27;                       // TC0 power ON - Timer Counter 0 channel 2 for second sensor
    PIOB->PIO_PDR |= PIO_PDR_P21;                            // Pin 12 is no more driven by GPIO
    PIOB->PIO_ABSR |= PIO_PB25B_TIOA0;                       // Assign B21 to alternative periph_B (TIOA5):
    TC0->TC_CHANNEL[2].TC_CMR = TC_CMR_TCCLKS_TIMER_CLOCK1   // MCK/2 = 42 MHz
                                | TC_CMR_WAVE                // Waveform mode
                                | TC_CMR_WAVSEL_UP_RC        // Count UP mode till RC
                                | TC_CMR_ACPA_CLEAR          // Clear TIOA5 on RA compare match
                                | TC_CMR_ACPC_SET;           // Set TIOA5 on RC compare match
    TC0->TC_CHANNEL[2].TC_RC = 2520000 - 1;                  // Set the frequency to 16.667Hz (Period 60 ms)
    TC0->TC_CHANNEL[2].TC_RA = 420 - 1;                      // Set the duty cycle (Pulse of 10 usec)
    TC0->TC_CHANNEL[2].TC_CCR = TC_CCR_SWTRG | TC_CCR_CLKEN; // Software trigger TC0 channel 2 counter and enable

    //-----------Setting Registers for the Second Echo Signal (Pin A6)---------------------
    PMC->PMC_PCER0 |= PMC_PCER0_PID28;                       // Timer Counter 0 channel 3 is TC1, TC1 power ON for second sensor
    TC0->TC_CHANNEL[3].TC_CMR = TC_CMR_TCCLKS_TIMER_CLOCK1   // Capture mode, MCK/2 = 42 MHz
                                | TC_CMR_ABETRG              // TIOA is used as the external trigger
                                | TC_CMR_LDRA_FALLING        // Load RA on falling edge of TIOA
                                | TC_CMR_ETRGEDG_RISING;     // Trigger on rising edge
    TC0->TC_CHANNEL[3].TC_CCR = TC_CCR_SWTRG | TC_CCR_CLKEN; // Reset TC counter and enable
}

ultraSonicVal readUltraSonic()
{
    ultraSonicVal result;

    volatile uint32_t CaptureCountA;
    CaptureCountA = TC0->TC_CHANNEL[1].TC_RA; // get register calue

    volatile uint32_t CaptureCountB;
    CaptureCountB = TC0->TC_CHANNEL[3].TC_RA; // get register calue

    result.right = 340.0 * CaptureCountB / (42000000.0) / 2 * 100;
    result.rightStatus = result.right <= ultraSonicThresh;
    result.left = 340.0 * CaptureCountA / (42000000.0) / 2 * 100;
    result.leftStatus = result.left <= ultraSonicThresh;

    result.overallStatus = result.leftStatus || result.rightStatus;

    return result;
}
