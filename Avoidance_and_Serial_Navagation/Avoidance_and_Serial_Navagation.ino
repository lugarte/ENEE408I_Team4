// ENEE408I Spring 2020
// Code adapted from Team 2 (Fall 2019) ardControlerV2
// Code for robot to navagate while avoiding obstacles 

/* CURRENT STATUS: 
      This code is being tested, it makes the robot navagate around obstacles.
      It currently works with serial commands entered from the serial monitor on a laptop but hasn't been tested with other serial command inputs yet.

      If the robot sees an obstacle, it avoids it.  If there is no obstacle, it follows serial commands sent to it.
      If there are no obstacles or serial commands, the robot slowly turns left to look for objects.

      Logic of obstacle avoidance:
      If it sees an obstacle in front of it, it goes back for 2 seconds then turns left for 1 second.
      If it sees an obstacle on the right, it turns left until it doesn't see the obstacle.
      If it sees an obstacle on the left, it turns right until it doesn't see the obstacle.
     
      Sensor readings and the command that the robot is following are printed to Serial for debugging.
*/
#include "DualMC33926MotorShield.h"
#include "Ultrasound.h"

//constants
byte PWM = 60;
const int STOP_DISTANCE_CENTER = 15; // cm
const int STOP_DISTANCE_SIDE   = 10; // cm

// enum for Direction
enum Directions {forward, backward, left, right, halt, leftSlow};
  // 0 - forward, 1 - backwards, 2 - left, 3 - right, 4 - halt

//globals for motor control
boolean avoidObstaclesEnabled = false; // Default to false
boolean freeRoam = false; // Not supported
Directions currDir = halt;
Directions prevDir = halt;

//globals for crash avoidance
boolean dangerCenter = false;
boolean dangerLeft   = false;
boolean dangerRight  = false;
boolean dangerDetected = false;
long leftDistance = 0, centerDistance = 0, rightDistance = 0;
boolean lastCommandFromSerial = false;

// Ping sensor pins - Do NOT use Digital 4, 7, 8, 9, 10 , 12 or Analog 1 or 2, those are taken by motor driver
const int pingPinFront = 3;
const int pingPinLeft  = 2;
const int pingPinRight  = 6;


// TODO: do we need these?
// OOP Initializations
//MotorControl leftMotor  (LeftMotorA_pin,  LeftMotorB_pin,  LeftMotorPWM_pin);
//MotorControl rightMotor (RightMotorA_pin, RightMotorB_pin, RightMotorPWM_pin);
Ultrasound leftUltrasound   (pingPinLeft);
Ultrasound centerUltrasound (pingPinFront);
Ultrasound rightUltrasound  (pingPinRight);


//Motor Speeds
int leftSpeedPos = -100; //the left motor is facing opposite direction of right, so reversing commands here
int leftSpeedNeg = 100;
int rightSpeedPos = 100;
int rightSpeedNeg = -100;
int leftSpeedSlowPos = -50; //the left motor is facing opposite direction of right, so reversing commands here
int leftSpeedSlowNeg = 50;
int rightSpeedSlowPos = 50;
int rightSpeedSlowNeg = -50;

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
  restartSequence();
}

void loop() {
// The code below is old code used to have the robot free roam around the room while avoiding anything
// that falls into its path
        char incomingCharacter;
        bool serialCommandAvailable;

        Serial.println("(1/6) starting loop");


        if(Serial.available() > 1) {//read incoming character
                while(Serial.available() > 1){//make sure if buffer is finished so we follow most recent command
                incomingCharacter = Serial.read();
                serialCommandAvailable = true;
                Serial.println("(2/6) Read something from serial - in while loop");
                }
        }
        else{
               serialCommandAvailable = false;
               Serial.println("(2/6) No Serial Commands");
        }

        // Check where the danger is...
        updatePingReadings();
        
        if (centerDistance <= STOP_DISTANCE_CENTER) { // If Danger at center...
          prevDir = currDir;
          currDir = backward;
          respondToCurrDir();
        }
        else if (leftDistance <= STOP_DISTANCE_SIDE) { // Issue @ left, so go right
            prevDir = currDir;
            currDir = right;
            respondToCurrDir();
        }
        else if (rightDistance <= STOP_DISTANCE_SIDE) { // Issue @ right, so go left
            prevDir = currDir;
            currDir = left;
            respondToCurrDir();            
        }
        else if (serialCommandAvailable == true) { // No obstacle - check for Serial commands
              Serial.println("(3/6) no obstacle- Serial commands!");
                  switch(incomingCharacter) {
                    case '\n': // Disregard newlines
                      break;
                    case 'f':
                    Serial.println("(4/6) Serial commands:f, changing direction to foward");
                      prevDir = currDir; 
                      currDir = forward;
                      respondToCurrDir(); 
                      break;
                    case 'l':
                      Serial.println("(4/6) Serial commands:l, changing direction to left");
                      prevDir = currDir; 
                      currDir = left;
                      respondToCurrDir(); 
                      break;
                    case 'r':
                      Serial.println("(4/6) Serial commands:r, changing direction to right");
                      prevDir = currDir;  
                      currDir = right;
                      respondToCurrDir(); 
                      break;
                    case 'b': 
                      Serial.println("(4/6) Serial commands:b, changing direction to back");
                      prevDir = currDir; 
                      currDir = backward;
                      respondToCurrDir(); 
                      break;
                    case 'h': 
                      prevDir = currDir; 
                      currDir = halt;
                      respondToCurrDir(); 
                      break;
                    default: 
                      Serial.println("ERROR: Bad Message from Serial - character was not expected"); 
                      break;                                         
               }
        }
        else { //No issues or Serial commands - go left slowly
            Serial.println("no issue- and no serial commands...\n\n");

            prevDir = currDir;
            currDir = leftSlow;
            //currDir = halt;
            respondToCurrDir();
        }
    }


