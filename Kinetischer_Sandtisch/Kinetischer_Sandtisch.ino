
//include DHT_sensor_library
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

//#include <cmath>

#define DHTTYPE DHT11                    //DHT 11 (Version)

#define DHTPIN 11                        //Sensor auf Digital Pin 11

DHT_Unified dht(DHTPIN, DHTTYPE);

uint32_t delayMS;

const int limitSwitchYPin = 7;          //X-Achse Limit Switch
const int limitSwitchXPin = 8;          //Y-Achse Limit Switch

const int motorLDirPin = 2;             //X-Achse Richtung
const int motorLStepPin = 3;            //X-Achse Schritt
const int motorRDirPin = 4;             //Y-Achse Richtung
const int motorRStepPin = 5;            //Y-Achse Schritt

const int motorDelaySlow = 1000;        //Geschwindigkeit Motor (höherer = langsamer)
const int motorDelayFast = 500;

const double faktor = 2; //+ (1.414213 * (9/31.7));         //für die Berechnung der Hypotenuse wurzel 2 (+ behebung differenz)

const int stepsPerCm = 421;             //Anzahl der Schritte für 1 cm   1600/3.8   12.1 x pi =38 mm 3.8cm
const int stepsLong = 30 * stepsPerCm;  //Schritte für 30 cm
const int step1Cm = stepsPerCm;         //Schritte für 1 cm
const int one_beam = 7.5;               //ein Segment
const int two_beam = 15;                //zwei Segmente

void setup() {
  
  Serial.begin(9600);

  dht.begin();  //Gerät initialisieren
  sensor_t sensor;
  dht.temperature().getSensor(&sensor); //Temperatur

  dht.humidity().getSensor(&sensor); //Luftfeuchtichkeit

  delayMS = sensor.min_delay / 200; //Setzt abfrage auf 2sek (100 = 10sek, 1000 = 1sek)


  //Pins initialisieren
  pinMode(limitSwitchYPin, INPUT_PULLUP);
  pinMode(limitSwitchXPin, INPUT_PULLUP);
  
  pinMode(motorLDirPin, OUTPUT);
  pinMode(motorLStepPin, OUTPUT);
  pinMode(motorRDirPin, OUTPUT);
  pinMode(motorRStepPin, OUTPUT);

  delay(1500);  //Damit keine Bewegung beim Anschalten ausgeführt wird

  //test_dimensions();

  

}

void loop() {

  autoHome();
  Clearboard45(); 
  
  TemperatureDigits();
  Serial.println("wait 5 min");
  delay(300000);  //10 Minuten = 600000 ms, 5 = 300000

  autoHome();
  //Clearboard();
  moveLinear(-31 * stepsPerCm, 0, motorDelayFast);
  drawSpiral(15*stepsPerCm, 15*stepsPerCm, step1Cm*0.2, 12);

  autoHome();
  moveLinear(-31 * stepsPerCm, 0, motorDelayFast);
  humidityGraph();

  autoHome();
  
  delay(300000);  //10 Minuten = 600000 ms, 5 = 300000

}

void test_dimensions() {
}

void autoHome(){

  Serial.println("Starten von Auto-Home");

  //Richtung setzen Y-Achse
  digitalWrite(motorLDirPin, HIGH); 
  digitalWrite(motorRDirPin, LOW);

  //Homing Y-Achse
  Serial.println("Starten Y-Achse Home");
  while (digitalRead(limitSwitchYPin) == HIGH) {  //Läuft bis der Switch erreicht ist
    //
    digitalWrite(motorLStepPin, HIGH);
    delayMicroseconds(motorDelayFast);
    digitalWrite(motorRStepPin, HIGH);
    delayMicroseconds(motorDelayFast);
    //
    digitalWrite(motorLStepPin, LOW);
    delayMicroseconds(motorDelayFast);
    digitalWrite(motorRStepPin, LOW);
    delayMicroseconds(motorDelayFast);
  }
  Serial.println("Y-Achse Fertig!");

  //Homing X-Achse 
  Serial.println("Starten X-Achse Home");

  //Richtung setzen X-Achse
  digitalWrite(motorLDirPin, HIGH); 
  digitalWrite(motorRDirPin, HIGH);

  while (digitalRead(limitSwitchXPin) == HIGH) {  //Läuft bis der Switch erreicht ist
    //
    digitalWrite(motorLStepPin, HIGH);
    delayMicroseconds(motorDelayFast);
    digitalWrite(motorRStepPin, HIGH);
    delayMicroseconds(motorDelayFast);
    //
    digitalWrite(motorLStepPin, LOW);
    delayMicroseconds(motorDelayFast);
    digitalWrite(motorRStepPin, LOW);
    delayMicroseconds(motorDelayFast);
  }
  Serial.println("X-Achse Fertig!");

  Serial.println("Auto-Home Fertig!");
  delay(1000);

}

