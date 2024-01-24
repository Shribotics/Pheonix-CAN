#include <Arduino.h>
#include <FlexCAN_T4.h>
#include "TimerOne.h"

// CAN Settings
#define CAN_BAUD_RATE 250000 // Adjust to your CAN bus speed

// Encoder Settings
const unsigned int pulsesPerRevolution = 50;
const float wheelCircumferenceInInches = 19.27;

// CAN Message
CAN_message_t canMsg;

// Speed calculation variables
static volatile unsigned int counter = 0;
static const unsigned long timerIntervalMicros = 500000;  // Timer interval in microseconds

// CAN1 object
FlexCAN_T4<CAN1, RX_SIZE_256, TX_SIZE_16> can1;

// Timer ISR
void Timer_Isr()
{
  Timer1.detachInterrupt(); // Disable the timer

  // ... (unchanged code)

  // Re-enable the timer
  Timer1.attachInterrupt(Timer_Isr);
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
  Timer1.initialize(timerIntervalMicros);
  attachInterrupt(0, Do_Count1, RISING);
  Timer1.attachInterrupt(Timer_Isr);
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
