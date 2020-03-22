/////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//SCREEN
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
// Das LCD Keypad-Shield steckt auf den Smartcars.
// Mit den Buttons können die Choreos ausgewählt werden.
// Mit btnNONE wird nichts gemacht.
// Bsp:  
// btnRIGHT  Choreo 1
// btnUP     Chroeo 2
// btnDOWN   Choreo 3
// btnLEFT   Choreo 4
// btnSELECT Choreo 5
//////////////////////////////////////////////////////////


//Sample using LiquidCrystal library
#include <LiquidCrystal.h>
// select the pins used on the LCD panel
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

// define some values used by the panel and buttons
int lcd_key     = 0;
int adc_key_in  = 0;
#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5

// read the buttons
int read_LCD_buttons()
{
 adc_key_in = analogRead(0);      // read the value from the sensor 
 // my buttons when read are centered at these valies: 0, 144, 329, 504, 741
 // we add approx 50 to those values and check to see if we are close
 if (adc_key_in > 1000) return btnNONE; // We make this the 1st option for speed reasons since it will be the most likely result
 // For V1.1 us this threshold
 if (adc_key_in < 50)   return btnRIGHT;  
 if (adc_key_in < 250)  return btnUP; 
 if (adc_key_in < 450)  return btnDOWN; 
 if (adc_key_in < 650)  return btnLEFT; 
 if (adc_key_in < 850)  return btnSELECT;  

 return btnNONE;  // when all others fail, return this...
}
//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////
//MOVEMENTS
/////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////
 // Used Libraries
 /////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////
#include <Servo.h>
Servo myservo;  // create servo object to control a servo
int  pos = 0;    // variable to store the servo position
#define ServoPin 16 //servo on pin A2 (=16)
//Define Pins
//////////////////////////////////////////////////////////////
//Motor A links
//////////////////////////////////////////////////////////////
#define ENA   11
#define N1    12
#define N2    13 // Port A4=18
//////////////////////////////////////////////////////////////
//Motor B rechts
//////////////////////////////////////////////////////////////
#define ENB   3
#define N3    2
#define N4    18
//////////////////////////////////////////////////////////////
// Blaue LED
//////////////////////////////////////////////////////////////
#define led  17 // Pin A3=17
//////////////////////////////////////////////////////////////
// IR-Sensor analog Signal
//////////////////////////////////////////////////////////////
#define IR  15 // Pin A1=15
//////////////////////////////////////////////////////////////
//millis///

#define onTime1 1000
#define onTime2 2000
#define onTime3 325
#define onTime4 500
int phase;
int spirale;
int enbgeschwindigkeit;
unsigned long startTime;


// Variable speed ist ein int. Umwandlung von int in char: sprintf(speedstring, "%d", speed);
// Danach Ausgabe als Char auf LCD-Display mit lcd.print(speedstring);
char speedstring[4];
//////////////////////////////////////////////////////////////

float ADCValue;
float UIR = 0.00; // Spannung des IR-Sensors. 2V=12cm, 2.4V = 10cm, 3V=7cm. 
                  // Car Stop, wenn Sensoroutput > 2V
float RefSpannung = 4.96;

void setup()
{
  /////////////////////////////////////////////////////////////////////
  //SCREEN
  ////////////////////////////////////////////////////////////////////
 lcd.begin(16, 2);              // start the library
 lcd.setCursor(0,0);
 lcd.print("Push the buttons"); // print a simple message
 /////////////////////////////////////////////////////////////////////
 //MOVEMENTS
 //////////////////////////////////////////////////////////////////////
 myservo.attach(ServoPin);  // attaches the servo on pin A2 (=16) to the servo object
 Serial.begin (9600);
 //configure pin modes
 pinMode (ENA, OUTPUT);
 pinMode (N1, OUTPUT);
 pinMode (N2, OUTPUT);
 pinMode (ENB, OUTPUT);
 pinMode (N3, OUTPUT);
 pinMode (N4, OUTPUT);
 pinMode (led, OUTPUT); 
 pinMode (IR, INPUT); 

 startTime = millis();
 phase = 1;
 spirale = 2;
}
 
