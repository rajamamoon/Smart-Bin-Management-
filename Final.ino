#include<UltraDistSensor.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <Stepper.h> // Include the header file
 
#define STEPS 4
Stepper stepper(STEPS, 8, 10, 9, 11);

LiquidCrystal_I2C lcd(0x3F,20,4);

UltraDistSensor mysensor;

float reading;// to hold value for distance
int buzzer = 7; //tocontrol buzzer
int ledPin=6; //acknowlegment for movement
int pirState=LOW; //logic for opening and closing lid

#define pirPin 4
int calibrationTime = 30;
long unsigned int lowIn;
long unsigned int pause = 1000;
boolean lockLow = true;
boolean takeLowTime;
int PIRValue = 0;

void setup() {
  
    Serial.begin(9600);
    stepper.setSpeed(500);
    
    lcd.init();                      // initialize the lcd 
    lcd.backlight();
    
    mysensor.attach(12,13);//Trigger pin , Echo pin
    pinMode(buzzer, OUTPUT);
    pinMode(pirPin, INPUT);  // initialize pir sensor as an input
    digitalWrite(pirPin, LOW);
      //give the sensor some time to calibrate
    lcd.setCursor(0,0);  
    lcd.print("Calibrating...");
    lcd.setCursor(0,1);
    lcd.print("Please Wait...");  
    Serial.print("Calibrating Sensor ");
    for(int i = 0; i < calibrationTime; i++){
    Serial.print(".");
    delay(1000);  
}
}
void loop() {

  if(digitalRead(pirPin) == HIGH){
      digitalWrite(ledPin, HIGH);   //the led visualizes the sensors output pin state
  if(lockLow){
      lockLow = false;
      Serial.println("---");
      Serial.print("Motion Detected at ");
      Serial.print(millis()/1000);
      Serial.println(" sec");
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Hi There!..."); 
      lcd.setCursor(0,1);
      lcd.print("Openning Lid");
      stepper.step(-500);
      delay(5000);
      lcd.clear();
}
takeLowTime = true;
}
  if(digitalRead(pirPin) == LOW){
     digitalWrite(ledPin, LOW);  //the led visualizes the sensors output pin state
  if(takeLowTime){
     lowIn = millis();          //save the time of the transition from high to LOW
     takeLowTime = false;       
}
    //if the sensor is low for more than the given pause,
    //we assume that no more motion is going to happen
  if(!lockLow && millis() - lowIn > pause){
    //makes sure this block of code is only executed again after
    //a new motion sequence has been detected
   lockLow = true;
   Serial.print("Motion Ended at ");      //output
   Serial.print((millis() - pause)/1000);
   Serial.println(" sec");
   lcd.clear();
   lcd.setCursor(0,0);
   lcd.print("Goodbye...");
   lcd.setCursor(0,1);
   lcd.print("Closing Lid");
   stepper.step(500);
   delay(5000);
   lcd.clear();
}
}
    
    reading=mysensor.distanceInCm();   
    lcd.setCursor(0,0);
    lcd.print("Distance: ");
    lcd.print(reading);
    Serial.println(reading);
    delay(1000);
    if (reading < 4.8){
     buzz();
    }
      }
      
void buzz()
      {
      tone(buzzer, 3000); // Send 1KHz sound signal...
      delay(500);        // ...for 1 sec
      noTone(buzzer);     // Stop sound...
      delay(500);        // ...for 1sec
 
       tone(buzzer, 3000); // Send 1KHz sound signal...
       delay(500);        // ...for 1 sec
       noTone(buzzer);     // Stop sound...
       delay(500);        // ...for 1sec     
        }


 