void moveY(int steps, bool direction, int motorDelayFast) {
  //oben LOW, unten HIGH
  digitalWrite(motorLDirPin, direction);  //Richtung setzen
  digitalWrite(motorRDirPin, !direction); //Andere Achse in Gegenrichtung
  for (int i = 0; i < steps; i++) {
    digitalWrite(motorLStepPin, HIGH);
    digitalWrite(motorRStepPin, HIGH);
    delayMicroseconds(motorDelayFast);
    digitalWrite(motorLStepPin, LOW);
    digitalWrite(motorRStepPin, LOW);
    delayMicroseconds(motorDelayFast);
  }
}

void moveX(int steps, bool direction, int motorDelayFast) {
  //Links LOW, Rechts HIGH
  digitalWrite(motorLDirPin, direction);  //Richtung setzen
  digitalWrite(motorRDirPin, direction);  //Beide Motoren in dieselbe Richtung
  for (int i = 0; i < abs(steps); i++) {
    digitalWrite(motorLStepPin, HIGH);
    digitalWrite(motorRStepPin, HIGH);
    delayMicroseconds(motorDelayFast);
    digitalWrite(motorLStepPin, LOW);
    digitalWrite(motorRStepPin, LOW);
    delayMicroseconds(motorDelayFast);
  }
}

void moveDiagonal(int steps, bool xDirection, bool yDirection, int motorDelayFast) {
  //Funktion, um gleichmäßig gleichzeitig in X- und Y-Richtung zu fahren
  
  double stepsDiagonal = steps * faktor;

  if (xDirection == HIGH){                        //Rechts HIGH

    if (yDirection == LOW){                       //oben LOW
      //digitalWrite(motorLDirPin, HIGH);         //
      digitalWrite(motorRDirPin, HIGH);           //
      for (int i = 0; i < stepsDiagonal; i++) {
        digitalWrite(motorRStepPin, HIGH);
        delayMicroseconds(motorDelayFast);
        digitalWrite(motorRStepPin, LOW);
        delayMicroseconds(motorDelayFast);
      }
    } 
    else if (yDirection == HIGH) {                //unten HIGH
      digitalWrite(motorLDirPin, HIGH);           //
      //digitalWrite(motorRDirPin, HIGH);         //
      for (int i = 0; i < stepsDiagonal; i++) {
        digitalWrite(motorLStepPin, HIGH);
        delayMicroseconds(motorDelayFast);
        digitalWrite(motorLStepPin, LOW);
        delayMicroseconds(motorDelayFast);
      }
    }
  } else if (xDirection == LOW) {                 //Links LOW

    if (yDirection == LOW){                       //oben LOW
      digitalWrite(motorLDirPin, LOW);            //
      //digitalWrite(motorRDirPin, LOW);          //
      for (int i = 0; i < stepsDiagonal; i++) {
        digitalWrite(motorLStepPin, HIGH);
        delayMicroseconds(motorDelayFast);
        digitalWrite(motorLStepPin, LOW);
        delayMicroseconds(motorDelayFast);         
      }
    } 
    else if ((yDirection == HIGH)) {              //unten HIGH
      //digitalWrite(motorLDirPin, LOW);          //
      digitalWrite(motorRDirPin, LOW);            //
      for (int i = 0; i < stepsDiagonal; i++) {
        digitalWrite(motorRStepPin, HIGH);
        delayMicroseconds(motorDelayFast);
        digitalWrite(motorRStepPin, LOW);
        delayMicroseconds(motorDelayFast);
      }
    }
  }

  /*
  for (int i = 0; i < stepsDiagonal; i++) {
    
    
    //1 step in x 
    digitalWrite(motorLDirPin, xDirection);  //Links LOW, Rechts HIGH
    digitalWrite(motorRDirPin, xDirection);

    digitalWrite(motorLStepPin, HIGH);
    digitalWrite(motorRStepPin, HIGH);
    delayMicroseconds(motorDelayFast);
    digitalWrite(motorLStepPin, LOW);
    digitalWrite(motorRStepPin, LOW);
    delayMicroseconds(motorDelayFast);

    //1 step in y 
    digitalWrite(motorLDirPin, yDirection);  //oben LOW, unten HIGH
    digitalWrite(motorRDirPin, yDirection);

    digitalWrite(motorLStepPin, HIGH);
    digitalWrite(motorRStepPin, LOW); //!!!
    delayMicroseconds(motorDelayFast);
    digitalWrite(motorLStepPin, LOW);
    digitalWrite(motorRStepPin, LOW);
    delayMicroseconds(motorDelayFast);
  }
  */
}

