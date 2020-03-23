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
unsigned long startTime;
float sensorWert;                            // Momentaner Sensor-Wert
float refSpannung = 4.96;                    // Korrektur für Sensor von 2A
const float sensorMin = 512;                 // Sensor-Wert bei freiem Feld
const float sensorMax = 2048;                // Gewünschter Maximalwert des Sensors

void setup() {
  // debugging output
  Serial.begin(9600);         /* DEV */
  
  // benötigt für die library
  myservo.attach(servoPin);
  myservo.write(sensorikPosition);

  pinMode(sensorPin, INPUT);
  pinMode(ENA, OUTPUT);
  pinMode(N1, OUTPUT);
  pinMode(N2, OUTPUT);
  pinMode(N3, OUTPUT);
  pinMode(N4, OUTPUT);
    
  pinMode(ENA, OUTPUT);

  // der Sensor ist sonst viel zu schnell
  sensorikVerzoegerung = 10;
  // nicht sicher ob es gebraucht wird
  gehirnVerzoegerung = 10;
}

void loop() {
  // put your main code here, to run repeatedly:
  gehirn();
  // hardcoded geschwindigkeitsanpassung
  // die Variable definiert, wie schnell vorwärts gefahren wird
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
    // boolean, welcher die Richtung des sensors definiert
    if (sensorikRichtung) {               // 
      sensorikPosition++;        
    }
    else {
      sensorikPosition--;
    }
    myservo.write(sensorikPosition);
    // wenn über 180 Grad oder weniger als 0 Grad kehre Richtung um.
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

void bewegungFahrt(bool richtungFahrt, int dauer,byte geschwindigkeit) {          // richtungFahrt: geradeaus = true; rückwärts = false, dauer in Millisekunden
  startTime=millis();
  analogWrite(ENA, geschwindigkeit);        /* DEV */
  analogWrite(ENB, geschwindigkeit);        /* DEV */ 
  //// Vorwärts fahren
  if (richtungFahrt) {               // 
    if (millis() - startTime > dauer){
      digitalWrite (N1, HIGH);
      digitalWrite (N2, LOW);
      digitalWrite (N3, HIGH);
      digitalWrite (N4, LOW);    
    }
    // Rückwärts fahren
  }
  else {
    if (millis() - startTime > dauer){
      digitalWrite (N1, LOW);
      digitalWrite (N2, HIGH);
      digitalWrite (N3, LOW);
      digitalWrite (N4, HIGH);
    }
  }
}

void bewegungKurve(bool richtungKurve, int dauer,byte geschwindigkeit) {            // richtungKurve: links = true, rechts = false; dauer in Millisekunden
  startTime=millis();
  analogWrite(ENA, geschwindigkeit);        /* DEV */
  analogWrite(ENB, geschwindigkeit);        /* DEV */ 
  // Kurve links
  if (richtungKurve) {               // 
    if (millis() - startTime > dauer){
      digitalWrite (N1, LOW);
      digitalWrite (N2, HIGH);
      digitalWrite (N3, HIGH);
      digitalWrite (N4, LOW);    
    }
    // Kurve rechts
  }
  else {
    if (millis() - startTime > dauer){
      digitalWrite (N1, HIGH);
      digitalWrite (N2, LOW);
      digitalWrite (N3, LOW);
      digitalWrite (N4, HIGH);  
    }
  }
}

void bewegungAntrieb(bool rad, bool drehrichtungAntrieb, byte geschwindigkeitAntrieb) {      // Subroutine - Grundlegender Antrieb eines Rads, rad: links = true, rechts = false; drehrichtungAntrieb: vorwärts = true, rückwärts = false; geschwindigkeitAntrieb in Prozent
  
}

void bewegungAnfahrt() {
  geschwindigkeit = 255;
  delay(20);
}

//  Entscheidungsroutinen:
//  ----------------------

void gehirn() {
  sensorikInfrarot();  
    int sensorInt = map(sensorWert, sensorMin, sensorMax, 0, 4);    
    switch (sensorInt) {
      case 0:
        geschwindigkeit = 80;
        break;
      case 1:
        geschwindigkeit = 40;
        break;
      case 2:
        geschwindigkeit = 20;
        break;      
      case 3:
        // stop
        geschwindigkeit = 0;
        break; 
      default:
        break;  
    }
}
