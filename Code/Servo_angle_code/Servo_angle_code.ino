#include <Servo.h>// define head file; note that can direct go to Arduino menu bar to click on Sketch>Importlibrary>Servo, call Servo function; or can direct input #include <Servo.h>，but there must be space between #include and <Servo.h>, otherwise compiling is failed.

Servo myservo;//define servo variable name
void setup()
{
myservo.attach(9);//define servo pin
}
void loop()
{
myservo.write(90);// set rotation angle of servo
}