void Clearboard(){
  
  Serial.println("Starten von Clearboard");
 
  for (int i = 0; i < 15; i++) { // 30 cm / 2 = 15 

    //40 cm nach oben fahren
    Serial.println("30 cm nach oben");
    moveY(stepsLong, LOW, 400);  //Richtung nach oben

    //1 cm nach links fahren
    Serial.println("1 cm nach links Nr." + String(i + 1));
    moveX(step1Cm, LOW, 800);    //Richtung nach links

    //40 cm nach unten fahren
    Serial.println("30 cm nach unten");
    moveY(stepsLong, HIGH, 400);   //Richtung nach unten

    //1 cm nach links fahren 
    Serial.println("1 cm nach links Nr." + String(i + 1));
    moveX(step1Cm, LOW, 800);    //Richtung nach links
  }

   
  moveY(stepsLong, LOW, 400);  //nacg oben
  moveY(stepsLong , HIGH, 400);  //nach unten

  moveLinear(stepsLong, 0, motorDelayFast);
  moveLinear(-stepsLong, 0, motorDelayFast);
}

void Clearboard45(){
  int count = 0;

  for (int i = 0; i < 15; i++){
    Serial.println("Starten von Clearboard 45°");

    Serial.println("1 cm nach links Nr." + String(i + 1));
    moveX(step1Cm, LOW, 800);                                  //Richtung nach links
    count++;

    Serial.print("Bewege diagonal nach rechts oben");
    moveDiagonal(step1Cm * count, HIGH, LOW, motorDelayFast);  //X nach rechts, Y nach oben

    Serial.println("1 cm nach oben Nr." + String(i + 1));
    moveY(step1Cm, LOW, 800);                                  //Richtung nach oben
    count++;

    Serial.print("Bewege diagonal nach links unten");
    moveDiagonal(step1Cm * count, LOW, HIGH, motorDelayFast);  //X nach links, Y nach unten
  }

  for (int i = 0; i < 15; i++){
    Serial.println("1 cm nach oben Nr." + String(i + 1));
    moveY(step1Cm, LOW, 800);                                  //Richtung nach oben
    count--;
    
    Serial.print("Bewege diagonal nach rechts oben");
    moveDiagonal(step1Cm * count, HIGH, LOW, motorDelayFast);  //X nach rechts, Y nach oben

    Serial.println("1 cm nach links Nr." + String(i + 1));
    moveX(step1Cm, LOW, 800);                                  //Richtung nach links
    count--;

    Serial.print("Bewege diagonal nach links unten");
    moveDiagonal(step1Cm * count, LOW, HIGH, motorDelayFast);  //X nach links, Y nach unten
  }
  //Für Startpunkt der Zahlen (unten links)
  moveY(stepsLong * 0.8, HIGH, motorDelayFast);

}

void moveLinear(int Xdifference, int Ydifference, int motorDelayFast) {
  //Lineare Bewegung auf 2 Achsen
  bool dirX;
  bool dirY;

  //Richtungen für die Bewegung festlegen
  if (Xdifference > 0) {
    Serial.println("Nach Rechts");
    dirX = HIGH;  //Links LOW, Rechts HIGH
  } else if (Xdifference <= 0) {
    Serial.println("Nach Links");
    dirX = LOW;
  }

  if (Ydifference > 0) {
    Serial.println("Nach Oben");
    dirY = LOW;  //Oben LOW, Unten HIGH
  } else if (Ydifference <= 0) {
    Serial.println("Nach Unten");
    dirY = HIGH;
  }

  //Sonderfall Rein vertikale Bewegung
  if (Xdifference == 0) {
    for (int i = 0; i < abs(Ydifference); i++) {
      moveY(1, dirY, motorDelayFast);
    }
    return;  //Beenden da vollständig 
  }

  //Sonderfall Rein horizontale Bewegung
  if (Ydifference == 0) {
    for (int i = 0; i < abs(Xdifference); i++) {
      moveX(1, dirX, motorDelayFast);
    }
    return;  //Beenden da vollständig
  }

  //Maximale Anzahl Schritte (längere Achse) für diagonale Bewegung
  int steps = max(abs(Xdifference), abs(Ydifference));
  int error = 0;
  
  if (abs(Xdifference) <= abs(Ydifference)){

    for (int i = 0; i < steps; i++) {

      if (2 * abs(error) < abs(Ydifference)) {  //Y wird häufiger bewegt
        moveY(1, dirY, motorDelayFast);
        error += abs(Xdifference);
      }
      if (2 * abs(error) >= abs(Xdifference)) {  //X wird häufiger bewegt
        moveX(1, dirX, motorDelayFast);
        error -= abs(Ydifference);
      }
    }
  } else {

    for (int i = 0; i < steps; i++) {

      if (2 * abs(error) < abs(Xdifference)) {  
        moveX(1, dirX, motorDelayFast);
        error += abs(Ydifference);
      }
      if (2 * abs(error) >= abs(Ydifference)) { 
        moveY(1, dirY, motorDelayFast);
        error -= abs(Xdifference);
      }
    }
  }


}

