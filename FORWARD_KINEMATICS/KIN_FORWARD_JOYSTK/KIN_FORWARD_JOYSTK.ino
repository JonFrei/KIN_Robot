#include <Servo.h>

Servo lowerServo;
Servo upperServo;
Servo baseServo;
Servo clawServo;

int lowerPos = 90;
int upperPos = 90;
int basePos = 90;
int clawPos = 90;

int lowerAdj = 0;
int upperAdj = -20;
int baseAdj = -11;

int lowerMax = 135;
int lowerMin = 45;
int upperMax = 135;
int upperMin = 45;
int baseMax = 135;
int baseMin = 45;

int buttonPin = 4;


unsigned long lastTime = 0;
unsigned long debounce = 300UL;
int state = HIGH;
int reading;
int previous = LOW;



void setup() {
  lowerServo.attach(5);
  upperServo.attach(6);
  baseServo.attach(3);
  clawServo.attach(9);

  lowerServo.write(lowerPos);
  upperServo.write(upperPos);
  baseServo.write(basePos);

  pinMode(buttonPin, INPUT_PULLUP);
  Serial.begin(9600);

}

void loop() {

  int lowerPos = map(analogRead(A0), 1023, 0, lowerMin, lowerMax); // UP=0     DOWN=1023
  int basePos = map(analogRead(A1), 1023, 0, upperMin, upperMax); // RIGHT=0  LEFT=1023
  int upperPos = map(analogRead(A2), 1023, 0, baseMin, baseMax); // UP=0     DOWN=1023


  reading = digitalRead(buttonPin);
  
  if (reading == HIGH && previous == LOW && millis() - lastTime > debounce)
  {
    if (state == HIGH){
      clawServo.write(0);
      state = LOW;
    }else{
      clawServo.write(80);
      state = HIGH;
    }
    lastTime = millis();
  }

  previous = reading;

  lowerServo.write(lowerPos + lowerAdj);
  upperServo.write(upperPos + upperAdj);
  baseServo.write(basePos + baseAdj);

}
