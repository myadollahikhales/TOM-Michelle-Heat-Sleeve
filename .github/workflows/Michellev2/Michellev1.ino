  /*

 Heating Pad Hand Warmer Blanket Code Example
  SparkFun Electronics, Pamela, 1/24/2013
  Beerware License

  Hardware Connections:
  -led1 = D9;
  -led2 = D10;
  -led3 = D11;
  -button = D2;
  -Mofset = D3;

  Usage: 
  Hit the switch to power, hit the button to adjust how warm the heating elements get, and three LEDs will indicate low, medium, and high levels. 
 */

#include <Wire.h> // Used to establied serial communication on the I2C bus
#include "SparkFunTMP102.h" // Used to send and recieve specific information from our sensor

// Temp sensor Connections
// VCC = 3.3V
// GND = GND
// SDA = A4
// SCL = A5
TMP102 sensor0(0x48); // Initialize sensor at I2C address 0x48


//Define the pin positions
int btnPin = 2; 
boolean btnPressed = false;
int mosPin = 3;
int led1 = 9;
int led2 = 10;
int led3 = 11;
int mode=3;
unsigned long startTime;
unsigned long currentTime;


//Begin the system
void setup() {  
  Serial.begin(9600);   
  Wire.begin();      
  sensor0.begin();  // Join I2C bus
  startTime = millis();

     
  // initialize the digital pin as an output.
  // input mode reads the state of the pin 
  // output mode directs power to the pin 
  pinMode(btnPin, INPUT);  
  pinMode(mosPin, OUTPUT);  
  pinMode(led1, OUTPUT);  
  pinMode(led2, OUTPUT);  
  pinMode(led3, OUTPUT);  
}



void loop() {

  //temp sensor  
  sensor0.wakeup();
  float temp;
  temp = sensor0.readTempF();
  
  currentTime = millis();
  if ((currentTime - startTime) >= 5000){ // enough time passed yet?
    Serial.print("Fahrenheit: ");
    Serial.println(temp);
    Serial.print("Current Heat Level: "); Serial.println(mode);  
    startTime=currentTime; //reset time
  }
  sensor0.sleep();

  
  //Increment mode on depress, unless mode = 3, then reset to 0
  btnPressed = digitalRead(btnPin);
//  Serial.println(btnPressed);
  
  if (btnPressed){
    Serial.println("button was pressed");

    if(mode ==3)
      mode = 1;
    else 
      mode = mode+1;
      
    delay(2000);
    }

  //conditions to automate and take input from user

  //Cond1: DEFAULT- level HIGH
  if (mode==3 || temp < 72){
    analogWrite(mosPin,255); //255 is highest power of mosfet
    digitalWrite(led1, HIGH);
    digitalWrite(led2, HIGH);
    digitalWrite(led3, HIGH); 
//    Serial.print("Level "); Serial.print(mode);  
//    Serial.println(" is now activated!");     
  }

  else if (mode==2){
    analogWrite(mosPin,200);
    digitalWrite(led1, HIGH);
    digitalWrite(led2, HIGH);
    digitalWrite(led3, LOW); 
//    Serial.print("Level "); Serial.print(mode);  
//    Serial.println(" is now activated!"); 
  }

  else if (mode==1){
    analogWrite(mosPin,170);
    digitalWrite(led1, HIGH);
    digitalWrite(led2, LOW);
    digitalWrite(led3, LOW); 
//    Serial.print("Level "); Serial.print(mode);  
//    Serial.println(" is now activated!"); 
  }

  else if (temp >200){
    analogWrite(mosPin,0); //turn off
    digitalWrite(led1, LOW);
    digitalWrite(led2, LOW);
    digitalWrite(led3, LOW);   
    Serial.println("Temperature too high. Heat is now OFF."); 
  }

  //wait for readings
  delay(300);  

  
}