void moveLinearNEU(int Xdifference, int Ydifference, int motorDelayFast) {
  //Lineare Bewegung auf 2 Achsen
  bool dirX;
  bool dirY;

  //Richtungen für die Bewegung festlegen
  if (Xdifference > 0) {
    Serial.println("Nach Rechts");
    dirX = HIGH;  //Links LOW, Rechts HIGH
  } else if (Xdifference <= 0) {
    Serial.println("Nach Links");
    dirX = LOW;
  }

  if (Ydifference > 0) {
    Serial.println("Nach Oben");
    dirY = LOW;  //Oben LOW, Unten HIGH
  } else if (Ydifference <= 0) {
    Serial.println("Nach Unten");
    dirY = HIGH;
  }

  //Maximale Anzahl Schritte (längere Achse) für diagonale Bewegung
  int stepsMin = min(abs(Xdifference), abs(Ydifference));
  int stepsMax = max(abs(Xdifference), abs(Ydifference));
  int Abweichung = 0;
  int xStep = Xdifference > 0 ? 1 : -1;
  int yStep = Ydifference > 0 ? 1 : -1;
  int minRichtung;

  if (Xdifference < Ydifference){
    int minRichtung = 1; //minrichtung x
  } else{
    int minRichtung = 0; //minrichtung y
  }

  //Sonderfall Rein vertikale Bewegung
  if (Xdifference == 0) {
    for (int i = 0; i < abs(Ydifference); i++) {
      moveY(1, dirY, motorDelayFast);
    }
    return;  //Beenden da vollständig 
  }

  //Sonderfall Rein horizontale Bewegung
  if (Ydifference == 0) {
    for (int i = 0; i < abs(Xdifference); i++) {
      moveX(1, dirX, motorDelayFast);
    }
    return;  //Beenden da vollständig
  }
  int stepsMinRemaining = stepsMin;
  for (int i = 0; i < stepsMin; i++) {
    
    float ratio = stepsMax / stepsMinRemaining;

    if(minRichtung == 1){
      moveX(1, dirX, motorDelayFast);
      moveY((int)ratio, dirY, motorDelayFast);
      stepsMinRemaining -= 1;
      stepsMax -= (int)ratio;
    } else{
      moveY(1, dirY, motorDelayFast);
      moveX((int)ratio, dirX, motorDelayFast);
      stepsMinRemaining -= 1;
      stepsMax -= (int)ratio;

    }
  }
}

void TemperatureDigits() {

  sensors_event_t event;
  dht.temperature().getEvent(&event);
  Serial.print(event.temperature);
  Serial.println(F("C°"));

  //Ausgabe der Temperatur auf 2 Stellen
  displayTemperature((int)event.temperature);

}

void displayTemperature(int temperature) {

  int tens = temperature / 10;      //Zehnerstelle
  int ones = temperature % 10;      //Einerstelle

  int decimal = (int)(temperature * 10) % 10;  //Erste Dezimalstelle

  drawDigit(tens, 1);               //Stelle 1 für Zehner

  drawDigit(ones, 2);               //Stelle 2 für Einer

  //drawDigit(decimal, 3);             // Stelle 3 für die Dezimalstelle
}

void decimalPoint(int motorDelayFast){
    
    //Rechts 1 cm
    moveLinear(1 * stepsPerCm, 0, motorDelayFast);

    //Hoch 2 cm
    moveLinear(0, 2 * stepsPerCm, motorDelayFast);
    
    //Links 1 cm 
    moveLinear(-2 * stepsPerCm, 0, motorDelayFast);    
    
    //Runter 2 cm
    moveLinear(0, -2 * stepsPerCm, motorDelayFast);

    //Rechts 1 cm
    moveLinear(1 * stepsPerCm, 0, motorDelayFast);
}

void drawDigit(int digit, int digit_position) {
  // Je nach Ziffer die entsprechende Funktion aufrufen
  switch (digit) {
    case 0:
      drawZero(digit_position);
      break;
    case 1:
      drawOne(digit_position);
      break;
    case 2:
      drawTwo(digit_position);
      break;
    case 3:
      drawThree(digit_position);
      break;
    case 4:
      drawFour(digit_position);
      break;
    case 5:
      drawFive(digit_position);
      break;
    case 6:
      drawSix(digit_position);
      break;
    case 7:
      drawSeven(digit_position);
      break;
    case 8:
      drawEight(digit_position);
      break;
    case 9:
      drawNine(digit_position);
      break;
  }
}