void loop()
{
 lcd.setCursor(9,1);            // move cursor to second line "1" and 9 spaces over


 lcd.setCursor(0,1);            // move to the begining of the second line
 lcd_key = read_LCD_buttons();  // read the buttons

 switch (lcd_key)               // depending on which button was pushed, we perform an action
 {
   case btnRIGHT:
     {
     lcd.print("TASK 5");
     startTime = millis();
     while (true){
      task5();
      }
     break;
     }
   case btnLEFT:
     {
     lcd.print("TASK 2");
     analogWrite (ENA, 100); // Speed Control mit Werten von 0-255(=FullSpeed)
     analogWrite (ENB, 100);
     startTime = millis();
     while (true){
      task2();
      }
     
     
     break;
     }
   case btnUP:
     {
     lcd.print("TASK 3");
     startTime = millis();
     while (true){
      task3();
      };
     break;
     }
   case btnDOWN:
     {
     lcd.print("TASK 4");
     startTime = millis();
     while (true){
      task4();
      }
     break;
     }
   case btnSELECT:
     {
     lcd.print("TASK 1");
     analogWrite (ENA, 255); // Speed Control mit Werten von 0-255(=FullSpeed)
     analogWrite (ENB, 255);
     startTime = millis();
     while (true){
      task1();
      }
     break;
     }
     case btnNONE:
     {
     lcd.print("TASK  ");
     break;
     }
 }

}

void task1() {  
  switch (phase) {

    case 1:
      digitalWrite (N1, HIGH);
      digitalWrite (N2, HIGH);
      digitalWrite (N3, HIGH);
      digitalWrite (N4, HIGH);
     
     if (millis() - startTime > onTime2){
        startTime=millis();
        phase = 2;
      }
      break;
    case 2:
      digitalWrite (N1, HIGH);
      digitalWrite (N2, LOW);
      digitalWrite (N3, HIGH);
      digitalWrite (N4, LOW);
      if (millis() - startTime > onTime1){
        startTime=millis();
        
        phase = 3;
      }
      break;

     

     case 3:
      digitalWrite (N1, HIGH);
      digitalWrite (N2, HIGH);
      digitalWrite (N3, HIGH);
      digitalWrite (N4, HIGH);
      
      if (millis() - startTime > onTime2){
        startTime=millis();
        phase = 4;
      }
      break;
      
      case 4:

        digitalWrite (N1, LOW);
        digitalWrite (N2, HIGH);
        digitalWrite (N3, LOW);
        digitalWrite (N4, HIGH);
      
      if (millis() - startTime > onTime1){
        startTime=millis();
        phase = 1;
      }
      break;
  }
}

void task2() {  
  switch (phase) {

    case 1:
      //vorwärts fahren mit max speed

      digitalWrite (N1, HIGH);
      digitalWrite (N2, LOW);
      digitalWrite (N3, HIGH);
      digitalWrite (N4, LOW);
     
     if (millis() - startTime > onTime2){
        digitalWrite (N1, HIGH);
        digitalWrite (N2, HIGH);
        digitalWrite (N3, HIGH);
        digitalWrite (N4, HIGH);
        delay(onTime2);
        startTime=millis();
       
        phase = 2;
      }
      break;
    case 2:
      //kurve rechts

      digitalWrite (N1, HIGH);
      digitalWrite (N2, LOW);
      digitalWrite (N3, LOW);
      digitalWrite (N4, HIGH);
      if (millis() - startTime > onTime3){
        digitalWrite (N1, HIGH);
        digitalWrite (N2, HIGH);
        digitalWrite (N3, HIGH);
        digitalWrite (N4, HIGH);
        delay(onTime2);
        startTime=millis();
        
        phase = 3;
      }
      break;

     

     case 3:

        digitalWrite (N1, HIGH);
        digitalWrite (N2, LOW);
        digitalWrite (N3, HIGH);
        digitalWrite (N4, LOW);
      
      if (millis() - startTime > onTime2){
        digitalWrite (N1, HIGH);
        digitalWrite (N2, HIGH);
        digitalWrite (N3, HIGH);
        digitalWrite (N4, HIGH);
        delay(onTime2);
        startTime=millis();
        
        phase = 4;
      }
      break;
      
      case 4:
        //kurve links

        digitalWrite (N1, LOW);
        digitalWrite (N2, HIGH);
        digitalWrite (N3, HIGH);
        digitalWrite (N4, LOW);
      
        if (millis() - startTime > onTime3){
          digitalWrite (N1, HIGH);
          digitalWrite (N2, HIGH);
          digitalWrite (N3, HIGH);
          digitalWrite (N4, HIGH);
          delay(onTime2);
          startTime=millis();
          
          phase = 1;
        }
        break;
  }
}

