#include <Stepper.h>  //load library

#define home_switch 13 // Arduino pin for microswitch
String homing = "";
#include "SR04.h"
#define TRIG_PIN 6
#define ECHO_PIN 7
SR04 sr04 = SR04(ECHO_PIN,TRIG_PIN);
long a;

int mm = 200; //steps to move 1mm
int pos;      // Used to set HOME position after Homing is completed
const int steps_per_rev=200; // number of steps required to rotate the motor 
int motSpeed=200; //define motor speed
int delaytime=2000; 
Stepper myStepper(steps_per_rev, 8,10,9,11); //create stepper and tell it how many stpes/rev as well as pint sequence 

void checkDistance() {
a=sr04.Distance();
Serial.print("Distance Check:");
Serial.println(a);
if(a > (pos/10)+7 || a < (pos/10)+5 ) { 
  Serial.print("Error Detected: Position is not accurate after ");
  Serial.print(millis());
  Serial.println("milliseconds");
  delay(1000);
  digitalWrite(5, LOW);
}
}

void setStepperIdle() { //Function to turn off coils when not in use
  digitalWrite(8, LOW);
  digitalWrite(9, LOW);
  digitalWrite(10, LOW);
  digitalWrite(11, LOW);
}

void setStepperHome() {   //Homing function for the stepper motor
  while (digitalRead(home_switch)) {  // Do this until the switch is activated
    myStepper.step(-100);             // Move up .5mm
    myStepper.setSpeed(motSpeed);
    setStepperIdle();     //turn off coils while not in use
  }

  pos = 0;  // Reset position variable to zero 
  Serial.print("Position: ");
  Serial.println(pos); //confirm pos is set to 0
}

void setup() {
  // set the speed at 200 rpm:
  myStepper.setSpeed(motSpeed);
  digitalWrite (5, HIGH);
  pinMode(5, OUTPUT);
  digitalWrite (4, HIGH);
  pinMode (4, OUTPUT);
  setStepperHome(); // runs the homing function
  // initialize the serial port:
  Serial.begin(9600);
}

void loop() {
  //put your main code here, to run repeatedly: 
  while (Serial.available() == 0) {}     //wait for data available
 int x = Serial.parseInt();  // reads integer input into serial monitor
 pos = pos + x;       // incriment position variable by number typed in serial monitor
 myStepper.step(x*mm);  // move number of mm corresponding to typed integer
  setStepperIdle();    // turn off coils when not in use
  Serial.print("Position: "); 
  Serial.println(pos);  //print updated position
  checkDistance();


 String homing = Serial.readString();  //read text string from serial monitor
 homing.trim();           // removes invisible characters from text string
 if(homing == "home") {     //if 0home is typed run homing command
   setStepperHome();
 }
  //read until timeout
 homing.trim(); 
 if(homing == "test") {   //move 10mm down then 1mm down 30 times, then run homing command if test is typed
   myStepper.step(10*mm);
   setStepperIdle();
   delay (delaytime);
   for (int i = 0; i < 30; i++){
   myStepper.step(1*mm);
    setStepperIdle();
    delay (delaytime);
   }
    setStepperHome();
 }  
 if(homing == "run") {  //Identical to test command, however waits until an integer is typed and runs cycle that many times
  while (Serial.available() == 0) {}  //wait for input in serial monitor
  int y = Serial.parseInt();      //number of times for cycle to run
  for (int i = 0; i < y; i++){
    Serial.print(millis());
       myStepper.step(65*mm);
   setStepperIdle();
   pos=pos+65;
    Serial.print("Position: "); 
   Serial.println(pos);
   checkDistance();
   delay (120000);
   for (int i = 0; i < 30; i++){    //moves stepper 1mm 30 times
   myStepper.step(1*mm);
    setStepperIdle();
    pos=pos+1;
    Serial.print("Position: "); 
   Serial.println(pos);
   checkDistance();
    delay (60000);    //change depending on how long between each measurement
   }
   for (int i = 0; i < 30; i++){    //moves stepper 1mm 30 times
   myStepper.step(-1*mm);
    setStepperIdle();
    pos=pos-1;
    Serial.print("Position: "); 
   Serial.println(pos);
   checkDistance();
    delay (60000);    //change depending on how long between each measurement
   }
    setStepperHome();
    delay (delaytime);
  }
 }
 if(homing == "osc") {
   while (Serial.available() == 0) {  //wait for input in serial monitor
   for (int i = 0; i < 5; i++){
   myStepper.step(-1*mm);
    setStepperIdle();
    delay(delaytime);
 }
 for (int i = 0; i < 5; i++){
   myStepper.step(1*mm);
    setStepperIdle();
    delay(delaytime);
 }
 checkDistance();
  }
 }
}