void drawZero(int digit_position) {

  Serial.println("0");

  if (digit_position == 1) {
    Serial.println("Nr. 1 (Zehnerstelle)");
    //moveX(one_beam * stepsPerCm, HIGH, motorDelayFast);
    moveLinear(6 * stepsPerCm, 0.5 * one_beam * stepsPerCm, motorDelayFast);
  } 
  else if(digit_position == 2) {
    Serial.println("Nr. 2 (Einerstelle)");
    moveLinear(2.5 * stepsPerCm, -2.5 * stepsPerCm, motorDelayFast);
    //moveX(one_beam * stepsPerCm, HIGH, motorDelayFast);
    moveLinear(2.5 * stepsPerCm, 2.5 * stepsPerCm, motorDelayFast);
  } 
  else if (digit_position == 3) {
    Serial.println("Nr. 3 (Dezimalstelle)");
    //
  }
    //Rechts one_beam
    moveLinear(one_beam * stepsPerCm, 0, motorDelayFast);

    //Hoch two_beam
    moveLinear(0, two_beam * stepsPerCm, motorDelayFast);
    
    //Links one_beam zurück
    moveLinear(-one_beam * stepsPerCm, 0, motorDelayFast);    
    
    //Runter two_beam
    moveLinear(0, -two_beam * stepsPerCm, motorDelayFast);
    //für Nr.2
    //Rechts one_beam
    moveLinear(one_beam * stepsPerCm, 0, motorDelayFast);
}

void drawOne(int digit_position) {

  if (digit_position == 1) {
    Serial.println("Nr. 1 (Zehnerstelle)");
    moveX(one_beam * stepsPerCm, HIGH, motorDelayFast);
    moveLinear(6 * stepsPerCm, 0.5 * one_beam * stepsPerCm, motorDelayFast);
  } 
  else if(digit_position == 2) {
    Serial.println("Nr. 2 (Einerstelle)");
    moveLinear(2.5 * stepsPerCm, -2.5 * stepsPerCm, motorDelayFast);
    moveX(one_beam * stepsPerCm, HIGH, motorDelayFast);
    moveLinear(2.5 * stepsPerCm, 2.5 * stepsPerCm, motorDelayFast);
  }
  else if (digit_position == 3) {
    Serial.println("Nr. 3 (Dezimalstelle)");
    //
  }
    //Hoch two_beam
    moveLinear(0, two_beam * stepsPerCm, motorDelayFast);

    //Links Runter one_beam
    moveLinear(-one_beam * stepsPerCm, -one_beam * stepsPerCm, motorDelayFast);
    //für Nr.2
    //Rechts Hoch one_beam 
    moveLinear(one_beam * stepsPerCm, one_beam * stepsPerCm, motorDelayFast);

    //Runter two_beam 
    moveLinear(0, -two_beam * stepsPerCm, motorDelayFast);
}

void drawTwo(int digit_position) {
    
  if (digit_position == 1) {
    Serial.println("Nr. 1 (Zehnerstelle)");
    moveX(one_beam * stepsPerCm, HIGH, motorDelayFast);
    moveLinear(6 * stepsPerCm, 0.5 * one_beam * stepsPerCm, motorDelayFast);
  } 
  else if(digit_position == 2) {
    Serial.println("Nr. 2 (Einerstelle)");
    moveLinear(2.5 * stepsPerCm, -2.5 * stepsPerCm, motorDelayFast);
    //moveX(one_beam * stepsPerCm, HIGH, motorDelayFast);
    moveLinear(2.5 * stepsPerCm, 2.5 * stepsPerCm, motorDelayFast);
  }
  else if (digit_position == 3) {
    Serial.println("Nr. 3 (Dezimalstelle)");
    //
  }
    //Links one_beam
    moveLinear(-one_beam * stepsPerCm, 0, motorDelayFast);

    //Hoch one_beam
    moveLinear(0, one_beam * stepsPerCm, motorDelayFast);
    
    //Rechts one_beam
    moveLinear(one_beam * stepsPerCm, 0, motorDelayFast);
    
    //Hoch one_beam
    moveLinear(0, one_beam * stepsPerCm, motorDelayFast);
    
    //Links one_beam
    moveLinear(-one_beam * stepsPerCm, 0, motorDelayFast);
    //für Nr.2
    //Rechts one_beam
    moveLinear(one_beam * stepsPerCm, 0, motorDelayFast);

    //Runter one_beam
    moveLinear(0, -one_beam * stepsPerCm, motorDelayFast);
    
    //Links one_beam
    moveLinear(-one_beam * stepsPerCm, 0, motorDelayFast);
    
    //Runter one_beam
    moveLinear(0, -one_beam * stepsPerCm, motorDelayFast);
    
    //Rechts one_beam
    moveLinear(one_beam * stepsPerCm, 0, motorDelayFast);

}

