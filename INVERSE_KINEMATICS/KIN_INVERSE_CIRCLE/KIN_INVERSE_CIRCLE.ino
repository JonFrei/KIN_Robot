#include <Servo.h>

Servo Base;
Servo lowerJoint;
Servo upperJoint;
Servo Claw;

float x;
float y;
float z;

float Cx;
float Cy;

const float Radius = 2;
const int CenterX = 8;
const int CenterY = 0;

void setup() 
{
  Base.attach(3);
  lowerJoint.attach(5);
  upperJoint.attach(6);
  Claw.attach(9);

  Serial.begin(9600);
}

void loop() 
{
  
  for (int t = 0 ; t < 360; t+=15 )
  {
  float Cx = Radius * cos(t*(PI/180)) + CenterX;
  float Cy = Radius * sin(t*(PI/180)) + CenterY;
  math(Cx, Cy , 1); 
  }

}


void math(float x, float y, float z)
{
  const float a = 9; // Fixed arm length
  const float b = 9; // Fixed arm length
  float c1 = sqrt(sq(x) + sq(y)); //Distance form bottom joint to upmost point of arm
  float c = sqrt(sq(x)+ sq(y) + sq(z)); //Projection of "c" on to x-y plane
  float ANG = atan(z/c1); //One of the angle required to move the bottom arm
  float ANG1 = atan(y/x)*(180/PI); //Angle used to rotate robot

  float upAng = (acos((sq(c) - sq(a) - sq(b)) / (-2 * a * b))) * (180/PI);
  float lowAng = (acos((sq(a) - sq(b) - sq(c)) / (-2 * b * c))+ ANG) * (180/PI);

  const float adj1 = 0;
  const float adj2 = 0;
  const float adj3 = 0;

  float baseS = 90 - ANG1 + adj1;
  float lowJ = lowAng + adj2;
  float upJ = upAng + adj3;

  float lowJ1 = map(lowJ, 0, 180, 180, 0);
   
  float errorThreshold = 6.0;
  float baseError = fabs(Base.read() - baseS);
  float lowJointError = fabs(lowerJoint.read() - lowJ1);
  float upJointError = fabs(upperJoint.read() - upJ);
  
  while( (baseError > errorThreshold) || (lowJointError > errorThreshold) || (upJointError > errorThreshold) )
  {
    Base.write(baseS);
    lowerJoint.write(lowJ1);
    upperJoint.write(upJ);
    
    baseError = fabs(Base.read() - baseS);
    lowJointError = fabs(lowerJoint.read() - lowJ1);
    upJointError = fabs(upperJoint.read() - upJ);
  }
 

  Serial.print("baseS");
  Serial.println(baseS);
  Serial.print("lowJ1");
  Serial.println(lowJ1);
  Serial.println("upJ");
  Serial.println(upJ);
}
