 #include <Wire.h>
#include <Adafruit_AMG88xx.h>
#include <SparkFun_GridEYE_Arduino_Library.h>
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"

static const uint8_t PIN_MP3_TX = 53; // Connects to module's RX 
static const uint8_t PIN_MP3_RX = 52; // Connects to module's TX 
SoftwareSerial softwareSerial(PIN_MP3_RX, PIN_MP3_TX);
// Create the Player object
DFRobotDFPlayerMini player;
Adafruit_AMG88xx amg;
int laserPin = 9;


// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);
#include <Wire.h>
// Define Constants
const int stepPin = 4; // define pin for step
const int dirPin = 3;  // define pin for direction
const int buttonPin= A0;
int buttonstate = 0; 
#define motorInterfaceType 2

// Creates an instance
AccelStepper myStepper(motorInterfaceType, stepPin, dirPin);


int inputPin1 = 5;               // choose the input pin (for PIR sensor)
int pirState2 = LOW;             // we start, assuming no motion detected
int val2 = 0;


int inputPin2 = 6;               // choose the input pin (for PIR sensor)
int pirState1 = LOW;             // we start, assuming no motion detected
int val1 = 0;


int inputPin3 = 7;               // choose the input pin (for PIR sensor)
int pirState3 = LOW;             // we start, assuming no motion detected
int val3 = 0;


int inputPin4 = 8;               // choose the input pin (for PIR sensor)
int pirState4 = LOW;             // we start, assuming no motion detected
int val4 = 0;


int ledPin = A1;
int ledPin1 = A2; 

// Define step constants
#define FULLSTEP 4
#define FULLSTEP 4

// Define Motor Pins (2 Motors used)

#define motorPin1  10     // Blue   - 28BYJ48 pin 1
#define motorPin2  11    // Pink   - 28BYJ48 pin 2
#define motorPin3  12    // Yellow - 28BYJ48 pin 3
#define motorPin4  13    // Orange - 28BYJ48 pin 4
                        
                        


// Define two motor objects
// The sequence 1-3-2-4 is required for proper sequencing of 28BYJ48
AccelStepper stepper1(FULLSTEP, motorPin1, motorPin3, motorPin2, motorPin4);


void setup()
{
  
{
  // initialize the LCD
  //lcd.begin();
  lcd.init();

  // Turn on the blacklight and print a message.
  lcd.backlight();
}

    
  // set the maximum speed, acceleration factor,
  // initial speed and the target position
{(player.begin(softwareSerial));   
  // Init serial port for DFPlayer Mini
  softwareSerial.begin(9600);
    Serial.println(F("AMG88xx test"));
    lcd.print(F("AMG88xx test"));
    pinMode(laserPin, OUTPUT);
    bool status;
    
    // default settings
    status = amg.begin();
    if (!status) {
        Serial.println("Could not find a valid AMG88xx sensor, check wiring!");
        while (1);
    }
    
    Serial.println("-- Thermistor Test --");
    lcd.print("-- Thermistor Test --");

    Serial.println();

    delay(100); // let sensor boot up
}
  
  
   
  myStepper.setMaxSpeed(10000);
  myStepper.setAcceleration(50);
  myStepper.setSpeed(100);
  myStepper.moveTo(350000);
  

  
 { // 1 revolution Motor 1 CW
   
  stepper1.setMaxSpeed(1000.0);
  stepper1.setAcceleration(50.0);
  stepper1.setSpeed(500);
  stepper1.moveTo(1024); 
 } 
  
 
pinMode(buttonPin,INPUT);  
pinMode(inputPin1, INPUT);
pinMode(inputPin2, INPUT);
pinMode(inputPin3, INPUT);
pinMode(inputPin4, INPUT); 
pinMode(ledPin, OUTPUT);
pinMode(ledPin1, OUTPUT);  



}

