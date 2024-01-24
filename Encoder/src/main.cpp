// Include necessary libraries
#include <Arduino.h>
#include "TimerOne.h"

unsigned int pulsesPerRevolution = 50; // Replace with your encoder's pulses per revolution
float wheelCircumferenceInInches = 19.27; // Replace with your wheel's circumference in inches

unsigned int counter = 0; // Holds the count of pulses detected

void Do_Count1();
void Timer_Isr();

//===============================================================================
// Initialization
//===============================================================================
void setup()
{
  Serial.begin(9600);

  Timer1.initialize(500000);           // Set timer to trigger every 1 second
  attachInterrupt(0, Do_Count1, RISING); // Attach interrupt to handle encoder pulses
  Timer1.attachInterrupt(Timer_Isr);    // Define Interrupt Service Routine (ISR)
}

//===============================================================================
// Main
//===============================================================================
void loop()
{
  // Do nothing, everything is handled by the interrupt and timer routines
}

//===============================================================================
// Subroutine - Increments the speed sensor counter when interrupt pin goes high
//===============================================================================
void Do_Count1()
{
  counter++; // Increase +1 the counter value
}

//===============================================================================
// Subroutine - Interrupt Service Routine (ISR)
// Prints out speed info every second and restarts the counter
//===============================================================================
void Timer_Isr()
{
  Timer1.detachInterrupt(); // Disable the timer

  // Calculate speed in pulses per second
  float pulsesPerSecond = float(counter);

  // Convert pulses per second to miles per hour
  float speedInMPH = (pulsesPerSecond * 60 * 60 * wheelCircumferenceInInches) / (pulsesPerRevolution * 63360);

  Serial.print("Speed: ");
  Serial.print(speedInMPH, 2); // Print speed with two decimal places
  Serial.println(" MPH");

  counter = 0;                      // Reset counter to zero
  Timer1.attachInterrupt(Timer_Isr); // Re-enable the timer
}
