/*  Code to read Joystick position and transmit it with a RF24L01+ to a receiver */
#include <SPI.h>
#include <RF24.h>

// Radio Configuration
RF24 radio(9,10);
byte addresses[][6] = {"1Node","2Node"};
bool radioNumber=1;
bool role = 1;  //Control transmit/receive

// Decide where you are going to plug the joystick into the circuitboard.
int ForeAft_Pin = 0;        // Plug Joystick Fore/Aft into Analog pin 0
int LeftRight_Pin = 1;      // Plug Joystick Left/Right into Analog pin 1

// Create variables to read joystick values
float ForeAft_Input ;       // Variable to store data for Fore/Aft input from joystick
float LeftRight_Input ;     // Variable to store data for for Left/Right input from joystick

// Create variables to transmit servo value
int ForeAft_Output;       // Expected range 0 - 180 degrees
int LeftRight_Output;     // Expected range 0 - 180 degrees

// These variables allow for math conversions and later error checking as the program evolves.
int Fore_Limit = 800;       // High ADC Range of Joystick ForeAft
int Aft_Limit = 220;        // Low ADC Range of Joystick ForeAft
int Right_Limit = 800;      // High ADC Range of Joystick LeftRight
int Left_Limit = 226;       // Low ADC Range of Joystick LeftRight

void setup() {
    Serial.begin(9600);       // Send data back for debugging purposes
    radio.begin();
    radio.setPALevel(RF24_PA_LOW);  //Set the power to low

    radio.openWritingPipe(addresses[1]);
    radio.openReadingPipe(1,addresses[0]);

  }




void loop() {
  ForeAft_Input = analogRead(ForeAft_Pin) ;              // Read the Fore/Aft joystick value
  LeftRight_Input = analogRead(LeftRight_Pin) ;         // Read the Left/Right joystick value
  ForeAft_Output = convertForeAftToServo(ForeAft_Input) ;        // Convert the Fore/Aft joystick value to a Servo value (0-180)
  LeftRight_Output = convertLeftRightToServo(LeftRight_Input) ;  // Convert the Left/Right joystick value to a Servo value (0-180)

//  Serial.print(ForeAft_Output);

  radio.stopListening();
  delay(100);
  if(radio.write(&ForeAft_Output, sizeof(ForeAft_Output)),Serial.println("sent ForeAft"));
  if(radio.write(&LeftRight_Output, sizeof(LeftRight_Output)),Serial.println("sent LeftRight"));
  radio.startListening();
  

}

// Function to convert and scale the Fore/Aft data
// Can be replaced with Map function
float convertForeAftToServo(float y) {
  int result;
  result = ((y - Aft_Limit) / (Fore_Limit - Aft_Limit) * 180);
  return result;
}

// Function to convert and scale the Left / Right data
// Can be replaced with Map function
float convertLeftRightToServo(float x) {
  int result;
  result = ((x - Left_Limit) / (Right_Limit - Left_Limit) * 180);
  return result;
}





