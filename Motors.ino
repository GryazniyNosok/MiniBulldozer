#include <RH_ASK.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <PWMServo.h>


RH_ASK rf_driver;
RF24 radio(7, 8);  // CE, CSN


const byte address[6] = "00001";

// Max size of this struct is 32 bytes - NRF24L01 buffer limit
struct Data_Package {
  int xAxis;
  int yAxis;
  int bucketAxis;
};
Data_Package data;

//Servomotor to move the bucket
PWMServo servo;
//Default angle of the servomotor
int angle = 179;

//Pins for the first motor
int in1 = 2;  
int in2 = 4;  
int enA = 3;  

//Pins for the second mototr
int in3 = A0;  
int in4 = A1;  
int enB = 6;   


void setup() {

  Serial.begin(9600);
  //Set up the radio unit
  rf_driver.init();
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();

  //Set up the motor pins
  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);

  pinMode(enB, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  //Set the servomotor pins
  servo.attach(9);
  servo.write(angle);

  //Set all motors to OFF
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);


}


void loop() {

  //Create speed and set it to 0
  int speedA = 0;
  int speedB = 0;
  
  //Create direction for both controllersticks and the servomotor
  int directionX = 515;  
  int directionY = 515;
  int bucketdirection = 515;

  //Create buffer for radio transmission
  int buf[3];  
  //Get buffer size
  uint8_t buflen = sizeof(buf);
  //Assign deffault values to the buffer
  buf[0] = 515;
  buf[1] = 515;
  buf[2] = 515;

  
  

  //Check if connection established
  if (radio.available()) {  
    //Read data
    radio.read(&data, sizeof(Data_Package));
    rf_driver.recv((uint8_t*)&buf, &buflen);
    //Message received with valid checksum
    //Serial.print("Message Received: ");
    //Serial.print("xAxis:");
    //Serial.println(data.xAxis);
    //Serial.print("yAxis:");
    //Serial.println(data.yAxis);
    
    //Assign the values 
    directionX = data.xAxis;  //analogRead(xAxis);
    directionY = data.yAxis;
    bucketdirection = data.bucketAxis;
    
  }
  //Map the bucket input
  int input = map(bucketdirection,0,1023,0,100);
  if(input > 50)
  {
    if(angle <  180)
      {
        angle+=3;
      }
  }
  else if(input <= 48)
  {
    if(angle > 80)
      {
        angle-=3;
      }

  }
  //Move the bucket
  servo.write(angle);

  //Controller

  //ENGINE A (in1,in2) IS ON THE RIGHT
  //ENGINE B (in3,in4) IS ON THE LEFT
  //FORWARDS = X = 0 == LOW HIGH
  //BACKWARDS = X = 1024 == HIGH LOW
  //RIGHT = y = 0
  //LEFT = y = 1024
  //FORWARD LEFT = (x = 0) (y = 1024)
  //FORWARD RIGHT = (x = 0) (y = 0)
  //BACKAWRDS LEFT = (x = 1024) (y = 1024)
  //BACKWARDS RIGHT = (x = 1024) (y = 0)


  //Determine the direction and speed from the position on the controller stick
  if (directionX > 600)  //BACKWARDS
  {

    speedA = (directionX - 520) / 2;  
    speedB = (directionX - 520) / 2;



    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
  }
  if (directionX < 400)  //FORWARDS
  {
    speedA = (510 - directionX) / 2;  
    speedB = (510 - directionX) / 2;


    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);
  }
  if (directionX > 410 && directionX < 590)  
  {
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);

    digitalWrite(in3, LOW);
    digitalWrite(in4, LOW);
  }


  if (directionY < 300)  //RIGHT
  {

    if (directionX < 500)  //Forwards
    {
     speedA = map(directionY,0,300,0,255);
    }

    if (directionX > 540)  //Backwards
    {
      digitalWrite(in3, LOW);
      digitalWrite(in4, HIGH);
    }

  } else if (directionY > 700)  //LEFT
  {


    if (directionX < 500)  //Forwards
    {
      digitalWrite(in3, HIGH);
      digitalWrite(in4, LOW);
    }

    if (directionX > 540)  //Backwards
    {
      digitalWrite(in1, LOW);
      digitalWrite(in2, HIGH);
    }
  }

  //Write the speed to the motors
  analogWrite(enA, speedA);
  analogWrite(enB, speedB);


  delay(50);



}
