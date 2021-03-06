// Make sure all "go" functions are working


#include "DualMC33926MotorShield.h"

// Ping sensor pins
const int center_ping_pin = 4;
const int left_ping_pin   = 2;
const int right_ping_pin  = 6;

//Motor Speeds
int leftSpeedPos = -100;
int leftSpeedNeg = 100;
int rightSpeedPos = 100;
int rightSpeedNeg = -100;



DualMC33926MotorShield md;
 
void stopIfFault()
{
  if (md.getFault())
  {
    Serial.println("fault");
    while(1);
  }
}
 

void setup() {
  Serial.begin(9600);
  md.init();
}


void loop() {
  go_forward();
  delay(2000);
  go_left();
  delay(2000);
  go_right();
  delay(2000);
  go_backward();
  delay(2000);
  go_stop();
  delay(2000);
}

void go_stop() {
  md.setM1Speed(0);
  md.setM2Speed(0);
}

void go_forward() {
  md.setM1Speed(rightSpeedPos);
  md.setM2Speed(leftSpeedPos);
}

void go_backward() {
  md.setM1Speed(rightSpeedNeg);
  md.setM2Speed(leftSpeedNeg);
  Serial.print("going back\n");

}

void go_left() {
  md.setM1Speed(rightSpeedPos);
  md.setM2Speed(leftSpeedNeg);
}

void go_right() {
  md.setM1Speed(rightSpeedNeg);
  md.setM2Speed(leftSpeedPos);
}