void drawThree(int digit_position) {

  if (digit_position == 1) {
    Serial.println("Nr. 1 (Zehnerstelle)");
    //moveX(one_beam * stepsPerCm, HIGH, motorDelayFast);
    moveLinear(6 * stepsPerCm, 0.5 * one_beam * stepsPerCm, motorDelayFast);
  } 
  else if(digit_position == 2) {
    Serial.println("Nr. 2 (Einerstelle)");
    moveLinear(2.5 * stepsPerCm, -2.5 * stepsPerCm, motorDelayFast);
    //moveX(one_beam * stepsPerCm, HIGH, motorDelayFast);
    moveLinear(2.5 * stepsPerCm, 2.5 * stepsPerCm, motorDelayFast);
  }
  else if (digit_position == 3) {
    Serial.println("Nr. 3 (Dezimalstelle)");
    //
  }
    //Rechts one_beam
    moveLinear(one_beam * stepsPerCm, 0, motorDelayFast);
    
    //Hoch one_beam
    moveLinear(0, one_beam * stepsPerCm, motorDelayFast);
    
    //Links one_beam
    moveLinear(-one_beam * stepsPerCm, 0, motorDelayFast);

    //Rechts one_beam
    moveLinear(one_beam * stepsPerCm, 0, motorDelayFast);

    //Hoch one_beam
    moveLinear(0, one_beam * stepsPerCm, motorDelayFast);
    
    //Links one_beam
    moveLinear(-one_beam * stepsPerCm, 0, motorDelayFast);
    //für Nr.2
    //Rechts one_beam
    moveLinear(one_beam * stepsPerCm, 0, motorDelayFast);

    //Runter two_beam
    moveLinear(0, -two_beam * stepsPerCm, motorDelayFast);
}

void drawFour(int digit_position) {

  if (digit_position == 1) {
    Serial.println("Nr. 1 (Zehnerstelle)");
    moveX(one_beam * stepsPerCm, HIGH, motorDelayFast);
    moveLinear(6 * stepsPerCm, 0.5 * one_beam * stepsPerCm, motorDelayFast);
  } 
  else if(digit_position == 2) {
    Serial.println("Nr. 2 (Einerstelle)");
    moveLinear(2.5 * stepsPerCm, -2.5 * stepsPerCm, motorDelayFast);
    //moveX(one_beam * stepsPerCm, HIGH, motorDelayFast);
    moveLinear(2.5 * stepsPerCm, 2.5 * stepsPerCm, motorDelayFast);
  }
  else if (digit_position == 3) {
    Serial.println("Nr. 3 (Dezimalstelle)");
    //
  }
    //Hoch two_beam
    moveLinear(0, two_beam * stepsPerCm, motorDelayFast);
    
    //Runter one_beam
    moveLinear(0, -one_beam * stepsPerCm, motorDelayFast);

    //Links one_beam
    moveLinear(-one_beam * stepsPerCm, 0, motorDelayFast);
    
    //Hoch one_beam
    moveLinear(0, one_beam * stepsPerCm, motorDelayFast);
    //für Nr.2
    //Runter one_beam
    moveLinear(0, -one_beam * stepsPerCm, motorDelayFast);

    //Rechts one_beam
    moveLinear(one_beam * stepsPerCm, 0, motorDelayFast);

    //Runter one_beam
    moveLinear(0, -one_beam * stepsPerCm, motorDelayFast);
}

void drawFive(int digit_position) {
  
  if (digit_position == 1) {
    Serial.println("Nr. 1 (Zehnerstelle)");
    //moveX(one_beam * stepsPerCm, HIGH, motorDelayFast);
    moveLinear(6 * stepsPerCm, 0.5 * one_beam * stepsPerCm, motorDelayFast);
  } 
  else if(digit_position == 2) {
    Serial.println("Nr. 2 (Einerstelle)");
    moveLinear(2.5 * stepsPerCm, -2.5 * stepsPerCm, motorDelayFast);
    //moveX(one_beam * stepsPerCm, HIGH, motorDelayFast);
    moveLinear(2.5 * stepsPerCm, 2.5 * stepsPerCm, motorDelayFast);
  }
  else if (digit_position == 3) {
    Serial.println("Nr. 3 (Dezimalstelle)");
    //
  }
    //Rechts one_beam
    moveLinear(one_beam * stepsPerCm, 0, motorDelayFast);

    //Hoch one_beam
    moveLinear(0, one_beam * stepsPerCm, motorDelayFast);
    
    //Links one_beam
    moveLinear(-one_beam * stepsPerCm, 0, motorDelayFast);
    
    //Hoch one_beam
    moveLinear(0, one_beam * stepsPerCm, motorDelayFast);
    
    //Rechts one_beam
    moveLinear(one_beam * stepsPerCm, 0, motorDelayFast);
    //für Nr.2
    //Links one_beam
    moveLinear(-one_beam * stepsPerCm, 0, motorDelayFast);

    //Runter one_beam
    moveLinear(0, -one_beam * stepsPerCm, motorDelayFast);

    //Rechts one_beam
    moveLinear(one_beam * stepsPerCm, 0, motorDelayFast);

    //Runter one_beam
    moveLinear(0, -one_beam * stepsPerCm, motorDelayFast);
}