void loop(){
 
  {
   Serial.print("Thermistor Temperature = ");
    Serial.print(amg.readThermistor());
    Serial.println(" *C");
   lcd.print("Thermistor Temperature = ");
   lcd.print(amg.readThermistor());
   lcd.print(" *C");
   
    if (amg.readThermistor() >= 31) {
 player.volume(30);
 player.play(1);  
 digitalWrite(ledPin1, HIGH);  
    // Play the first MP3 file on the SD card
    Serial.println("THREAT SOUNDS ON");
    lcd.print("THREAT SOUNDS ON");
     
    
digitalWrite(laserPin, HIGH);
  
Serial.print("Quelea detected, Turn on the Laser : ");
lcd.print("Quelea detected, Turn on the Laser : ");
 delay(1000);
Serial.println(amg.readThermistor());

} else {
 
digitalWrite(laserPin, LOW);
digitalWrite(ledPin1, LOW);
Serial.print("No Quelea, Turn off the Laser : ");
Serial.println("Connecting to DFPlayer Mini failed!");
lcd.print("No Quelea, Turn off the Laser : ");
lcd.print("Connecting to DFPlayer Mini failed!");
 delay(1000);
Serial.println(amg.readThermistor());
lcd.print(amg.readThermistor()); 
    Serial.println();

    //delay a second
    delay(1000);

}
}


  {
     buttonstate = digitalRead(buttonPin);
  // Change direction once the motor reaches target position
  if (myStepper.distanceToGo() == 0   and buttonstate == HIGH)
   
    myStepper.moveTo(-200),myStepper.setSpeed(1000),myStepper.setAcceleration(500);//(-myStepper.currentPosition());

  // Move the motor one step
  myStepper.run();
  }

  
  {
 
  val1 = digitalRead(inputPin1); // read input value
    if (val1 == HIGH) { // check if the input is HIGH
       digitalWrite(ledPin, HIGH);
    stepper1.moveTo(-512);        // Move motors
    
      stepper1.run();
   Serial.println("PIR 1 "); 
   lcd.print("PIR 1 ");  
}
else{
  digitalWrite(ledPin, LOW);
   
      stepper1.run();
       
}
 
 val2 = digitalRead(inputPin2); // read input value
    if (val2 == HIGH) {            // check if the input is HIGH
      digitalWrite(ledPin, HIGH); 
    stepper1.moveTo(0);        // Move motors
    
  
      stepper1.run();
    Serial.println("PIR 2 ");
     lcd.print("PIR 2 ");
}
else{
  digitalWrite(ledPin, LOW);
   
    
      stepper1.run();
}

val3 = digitalRead(inputPin3); // read input value
    if (val3 == HIGH) {            // check if the input is HIGH
       digitalWrite(ledPin, HIGH);
    stepper1.moveTo(512);        // Move motors
    
  
      stepper1.run();
    Serial.println("PIR 3 ");
    lcd.print("PIR 3 ");
}
else{
  digitalWrite(ledPin, LOW);
    
      stepper1.run();
}

val4 = digitalRead(inputPin4); // read input value
    if (val4 == HIGH) {            // check if the input is HIGH
       digitalWrite(ledPin, HIGH);
    stepper1.moveTo(1024);        // Move motors
    
  
      stepper1.run();
      Serial.println("PIR 4");
      lcd.print("PIR 4 ");
}
else{
  digitalWrite(ledPin, LOW);
   
    
      stepper1.run();
}
if (val4 == HIGH and val3 == HIGH) {            // check if the input is HIGH
       digitalWrite(ledPin, HIGH);
    stepper1.moveTo(1024);        // Move motors
    delay(10000);
     stepper1.moveTo(512);
    
      stepper1.run();
      Serial.println("PIR 3 AND PIR 4");
      lcd.print("PIR 3 AND PIR 4");
}
else{
  digitalWrite(ledPin, LOW);
   
    
      stepper1.run();
}
if (val2 == HIGH and val4 == HIGH) {            // check if the input is HIGH
       digitalWrite(ledPin, HIGH);
    stepper1.moveTo(0);        // Move motors
    delay(10000);
     stepper1.moveTo(1024);
    
      stepper1.run();
      Serial.println("PIR 2 and PIR 4");
      lcd.print("PIR 2 AND PIR 4");
}
else{
  digitalWrite(ledPin, LOW);
   
    
      stepper1.run();
}
if (val2 == HIGH and val3 == HIGH) {            // check if the input is HIGH
       digitalWrite(ledPin, HIGH);
    stepper1.moveTo(0);        // Move motors
    delay(10000);
     stepper1.moveTo(512);
    
      stepper1.run();
      Serial.println("PIR 2 and PIR 3");
      lcd.print("PIR 3 AND PIR 3");
}
else{
  digitalWrite(ledPin, LOW);
   
    
      stepper1.run();
}
if (val1 == HIGH and val4 == HIGH) {            // check if the input is HIGH
       digitalWrite(ledPin, HIGH);
    stepper1.moveTo(-512);        // Move motors
    delay(10000);
     stepper1.moveTo(1024);
    
      stepper1.run();
      Serial.println("PIR 1 and PIR 4");
      lcd.print("PIR 1 AND PIR 4");
}
else{
  digitalWrite(ledPin, LOW);
   
    
      stepper1.run();
}
if (val1 == HIGH and val3 == HIGH) {            // check if the input is HIGH
       digitalWrite(ledPin, HIGH);
    stepper1.moveTo(-512);        // Move motors
    delay(10000);
     stepper1.moveTo(512);
    
      stepper1.run();
      Serial.println("PIR 1 and PIR 3");
      lcd.print("PIR 1 AND PIR 3");
}
else{
  digitalWrite(ledPin, LOW);
   
    
      stepper1.run();
}
if (val1 == HIGH and val2 == HIGH) {            // check if the input is HIGH
       digitalWrite(ledPin, HIGH);
    stepper1.moveTo(-512);        // Move motors
    delay(10000);
     stepper1.moveTo(0);
    
      stepper1.run();
      Serial.println("PIR 1 and PIR 2");
      lcd.print("PIR 1 AND PIR 2");
}
else{
  digitalWrite(ledPin, LOW);
   
    
      stepper1.run();
}
if (val2 == HIGH and val3 == HIGH and val4 == HIGH) {            // check if the input is HIGH
       digitalWrite(ledPin, HIGH);
    stepper1.moveTo(0);        // Move motors
    delay(10000);
     stepper1.moveTo(512);
    delay(10000);
     stepper1.moveTo(1024);
    
      stepper1.run();
      Serial.println("PIR 2 and PIR 3 and  PIR 4");
      lcd.print("PIR2 andPIR 3 AND PIR 4");
}
else{
  digitalWrite(ledPin, LOW);
   
    
      stepper1.run();
}
if (val1 == HIGH and val3 == HIGH and val4 == HIGH) {            // check if the input is HIGH
       digitalWrite(ledPin, HIGH);
    stepper1.moveTo(-512);        // Move motors
    delay(10000);
     stepper1.moveTo(512);
    delay(10000);
     stepper1.moveTo(1024);
    
      stepper1.run();
      Serial.println("PIR 1 and PIR 3 and  PIR 4");
       lcd.print("PIR1 andPIR 2 AND PIR 4");
}
else{
  digitalWrite(ledPin, LOW);
   
    
      stepper1.run();
}
if (val1 == HIGH and val2 == HIGH and val4 == HIGH) {            // check if the input is HIGH
       digitalWrite(ledPin, HIGH);
    stepper1.moveTo(-512);        // Move motors
    delay(10000);
     stepper1.moveTo(0);
    delay(10000);
     stepper1.moveTo(1024);
    
      stepper1.run();
      Serial.println("PIR 1 and PIR 2 and  PIR 4");
       lcd.print("PIR1 andPIR 2 AND PIR 4");
}
else{
  digitalWrite(ledPin, LOW);
   
    
      stepper1.run();
}
if (val1 == HIGH and val2 == HIGH and val3 == HIGH) {            // check if the input is HIGH
       digitalWrite(ledPin, HIGH);
    stepper1.moveTo(-512);        // Move motors
    delay(10000);
     stepper1.moveTo(0);
    delay(10000);
     stepper1.moveTo(512);
    
      stepper1.run();
      Serial.println("PIR 1 and PIR 2 and PIR 3");
       lcd.print("PIR1 andPIR 2 AND PIR 3");
}
else{
  digitalWrite(ledPin, LOW);
   
    
      stepper1.run();
}
if (val1 == HIGH and val2 == HIGH and val3 == HIGH and val4 == HIGH) {            // check if the input is HIGH
       digitalWrite(ledPin, HIGH);
    stepper1.moveTo(-512);        // Move motors
    delay(10000);
     stepper1.moveTo(0);
    delay(10000);
     stepper1.moveTo(512);
    delay(10000);
     stepper1.moveTo(1024);
      stepper1.run();
      Serial.println("PIR 1 and PIR 2 and PIR 3 and PIR 4");
       lcd.print("PIR 1 and PIR 2 and PIR 3 and PIR 4");
}
else{
  digitalWrite(ledPin, LOW);
   
    
      stepper1.run();
}
}
}
