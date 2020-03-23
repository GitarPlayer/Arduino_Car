#include <Servo.h>
Servo myservo;  // create servo object to control a servo
#define servoPin 16
#define sensorPin 15
#define ENA    11
#define N1     12
#define N2     13 
#define ENB     3
#define N3      2
#define N4      18
byte geschwindigkeit;                        // Die aktuelle Geschwindigkeit
byte korrekturBewegungProzent;               // Stärke der Korrektur in Prozent
bool korrekturBewegungRichtung;              // true = links, false = rechts
bool sensorikRichtung = true;                // Momentane Drehrichtung des Servos, true = links, false = rechts
byte sensorikPosition = 0;                   // Sensor-Position, 0 = rechts, 180 = links
unsigned long sensorikVerzoegerung;          // Verzögerung für Schwenkung und Messung der Sensorik
unsigned long gehirnVerzoegerung;
unsigned long bewegungAusweichenVerzoegerung; // Verzögerungsvariable für Ausweich-Funktion   // NEU!!!!!!!

float sensorWert;                            // Momentaner Sensor-Wert
float refSpannung = 4.96;                    // Korrektur für Sensor von 2A
const float sensorMin = 512;                 // Sensor-Wert bei freiem Feld
const float sensorMax = 2048;                // Gewünschter Maximalwert des Sensors

void setup() {
  Serial.begin(9600);         /* DEV */
  
  myservo.attach(servoPin);
  myservo.write(sensorikPosition);

  pinMode(sensorPin, INPUT);
  pinMode(ENA, OUTPUT);
  pinMode(N1, OUTPUT);
  pinMode(N2, OUTPUT);
  pinMode(N3, OUTPUT);
  pinMode(N4, OUTPUT);
    
  pinMode(ENA, OUTPUT);

  sensorikVerzoegerung = 0;
  gehirnVerzoegerung = 0;
  bewegungAusweichenVerzoegerung = 1000;               // NEU!!!!!!!
}

void loop() {
  // put your main code here, to run repeatedly:
  gehirn();
  analogWrite(ENA, geschwindigkeit);        /* DEV */
  analogWrite(ENB, geschwindigkeit);        /* DEV */  
  digitalWrite(N1, LOW);                    /* DEV */ 
  digitalWrite (N2, HIGH);                  /* DEV */ 
  digitalWrite (N3, HIGH);                  /* DEV */ 
  digitalWrite (N4, LOW);                   /* DEV */ 
  
}

//  Sensorik-Routine
//  ----------------

void sensorikInfrarot() {
  if (sensorikVerzoegerung < millis()) {  // Verzögerung für Schwenkung und Messung
    if (sensorikRichtung) {               // 
      sensorikPosition++;        
    }
    else {
      sensorikPosition--;
    }
    myservo.write(sensorikPosition);
    if (sensorikPosition >= 180 || sensorikPosition <= 0) {
        sensorikRichtung = !sensorikRichtung;
    }
    sensorikVerzoegerung = millis()+5;    // 5ms Verzögerung für Servo
    sensorWert = analogRead(sensorPin)*refSpannung;
    Serial.println(sensorWert);               /* DEV */
    Serial.println(geschwindigkeit);          /* DEV */
  }
}


//  Bewegungsfunktionen:
//  --------------------

void bewegungFahrt(byte richtungFahrt) {          // richtungFahrt: 0 = geradeaus, 1 = links, 2 = rechts
  if (richtungFahrt == 0) {
    analogWrite(ENA, geschwindigkeit);        /* DEV */
    analogWrite(ENB, geschwindigkeit);        /* DEV */  
    digitalWrite(N1, LOW);                    /* DEV */ 
    digitalWrite (N2, HIGH);                  /* DEV */ 
    digitalWrite (N3, HIGH);                  /* DEV */ 
    digitalWrite (N4, LOW);                   /* DEV */ 
  }
  else if (richtungFahrt == 1) {
    analogWrite(ENA, geschwindigkeit);        /* DEV */
    analogWrite(ENB, geschwindigkeit-20);     /* DEV */  
    digitalWrite(N1, LOW);                    /* DEV */ 
    digitalWrite (N2, HIGH);                  /* DEV */ 
    digitalWrite (N3, HIGH);                  /* DEV */ 
    digitalWrite (N4, LOW);                   /* DEV */ 
  }
  else { 
    analogWrite(ENA, geschwindigkeit-20);      
    analogWrite(ENB, geschwindigkeit);     
    digitalWrite(N1, LOW);                  
    digitalWrite (N2, HIGH);                 
    digitalWrite (N3, HIGH);              
    digitalWrite (N4, LOW);
  }
}

//  Entscheidungsroutinen:
//  ----------------------

void gehirn() {
  sensorikInfrarot();  
    int sensorInt = map(sensorWert, sensorMin, sensorMax, 0, 4);    
    switch (sensorInt) {
      case 0:
        geschwindigkeit = 255;
        bewegungFahrt(0);
        break;
      case 1:
        geschwindigkeit = 40;
        bewegungFahrt(1);
        break;
      case 2:
        geschwindigkeit = 20;
        bewegungFahrt(1);
        break;      
      case 3:
        geschwindigkeit = 0;
        bewegungFahrt(1);
        break; 
      default:
        break;  
    }
}