void drawSix(int digit_position) {

  if (digit_position == 1) {
    Serial.println("Nr. 1 (Zehnerstelle)");
    //moveX(one_beam * stepsPerCm, HIGH, motorDelayFast);
    moveLinear(6 * stepsPerCm, 0.5 * one_beam * stepsPerCm, motorDelayFast);
  } 
  else if(digit_position == 2) {
    Serial.println("Nr. 2 (Einerstelle)");
    moveLinear(5 * stepsPerCm, -5 * stepsPerCm, motorDelayFast);
    //moveX(one_beam * stepsPerCm, HIGH, motorDelayFast);
    moveLinear(5 * stepsPerCm, 5 * stepsPerCm, motorDelayFast);
  }
  else if (digit_position == 3) {
    Serial.println("Nr. 3 (Dezimalstelle)");
    //
  }
    //Rechts one_beam
    moveLinear(one_beam * stepsPerCm, 0, motorDelayFast);
    
    //Hoch one_beam
    moveLinear(0, one_beam * stepsPerCm, motorDelayFast);
    
    //Links one_beam
    moveLinear(-one_beam * stepsPerCm, 0, motorDelayFast);
    
    //Hoch one_beam
    moveLinear(0, one_beam * stepsPerCm, motorDelayFast);
    
    //Rechts one_beam
    moveLinear(one_beam * stepsPerCm, 0, motorDelayFast);

    //Links one_beam
    moveLinear(-one_beam * stepsPerCm, 0, motorDelayFast);

    //Runter two_beam
    moveLinear(0, -two_beam * stepsPerCm, motorDelayFast);
    //für Nr.2
    //Rechts one_beam
    moveLinear(one_beam * stepsPerCm, 0, motorDelayFast);
}

void drawSeven(int digit_position) {

  if (digit_position == 1) {
    Serial.println("Nr. 1 (Zehnerstelle)");
    moveX(one_beam * stepsPerCm, HIGH, motorDelayFast);
    moveLinear(6 * stepsPerCm, 0.5 * one_beam * stepsPerCm, motorDelayFast);
  } 
  else if(digit_position == 2) {
    Serial.println("Nr. 2 (Einerstelle)");
    moveLinear(2.5 * stepsPerCm, -2.5 * stepsPerCm, motorDelayFast);
    //moveX(one_beam * stepsPerCm, HIGH, motorDelayFast);
    moveLinear(2.5 * stepsPerCm, 2.5 * stepsPerCm, motorDelayFast);
  }
  else if (digit_position == 3) {
    Serial.println("Nr. 3 (Dezimalstelle)");
    //
  }
    //Hoch two_beam
    moveLinear(0, two_beam * stepsPerCm, motorDelayFast);
    
    //Links one_beam
    moveLinear(-one_beam * stepsPerCm, 0, motorDelayFast);
    //für Nr.2
    //Rechts one_beam
    moveLinear(one_beam * stepsPerCm, 0, motorDelayFast);

    //Runter two_beam
    moveLinear(0, -two_beam * stepsPerCm, motorDelayFast);   
}

void drawEight(int digit_position) {

  if (digit_position == 1) {
    Serial.println("Nr. 1 (Zehnerstelle)");
    //moveX(one_beam * stepsPerCm, HIGH, motorDelayFast);
    moveLinear(6 * stepsPerCm, 0.5 * one_beam * stepsPerCm, motorDelayFast);
  } 
  else if(digit_position == 2) {
    Serial.println("Nr. 2 (Einerstelle)");
    moveLinear(2.5 * stepsPerCm, -2.5 * stepsPerCm, motorDelayFast);
    //moveX(one_beam * stepsPerCm, HIGH, motorDelayFast);
    moveLinear(2.5 * stepsPerCm, 2.5 * stepsPerCm, motorDelayFast);
  }
  else if (digit_position == 3) {
    Serial.println("Nr. 3 (Dezimalstelle)");
    //
  }
    //Hoch two_beam
    moveLinear(0, two_beam * stepsPerCm, motorDelayFast);
    
    //Rechts one_beam
    moveLinear(one_beam * stepsPerCm, 0, motorDelayFast);
    
    //Runter one_beam
    moveLinear(0, -one_beam * stepsPerCm, motorDelayFast);
    
    //Links one_beam
    moveLinear(-one_beam * stepsPerCm, 0, motorDelayFast);
    
    //Runter one_beam
    moveLinear(0, -one_beam * stepsPerCm, motorDelayFast);
    
    //Rechts one_beam
    moveLinear(one_beam * stepsPerCm, 0, motorDelayFast);
    
    //Hoch one_beam
    moveLinear(0, one_beam * stepsPerCm, motorDelayFast);
    //für Nr.2
    //Runter one_beam
    moveLinear(0, -one_beam * stepsPerCm, motorDelayFast);
}

