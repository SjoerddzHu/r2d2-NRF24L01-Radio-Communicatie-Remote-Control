/*  Code to receive data from RF24L01+ and use it to control a servo */

#include <Servo.h>
#include <SPI.h>
#include <RF24.h>

//Radio Configuration
bool radioNumber=0;
RF24 radio(9,10);
byte addresses[][6] = {"1Node","2Node"};
bool role = 0;  //Control transmit/receive

// Create variables to control servo value

Servo ForeAft;
Servo LeftRight;
unsigned int ForeAft_Output;       // Expected range 0 - 180 degrees
unsigned int LeftRight_Output;     // Expected range 0 - 180 degrees

void setup() {

  Serial.begin(9600);       // Send data back for debugging purposes
  ForeAft.attach(6);         // Plug a servo signal line into digital output pin 6
  LeftRight.attach(7);       // Plug a servo signal line into digital output pin 7
  radio.begin();
  radio.setPALevel(RF24_PA_LOW);

  radio.openWritingPipe(addresses[0]);
  radio.openReadingPipe(1,addresses[1]);

  radio.startListening();  
}

void loop() {

  delay(100);
  if(radio.available()){
         radio.read(&ForeAft_Output,sizeof(ForeAft_Output));
         radio.read(&LeftRight_Output,sizeof(ForeAft_Output));
  } else {Serial.print("No radio");
  }
  Serial.print(ForeAft_Output);
  Serial.print("\t");
  Serial.println(LeftRight_Output);
  
  ForeAft.write(ForeAft_Output);                          // Command the Fore/Aft servo to a position
  LeftRight.write(LeftRight_Output);                    // Command the Left/Right servo to a position

}
