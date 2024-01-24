#include <Arduino.h>
#include <FlexCAN_T4.h>
#include "TimerOne.h"

// CAN Settings
#define CAN_BAUD_RATE 250000 // Adjust to your CAN bus speed

// Encoder Settings
const unsigned int pulsesPerRevolution = 50;      // Replace with your encoder's pulses per revolution
const float wheelCircumferenceInInches = 19.27;   // Replace with your wheel's circumference in inches

// CAN Message
CAN_message_t canMsg;

// Speed calculation variables
volatile unsigned int counter = 0;
const unsigned long timerIntervalMicros = 500000;  // Timer interval in microseconds

// CAN1 object
FlexCAN_T4<CAN1, RX_SIZE_256, TX_SIZE_16> can1;

// Timer ISR
void Timer_Isr()
{
  Timer1.detachInterrupt(); // Disable the timer

  // Calculate speed in pulses per second
  float pulsesPerSecond = static_cast<float>(counter);

  // Convert pulses per second to miles per hour
  float speedInMPH = (pulsesPerSecond * 60.0 * 60.0 * wheelCircumferenceInInches) / (pulsesPerRevolution * 63360.0);

  // Update CAN message data with the speed
  canMsg.len = 4;
  canMsg.buf[0] = static_cast<uint8_t>(speedInMPH);           // Assuming speed is within byte range
  canMsg.buf[1] = static_cast<uint8_t>(speedInMPH * 100);      // Two decimal places
  canMsg.buf[2] = 0; // Placeholder for additional data
  canMsg.buf[3] = 0; // Placeholder for additional data

  // Send CAN message using FlexCAN_T4 library
  can1.write(canMsg);

  // Print speed to Serial monitor
  Serial.print("Speed: ");
  Serial.print(speedInMPH, 2); // Print speed with two decimal places
  Serial.println(" MPH");

  counter = 0;                      // Reset counter to zero
  Timer1.attachInterrupt(Timer_Isr); // Re-enable the timer
}

// Encoder ISR
void Do_Count1()
{
  counter++; // Increase +1 the counter value
}

void setup()
{
  Serial.begin(9600);

  // Initialize CAN bus using FlexCAN_T4 library
  can1.begin();
  can1.setBaudRate(CAN_BAUD_RATE);

  // Initialize Timer and Encoder
  Timer1.initialize(timerIntervalMicros);           // Set timer to trigger every 0.5 seconds
  attachInterrupt(0, Do_Count1, RISING); // Attach interrupt to handle encoder pulses
  Timer1.attachInterrupt(Timer_Isr);    // Define Interrupt Service Routine (ISR)
}

void loop()
{
  // Send CAN message
  canMsg.flags.extended = 1;
  canMsg.id = 0x18FEDF00;
  canMsg.len = 8;
  for (uint8_t i = 0; i < 8; i++)
    canMsg.buf[i] = i + 1;

  can1.write(canMsg);

  // You can add delay or other logic as needed
  delay(500);
}