void respondToCurrDir() {  
 
  if(prevDir == backward){//if last direction was go back, then go left
    currDir = left;
  }
  
  if (currDir != prevDir) {//only send instructions to change motor speed if the direction has changed
     Serial.println("(4/6) The current direction != last direction");
    if (currDir == forward){
           Serial.println("(5/6) Calling go_foward");
      go_forward();
    }
    else if (currDir == left)
      go_left();
    else if (currDir == right)
      go_right();
    else if (currDir == backward){
      go_backward();
    }
    else if (currDir == halt)
      go_stop();
    else if (currDir == leftSlow)
      go_left_slow();
    else
      Serial.println("Error in respondToCurrDir() - currDir not found");
  }
  else Serial.print("Command not changed...");
}



void go_stop() {
  md.setM1Speed(0);
  md.setM2Speed(0);
  Serial.println("Stopping\n\n");
  delay(5000);


}

void go_forward() {
   Serial.println("(6/6) In go_foward");

  md.setM1Speed(rightSpeedPos);
  md.setM2Speed(leftSpeedPos);
  Serial.println("Going Foward \n\n");
}

void go_backward() {
  md.setM1Speed(rightSpeedNeg);
  md.setM2Speed(leftSpeedNeg); 
  Serial.println("Going Back\n\n");
  delay(2000);
}

void go_left() {
  md.setM1Speed(rightSpeedPos);
  md.setM2Speed(leftSpeedNeg);
  Serial.println("Going Left\n\n");
  if(prevDir == backward){
    delay(1000);
  }

}

void go_left_slow() {
  md.setM1Speed(rightSpeedSlowPos);
  md.setM2Speed(leftSpeedSlowNeg);
  Serial.println("Going Left Slowly\n\n");
}

void go_right() {
  md.setM1Speed(rightSpeedNeg);
  md.setM2Speed(leftSpeedPos);
  Serial.println("Going Right\n\n");
}


void updatePingReadings(){
      
     updatePingCenter();
     updatePingRight();
     updatePingLeft();
}

void updatePingCenter(){
  long duration;
  pinMode(pingPinFront, OUTPUT);
  digitalWrite(pingPinFront, LOW);
  delayMicroseconds(2);
  digitalWrite(pingPinFront, HIGH);  
  delayMicroseconds(5);
  digitalWrite(pingPinFront, LOW);
  pinMode(pingPinFront, INPUT);
  duration = pulseIn(pingPinFront, HIGH);
  centerDistance = microsecondsToCentimeters(duration);

  Serial.print("Front sensor = ");
  Serial.println(centerDistance);
 
}

void updatePingLeft(){
  long duration;
  pinMode(pingPinLeft, OUTPUT);
  digitalWrite(pingPinLeft, LOW);
  delayMicroseconds(2);
  digitalWrite(pingPinLeft, HIGH);  
  delayMicroseconds(5);
  digitalWrite(pingPinLeft, LOW);
  pinMode(pingPinLeft, INPUT);
  duration = pulseIn(pingPinLeft, HIGH);
  leftDistance = microsecondsToCentimeters(duration);

  Serial.print("Left sensor = ");
  Serial.println(leftDistance);
}

void updatePingRight(){
  long duration;
  pinMode(pingPinRight, OUTPUT);
  digitalWrite(pingPinRight, LOW);
  delayMicroseconds(2);
  digitalWrite(pingPinRight, HIGH);  
  delayMicroseconds(5);
  digitalWrite(pingPinRight, LOW);
  pinMode(pingPinRight, INPUT);
  duration = pulseIn(pingPinRight, HIGH);
  rightDistance = microsecondsToCentimeters(duration);

  Serial.print("Right sensor = ");
  Serial.println(rightDistance);

}


long microsecondsToCentimeters(long microseconds) {
  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the object we
  // take half of the distance travelled.
  return microseconds / 29 / 2;
}

void restartSequence(){
  Serial.print("STARTING UP....");

  go_forward();
  delay(500);
  go_backward();
  delay(500);
  go_left();
  delay(500);
  go_right();
  delay(500);

  Serial.print("DONE.");
}
