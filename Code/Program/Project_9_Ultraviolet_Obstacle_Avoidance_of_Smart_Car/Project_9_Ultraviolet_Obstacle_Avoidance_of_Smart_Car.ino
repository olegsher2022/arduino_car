#include <Servo.h> 
int pinLB = 2;     // defining pin 12
int pinLF = 4;     // defining pin 3 
int pinRB = 7;    // defining pin 13 
int pinRF = 8;    // defining pin 11 
int Lpwm_pin = 5;  //adjusting speed 
int Rpwm_pin = 10;   //adjusting speed //
unsigned char Lpwm_val = 200;
unsigned char Rpwm_val = 200;
////////////////////////////////
int inputPin = A0;    // defining receiving pin of ultrasonic signal
int outputPin =A1;    // defining emitting pin of ultrasonic signal

int Fspeedd = 0;      // forward speed
int Rspeedd = 0;      // right speed
int Lspeedd = 0;      // left speed
int directionn = 0;   // front=8 back=2 left=4 right=6 
Servo myservo;        // setting myservo
int delay_time = 250; // time for servo motor turning backward

int Fgo = 8;         // going forward
int Rgo = 6;         // turning right
int Lgo = 4;         // turning left
int Bgo = 2;         // turning backward

void setup()
 {
  Serial.begin(9600);     // defining output pin of motor 
  pinMode(pinLB,OUTPUT); // pin 12
  pinMode(pinLF,OUTPUT); // pin 3 (PWM)
  pinMode(pinRB,OUTPUT); // pin 13
  pinMode(pinRF,OUTPUT); // pin 11 (PWM) 
  pinMode(inputPin, INPUT);    // defining input pin of ultrasonic
  pinMode(outputPin, OUTPUT);  // defining output pin of ultrasonic   
  myservo.attach(3);    // defining output pin9 of motor
 }
void advance()     // going forward
    { 
    digitalWrite(pinLB,LOW);    // right wheel going forward
    digitalWrite(pinRB, LOW);  // left wheel going forward
    digitalWrite(pinLF,HIGH);   
    digitalWrite(pinRF,HIGH);
    }
void stopp()         //stop
    {
     digitalWrite(pinRB,HIGH);
     digitalWrite(pinRF,HIGH);
     digitalWrite(pinLB,HIGH);
     digitalWrite(pinLF,HIGH);
    }
void right()        //turning right(single wheel)
    {
  
     digitalWrite(pinRB,LOW);  //making motor move towards right rear
     digitalWrite(pinRF,HIGH);
     digitalWrite(pinLB,HIGH);
     digitalWrite(pinLF,LOW);  //making motor move towards left front
    }
void left()         //turning left(single wheel)
    {
     digitalWrite(pinRB,HIGH);
     digitalWrite(pinRF,LOW );   //making motor move towards right front
     digitalWrite(pinLB,LOW);   //making motor move towards left rear
     digitalWrite(pinLF,HIGH);
    }
  
void back()          //going backward
    {
     digitalWrite(pinRB,HIGH);  //making motor move towards right rear
     digitalWrite(pinRF,LOW);
     digitalWrite(pinLB,HIGH);  //making motor move towards left rear
     digitalWrite(pinLF,LOW);  
    }
void detection()        //measuring 3 angles(0.90.179)
    {      
      int delay_time = 250;   // time for servo motor turning backward
      ask_pin_F();            // reading out the front distance
     if(Fspeedd < 20)         // assuming the front distance less than 10cm
      {
      stopp();               // clear output material
      delay(100);
      back();                // going backward for 0.2 second
      delay(200);
      }
           
      if(Fspeedd < 40)         // assuming the front distance less than 25cm
      {
        stopp();  
        delay(100);             // clear output material 
        ask_pin_L();            // reading out the left distance
        delay(delay_time);      // waiting servo motor to be stable
        ask_pin_R();            // reading out the right distance  
        delay(delay_time);      // waiting servo motor to be stable  
        
        if(Lspeedd > Rspeedd)   //assuming left distance more than  right distance
        {
          directionn = Lgo;      //turning left
        }
        
        if(Lspeedd <= Rspeedd)   //assuming left distance less than or equal to right distance
        {
         directionn = Rgo;      //turning right
        } 
        
        if (Lspeedd < 15 && Rspeedd < 15)   //assuming both left distance and right distance less than  10cm
        {
         directionn = Bgo;      //going backward        
        }          
      }
      else                      //assuming the front distance  more than 25 cm   
      {
        directionn = Fgo;        //going forward     
      }
     
    }    
void ask_pin_F()   // measuring the front distance 
    {
      myservo.write(90);
      digitalWrite(outputPin, LOW);   // ultrasonic launching low voltage at 2μs
      delayMicroseconds(2);
      digitalWrite(outputPin, HIGH);  // ultrasonic launching high voltage at 10μs，at least at10μs
      delayMicroseconds(10);
      digitalWrite(outputPin, LOW);    // keeping ultrasonic launching low voltage
      float Fdistance = pulseIn(inputPin, HIGH);  // time of error reading
      Fdistance= Fdistance/5.8/10;       // converting time into distance（unit：cm） 
      Fspeedd = Fdistance;              // reading-in Fspeedd(fore speed) with distance
    }  
 void ask_pin_L()   // measuring left distance 
    {
      myservo.write(5);
      delay(delay_time);
      digitalWrite(outputPin, LOW);   // ultrasonic launching low voltage at 2μs
      delayMicroseconds(2);
      digitalWrite(outputPin, HIGH);  // ultrasonic launching high voltage at 10μs，at least at10μs
      delayMicroseconds(10);
      digitalWrite(outputPin, LOW);    // keeping ultrasonic launching low voltage
      float Ldistance = pulseIn(inputPin, HIGH);  // time of error reading
      Ldistance= Ldistance/5.8/10;       // converting time into distance（unit：cm）
      Lspeedd = Ldistance;              //reading-in Lspeedd(left speed) with distance 
    }  
void ask_pin_R()   // measuring right distance 
    {
      myservo.write(177);
      delay(delay_time);
      digitalWrite(outputPin, LOW);   // ultrasonic launching low voltage at 2μs
      delayMicroseconds(2);
      digitalWrite(outputPin, HIGH);  // ultrasonic launching high voltage at 10μs，at least at10μs
      delayMicroseconds(10);
      digitalWrite(outputPin, LOW);    // keeping ultrasonic launching low voltage
      float Rdistance = pulseIn(inputPin, HIGH);  // time of error reading
      Rdistance= Rdistance/5.8/10;       // onverting time into distance（unit：cm）
      Rspeedd = Rdistance;              // reading-in Rspeedd(right speed) with distance 
    }  
    
void loop()
  {
    myservo.write(90);  //making motor regression, being ready for next measurement
    detection();        //measuring angle and deciding which direction it moves towards   
   if(directionn == 2)  //supposing direction = 2(back up)             
   {
     back(); 
     delay(800);                    //  back up
     left() ;      
     delay(200);              //moving slightly towards left(avoiding locked)
   }
   if(directionn == 6)           //supposing direction = 6(turning right)    
   {
     back();
     delay(100);  
     right();  
     delay(600);                 // turning right
   }
   if(directionn == 4)          //supposing direction = 4(turning left)    
   {  
     back(); 
     delay(600);      
     left(); 
     delay(600);                  // turning left
   }  
   if(directionn == 8)          //supposing direction =  = 8(going forwards)      
   { 
    advance();                 // going forwards normally  
    delay(100);  
   }
   }
