#include <Servo.h>
///////// ROBOT /////////////
Servo base;
Servo lowJoint;
Servo upJoint;
Servo claw;

float x = 45.000;
float y = 0.000;
float z = 45.000;
float update_val = 6.5;

/////// CONTROLLER //////////
const int buttonPin = 4;

int MaxH = 100;
int MinH = 10;
int MaxD = 100;
int MinD = 20;
int MaxHD = 90;
int MinHD = -90;

unsigned long last_time = 0;           // the last time the output pin was toggled
unsigned long debounce = 300UL;   // the debounce time, increase if the output flickers
int state = HIGH;      // the current state of the output pin
int reading;           // the current reading from the input pin
int previous = LOW;    // the previous reading from the input pin

void setup() 
{
  base.attach(3);
  lowJoint.attach(5);
  upJoint.attach(6);
  claw.attach(9);
  pinMode(buttonPin, INPUT_PULLUP);
  Serial.begin(9600);

}

void loop() 
{
  float AnaVal1 = map(analogRead(A0), 1023, 0, MinD, MaxD); // UP=0     DOWN=1023
  float AnaVal2 = map(analogRead(A1), 1023, 0, MinHD, MaxHD); // RIGHT=0  LEFT=1023
  float AnaVal3 = map(analogRead(A2), 1023, 0, MinH, MaxH); // UP=0     DOWN=1023

  reading = digitalRead(buttonPin);
  
  // if the input just went from LOW and HIGH and we've waited long enough
  // to ignore any noise on the circuit, toggle the output pin and remember
  // the time
  if (reading == HIGH && previous == LOW && millis() - last_time > debounce)
  {
    if (state == HIGH){
      claw.write(0);
      state = LOW;
    }else{
      claw.write(80);
      state = HIGH;
    }
    last_time = millis();
  }

  previous = reading;

  math(AnaVal1,AnaVal2,AnaVal3);
  
}

void math(float x, float y, float z)
{
  
  const float a = 90; // Fixed arm length
  const float b = 90; // Fixed arm length
  float c1 = sqrt(sq(x) + sq(y)); //Distance form bottom joint to upmost point of arm
  float c = sqrt(sq(x)+ sq(y) + sq(z)); //Projection of "c" on to x-y plane
  float ANG = atan(z/c1); //One of the angle required to move the bottom arm
  float ANG1 = atan(y/x)*(180/PI); //Angle used to rotate robot

  float upAng = (acos((sq(c) - sq(a) - sq(b)) / (-2 * a * b))) * (180/PI);
  float lowAng = (acos((sq(a) - sq(b) - sq(c)) / (-2 * b * c))+ ANG) * (180/PI);

  const float adj1 = 8.00;
  const float adj2 = 12.00;
  const float adj3 = 30.00;

  float baseS = 90 - ANG1 + adj1;
  float lowJ =180 - lowAng + adj2;
  float upJ = upAng + adj3;

  //float lowJ1 = map(lowJ, 0, 180, 180, 0);

  float errorThreshold = 7.00;
  float baseError = fabs(base.read() - baseS);
  float lowJointError = fabs(lowJoint.read() - lowJ);
  float upJointError = fabs(upJoint.read() - upJ);
  
  while( (baseError > errorThreshold) || (lowJointError > errorThreshold) || (upJointError > errorThreshold) )
  {
    base.write(baseS);
    lowJoint.write(lowJ);
    upJoint.write(upJ);
    baseError = fabs(base.read() - baseS);
    lowJointError = fabs(lowJoint.read() - lowJ);
    upJointError = fabs(upJoint.read() - upJ);
  }
 

  Serial.print("baseS: ");
  Serial.println(baseS);
  Serial.print("lowJ: ");
  Serial.println(lowJ);
  Serial.print("upJ: ");
  Serial.println(upJ);
  Serial.println();
  Serial.print("base error: ");
  Serial.println(baseError);
  Serial.print("lowJoint: ");
  Serial.println(lowJointError);
  Serial.print("upJ: ");
  Serial.println(upJointError);
  //Serial.println();
  Serial.println();
  
}