void task3() { 
  analogWrite (ENA, 255); // Schnell
  analogWrite (ENB, 100); // langsam
  digitalWrite (N1, HIGH); //-> Arduino dreht sich im Kreis
  digitalWrite (N2, LOW);
  digitalWrite (N3, HIGH);
  digitalWrite (N4, LOW);
}

void task4() {  
  switch (phase) {

    case 1:
      //kurve links
      analogWrite (ENA, 80); // Speed Control mit Werten von 0-255(=FullSpeed)
      analogWrite (ENB, 0);
      digitalWrite (N1, HIGH);
      digitalWrite (N2, LOW);
      digitalWrite (N3, HIGH);
      digitalWrite (N4, LOW);
     
     if (millis() - startTime > onTime4){
        digitalWrite (N1, HIGH);
        digitalWrite (N2, HIGH);
        digitalWrite (N3, HIGH);
        digitalWrite (N4, HIGH);
        startTime=millis();
       
        phase = 2;
      }
      break;
    case 2:
      analogWrite (ENA, 80); // Speed Control mit Werten von 0-255(=FullSpeed)
      analogWrite (ENB, 100);
      digitalWrite (N1, HIGH);
      digitalWrite (N2, LOW);
      digitalWrite (N3, HIGH);
      digitalWrite (N4, LOW);
      
      if (millis() - startTime > onTime4){
        digitalWrite (N1, HIGH);
        digitalWrite (N2, HIGH);
        digitalWrite (N3, HIGH);
        digitalWrite (N4, HIGH);
        //delay(onTime2);
        startTime=millis();
        
        phase = 3;
      }
      
      break;

     case 3:
//kurve rechts

      analogWrite (ENA, 0); // Speed Control mit Werten von 0-255(=FullSpeed)
      analogWrite (ENB, 100);
      digitalWrite (N1, HIGH);
      digitalWrite (N2, LOW);
      digitalWrite (N3, HIGH);
      digitalWrite (N4, LOW);
      if (millis() - startTime > onTime4){
        digitalWrite (N1, HIGH);
        digitalWrite (N2, HIGH);
        digitalWrite (N3, HIGH);
        digitalWrite (N4, HIGH);
        startTime=millis();
        
        phase = 4;
      }
        
      break;
      
      case 4:
        analogWrite (ENA, 80); // Speed Control mit Werten von 0-255(=FullSpeed)
        analogWrite (ENB, 100);

        digitalWrite (N1, HIGH);
        digitalWrite (N2, LOW);
        digitalWrite (N3, HIGH);
        digitalWrite (N4, LOW);
      
        if (millis() - startTime > onTime4){
          digitalWrite (N1, HIGH);
          digitalWrite (N2, HIGH);
          digitalWrite (N3, HIGH);
          digitalWrite (N4, HIGH);
          //delay(onTime2);
          startTime=millis();
          
          phase = 1;
        }
        break;
  }
}

void task5() {  
  switch (1) {

    case 1:
      if (spirale < 20){
        enbgeschwindigkeit = 100 - (spirale * 5);
      }
      else{
        enbgeschwindigkeit = 0;
      }
      //kurve links
      analogWrite (ENA, 100); // Speed Control mit Werten von 0-255(=FullSpeed)
      analogWrite (ENB, enbgeschwindigkeit);
      digitalWrite (N1, HIGH);
      digitalWrite (N2, LOW);
      digitalWrite (N3, HIGH);
      digitalWrite (N4, LOW);
     
     if (millis() - startTime > onTime1){
        spirale =spirale + 1;
        startTime=millis();
       
        phase = 1;
      }
      break;
    
  }
}
