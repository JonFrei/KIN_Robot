  #include <Servo.h>

Servo lowerServo;
Servo upperServo;
Servo baseServo;
Servo clawServo;

int lowerPos = 90;
int upperPos = 90;
int basePos = 90;
int clawPos = 90;

int lowerAdj = 0; //Positive = forward, Negative = down
int upperAdj = 0; //Positive = up, Negative = down
int baseAdj = 0; //Positive = CCW, Negative = CW


void setup() {
  lowerServo.attach(5);
  upperServo.attach(6);
  baseServo.attach(3);
  clawServo.attach(9);

  lowerServo.write(lowerPos);
  upperServo.write(upperPos);
  baseServo.write(basePos);

  Serial.begin(9600);

}

void loop() {


  lowerServo.write(lowerPos + lowerAdj);
  upperServo.write(upperPos + upperAdj);
  baseServo.write(basePos + baseAdj);

}
