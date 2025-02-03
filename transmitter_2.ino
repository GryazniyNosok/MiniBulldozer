#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(7, 8); // CE, CSN

const byte address[6] = "00001";

// Max size of this struct is 32 bytes - NRF24L01 buffer limit
struct Data_Package {
  int xAxis = 1024;
  int yAxis = 1024;
  int bucketAxis = 1024;
  int buttonPressed = 0;
  int pass = 1234;
};

//TANK
int xAxis = A0;
int yAxis = A1;
//TANK

//BUCKET
int bucketAxis = A2;
//BUCKET

int directionX;
int directionY;

int groundbutton = 2;

int button1 = 3;
int button2 = 4;
int button3 = 5;




Data_Package data; // Create a variable with the above structure

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();

  pinMode(xAxis, INPUT);
  pinMode(yAxis, INPUT);
  pinMode(bucketAxis, INPUT);  

pinMode(groundbutton, OUTPUT);

  pinMode(button1, INPUT_PULLUP);
pinMode(button2, INPUT_PULLUP);
pinMode(button3, INPUT_PULLUP);



digitalWrite(groundbutton, LOW);
}

void loop() {
if(digitalRead(button1) == LOW)
{
  data.buttonPressed = 1;
}
if(digitalRead(button2) == LOW)
{
  data.buttonPressed = 2;
}
if(digitalRead(button3) == LOW)
{
  data.buttonPressed = 3;
}
// Serial.println(digitalRead(button1));
// Serial.println(digitalRead(button2));

Serial.println(data.buttonPressed);  
  data.xAxis = analogRead(xAxis);
  data.yAxis = analogRead(yAxis);
  data.bucketAxis = analogRead(bucketAxis);
  // Send the whole data from the structure to the receiver
   Serial.print("xAxis:");
   Serial.println(analogRead(xAxis));
   Serial.print("yAxis:");
   Serial.println(analogRead(yAxis));
  // //Serial.print("bucketAxis:");
  //Serial.println(analogRead(bucketAxis));

  radio.write(&data, sizeof(Data_Package));
  
    

  
}