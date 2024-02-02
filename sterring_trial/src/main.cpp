#include <Arduino.h>
#include <FlexCAN_T4.h>

const int pin3V3 = 1;    
const int pinEnable = 3;  

void setup() {
  pinMode(pin3V3, OUTPUT);
  pinMode(pinEnable, OUTPUT);
}

void loop() {
  
  digitalWrite(pin3V3, HIGH);

  // Enable PWM by setting the enable pin
  digitalWrite(pinEnable, HIGH);



  // 5% duty cycle
  analogWrite(pin3V3, 13);  // (5% of 255)

  delay(1000);  // 1 second delay

  // 50% duty cycle
  analogWrite(pin3V3, 128);  // (50% of 255)

  delay(1000);  // 1 second delay

  // 95% duty cycle
  analogWrite(pin3V3, 242);  // (95% of 255)

  delay(1000);  // 1 second delay

  // Stop PWM by setting the PWM value to 0
  // analogWrite(pin3V3, 0);

  // Disable 3.3V pin
  digitalWrite(pin3V3, LOW);

  // Disable PWM by resetting the enable pin
  digitalWrite(pinEnable, LOW);


}