void drawNine(int digit_position) {

  if (digit_position == 1) {
    Serial.println("Nr. 1 (Zehnerstelle)");
    //moveX(one_beam * stepsPerCm, HIGH, motorDelayFast);
    moveLinear(6 * stepsPerCm, 0.5 * one_beam * stepsPerCm, motorDelayFast);
  } 
  else if(digit_position == 2) {
    Serial.println("Nr. 2 (Einerstelle)");
    moveLinear(2.5 * stepsPerCm, -2.5 * stepsPerCm, motorDelayFast);
    //moveX(one_beam * stepsPerCm, HIGH, motorDelayFast);
    moveLinear(2.5 * stepsPerCm, 2.5 * stepsPerCm, motorDelayFast);
  }
  else if (digit_position == 3) {
    Serial.println("Nr. 3 (Dezimalstelle)");
    //
  }
    //Rechts one_beam
    moveLinear(one_beam * stepsPerCm, 0, motorDelayFast);

    //Hoch one_beam
    moveLinear(0, one_beam * stepsPerCm, motorDelayFast);
    
    //Links one_beam
    moveLinear(-one_beam * stepsPerCm, 0, motorDelayFast);
    
    //Hoch one_beam
    moveLinear(0, one_beam * stepsPerCm, motorDelayFast);
    
    //Rechts one_beam
    moveLinear(one_beam * stepsPerCm, 0, motorDelayFast);
    
    //Runter one_beam
    moveLinear(0, -one_beam * stepsPerCm, motorDelayFast);
    //für Nr.2
    //Runter one_beam
    moveLinear(0, -one_beam * stepsPerCm, motorDelayFast);
}

void humidityGraph(){

  float humidity;
  float prevHumidity = humidity;

  sensors_event_t event;
  dht.humidity().getEvent(&event);
  Serial.print((int)event.relative_humidity);
  Serial.println(F("%"));
  Serial.println(event.relative_humidity / 100);

  prevHumidity = event.relative_humidity / 100;

  moveLinear(0, (event.relative_humidity / 100) * stepsLong, motorDelayFast);

  for (int i = 1; i < 10; i++) {

    //Luftfeuchtigkeit auslesen
    sensors_event_t event;
    dht.humidity().getEvent(&event);
    Serial.print((int)event.relative_humidity);
    Serial.println(F("%"));

    humidity = event.relative_humidity / 100;
    //humidity = random(28, 90) / 100.0; //zum Testen

    Serial.println(humidity);

    //Differenz berechnen
    float yChange = (humidity - prevHumidity); //Differenz als Höhe
    Serial.print("Diff. ");
    Serial.println(yChange);
    Serial.print("Diff. Ganz ");
    Serial.println(yChange * stepsLong);
    delay(5000);

    //Graph bewegen
    moveGraph(yChange); 
    prevHumidity = humidity;  //Speichern von Luftfeuchtigkeit für den nächsten Vergleich

    Serial.print("Nr. ");
    Serial.println(i);
    Serial.println();

    delay(300000); //5 Minuten warten
    Serial.print("5 Minuten Pause");
  }
}

void moveGraph(float yChange) {
  int stepsDiff = yChange * stepsLong;
  Serial.print("Diff. Steps ");
  Serial.println(stepsDiff);
  moveLinear(3 * stepsPerCm, stepsDiff, motorDelayFast);
}

void drawSpiral(float x_start, float y_start, float a, int turns) {

  float theta = 0;
  //Parameter
  //float theta_increment = 0.1; //Inkrement für den Winkel
  float theta_increment = calculateThetaIncrement(theta); //Wächst mit theta, von min 0.01 auf max 0.2

  float max_theta = turns * 2 * M_PI; //Maximalwinkel = Umdrehungen*2*PI

  //Startposition
  double x0 = x_start;
  double y0 = y_start;
  double x1 = x_start;
  double y1 = y_start;

  //Startposition Motoren
  moveLinear(x0, y0, motorDelayFast); 

  //Drucken der Spirale
  for (float theta = 14; theta <= max_theta; theta += theta_increment) {

    theta_increment = calculateThetaIncrement(theta); //Aktualisieren des Werts in jedem Schritt

    double r = a * theta; //Radius

    x1 = x_start + r * cos(theta); //x-Koordinate Zielpunkt       
    y1 = y_start + r * sin(theta); //y-Koordinate Zielpunkt 
     
    //Code zum Steuern der Motoren   
    moveLinear(x1-x0, y1-y0, motorDelayFast);

    x0 = x1;
    y0 = y1;
  }
}

float calculateThetaIncrement(float theta) {

  //Start bei 0.25 und reduziert sich, ohne unter 0.07 zu fallen.
  return max(0.25f - theta * 0.001f, 0.07f); 

}