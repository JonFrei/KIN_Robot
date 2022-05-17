#include <Servo.h>

Servo base;
Servo lowJoint;
Servo upJoint;


float x;
float y;
float z;

int tiltA = 180;
int twistA = 90;

void setup() 
{
  base.attach(3);
  lowJoint.attach(5);
  upJoint.attach(6);
  

  Serial.begin(9600);

}

void loop() 
{

  
  math(4,2,4);
  delay(1500);
  math(8,2,4);
  delay(1500);
  math(8,-2,4);
  delay(1500);
  math(4,-2,4);
  delay(1500);
  math(4,2,4);
  delay(1500);

  
  math(4,2,8);
  delay(1500);
  math(8,2,8);
  delay(1500);
  math(8,-2,8);
  delay(1500);
  math(4,-2,8);
  delay(1500);
  math(4,2,8);
  delay(1500);

  
  
}

void math(float x, float y, float z)
{
  const float a = 9; // Fixed arm length
  const float b = 9; // Fixed arm length
  float c1 = sqrt(sq(x) + sq(y)); //Distance form bottom joint to upmost point of arm
  float c = sqrt(sq(x)+ sq(y) + sq(z)); //Projection of "c" on to x-y plane
  float ANG = atan(z/c1); //One of the angle required to move the bottom arm
  float ANG1 = atan(y/x)*(180/PI); //Angle used to rotate robot
Serial.println("ANG1");
Serial.println(ANG1);
  float upAng = (acos((sq(c) - sq(a) - sq(b)) / (-2 * a * b))) * (180/PI);
  float lowAng = (acos((sq(a) - sq(b) - sq(c)) / (-2 * b * c))+ ANG) * (180/PI);

  const float adj1 = 0;
  const float adj2 = 0;
  const float adj3 = 0;

  float baseS = 90 - ANG1 + adj1;
  float lowJ = lowAng + adj2;
  float upJ = upAng + adj3;

  float lowJ1 = map(lowJ, 0, 180, 180, 0);
   
  /*while( (base.read() < baseS || base.read() > baseS) && (lowJoint.read() < lowJ || lowJoint.read() > lowJ) && (upJoint.read() < upJ || upJoint.read() > upJ))
  {
    base.write(baseS);
    lowJoint.write(lowJ1);
    upJoint.write(upJ); 
  }
*/

  float errorThreshold = 8.0;
  float baseError = fabs(base.read() - baseS);
  float lowJointError = fabs(lowJoint.read() - lowJ1);
  float upJointError = fabs(upJoint.read() - upJ);
  
  while( (baseError > errorThreshold) || (lowJointError > errorThreshold) || (upJointError > errorThreshold) )
  {
    base.write(baseS);
    lowJoint.write(lowJ1);
    upJoint.write(upJ);
    Serial.print("Base error: ");
    Serial.println(baseError);
    Serial.print("Base angle: ");
    Serial.println(base.read());
    Serial.print("Base calc angle: ");
    Serial.println(baseS);
    baseError = fabs(base.read() - baseS);
    lowJointError = fabs(lowJoint.read() - lowJ1);
    upJointError = fabs(upJoint.read() - upJ);
  }
 

//  Serial.print("baseS");
//  Serial.println(baseS);
  //Serial.print("lowJ1");
//  Serial.println(lowJ1);
//  Serial.println("upJ");
//  Serial.println(upJ);
}
