//include DHT_sensor_library
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

#define DHTTYPE DHT11  //DHT 11 (Version)

#define DHTPIN 11  //Sensor auf Digital Pin 11

DHT_Unified dht(DHTPIN, DHTTYPE);

uint32_t delayMS;

const int limitSwitchYPin = 7;  //X-Achse Limit Switch
const int limitSwitchXPin = 8;  //Y-Achse Limit Switch

const int motorLDirPin = 2;  //X-Achse Richtung
const int motorLStepPin = 3; //X-Achse Schritt
const int motorRDirPin = 4;  //Y-Achse Richtung
const int motorRStepPin = 5; //Y-Achse Schritt

const int motorDelaySlow = 1000;  //Geschwindigkeit Motor (höherer = langsamer)
const int motorDelayFast = 400;

const int stepsPerRevolution = 1600; //Anzahl der Schritte pro Umdrehung (abhängig vom Motor)
const int numberOfRevolutions = 1;  //Anzahl der gewünschten Umdrehungen

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


  //autoHome();

  //Clearboard();

  //Clearboard45();

  moveLinear(421 * -20, 421 * -40, motorDelayFast);
}

void loop() {
  

  delay(delayMS);
  sensors_event_t event;

  //Abrufen der Temperatur und ausgabe.
  dht.temperature().getEvent(&event);
    Serial.print(event.temperature);
    Serial.println(F("C°"));
  
  //Abrufen der Feuchtigkeits und ausgabe.
  dht.humidity().getEvent(&event);
    Serial.print(event.relative_humidity);
    Serial.println(F("%"));
  
  int totalSteps = stepsPerRevolution * numberOfRevolutions;

  for (int i = 0; i < totalSteps; i++) {
    //Schritt ausführen
    digitalWrite(motorLStepPin, HIGH);
    digitalWrite(motorRStepPin, HIGH);
    delayMicroseconds(500); //Geschwindigkeit
    digitalWrite(motorLStepPin, LOW);
    digitalWrite(motorRStepPin, LOW);
    delayMicroseconds(500); 
  }
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
    delayMicroseconds(motorDelaySlow);
    digitalWrite(motorRStepPin, HIGH);
    delayMicroseconds(motorDelaySlow);
    //
    digitalWrite(motorLStepPin, LOW);
    delayMicroseconds(motorDelaySlow);
    digitalWrite(motorRStepPin, LOW);
    delayMicroseconds(motorDelaySlow);
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

}

void moveY(int steps, bool direction, int motorDelayFast) {
  digitalWrite(motorLDirPin, direction);  //Richtung setzen
  digitalWrite(motorRDirPin, !direction); //Andere Achse in Gegenrichtung
  int stepsfory = abs(steps) / 2;
  for (int i = 0; i < stepsfory; i++) {
    digitalWrite(motorLStepPin, HIGH);
    digitalWrite(motorRStepPin, HIGH);
    delayMicroseconds(motorDelayFast);
    digitalWrite(motorLStepPin, LOW);
    digitalWrite(motorRStepPin, LOW);
    delayMicroseconds(motorDelayFast);
  }
}

void moveX(int steps, bool direction, int motorDelayFast) {
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

void moveDiagonal(int Steps, bool xDirection, bool yDirection, int motorDelayFast) {
  // Funktion, um gleichmäßig gleichzeitig in X- und Y-Richtung zu fahren
  //int maxSteps = max(xSteps, ySteps);  // Maximaler Schrittwert für die Synchronisierung

  for (int i = 0; i < Steps; i++) {
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
}

void Clearboard(){
  int stepsPerCm = 421;  //Anzahl der Schritte für 1 cm   1600/3.8   12.1 x pi =38 mm 3.8cm
  int stepsLong = 40 * stepsPerCm;  //Schritte für 40 cm
  int step1Cm = stepsPerCm;         //Schritte für 1 cm
  
  Serial.println("Starten von Clearboard");
 
  for (int i = 0; i < 20; i++) { //40 cm / 2 = 20 

    //40 cm nach oben fahren
    Serial.println("40 cm nach oben");
    moveY(stepsLong, LOW, 400);  //Richtung nach oben

    //1 cm nach links fahren
    Serial.println("1 cm nach links Nr." + String(i + 1));
    moveX(step1Cm, LOW, 800);    //Richtung nach links

    //40 cm nach unten fahren
    Serial.println("40 cm nach unten");
    moveY(stepsLong, HIGH, 400);   //Richtung nach unten

    //1 cm nach links fahren 
    Serial.println("1 cm nach links Nr." + String(i + 1));
    moveX(step1Cm, LOW, 800);    //Richtung nach links
  }

  //40 cm Zurück nach rechts 
  Serial.println("40 cm zurück nach rechts");
  moveX(stepsLong, HIGH, 400);  //Richtung nach rechts
  Serial.println("Clearboard Fertig!");
}

void Clearboard45(){
  int stepsPerCm = 421;  //Anzahl der Schritte für 1 cm   1600/3.8   12.1 x pi =38 mm 3.8cm
  int stepsLong = 40 * stepsPerCm;  //Schritte für 40 cm
  int step1Cm = stepsPerCm;         //Schritte für 1 cm
  int count = 0;

  for (int i = 0; i < 20; i++){
    Serial.println("Starten von Clearboard 45°");

    Serial.println("1 cm nach links Nr." + String(i + 1));
    moveX(step1Cm, LOW, 800);    //Richtung nach links
    count++;

    Serial.print("Bewege diagonal nach rechts oben");
    moveDiagonal(step1Cm * count, HIGH, LOW, 800);  //X nach rechts, Y nach oben

    Serial.println("1 cm nach oben Nr." + String(i + 1));
    moveY(step1Cm, LOW, 800);    //Richtung nach oben
    count++;

    Serial.print("Bewege diagonal nach links unten");
    moveDiagonal(step1Cm * count, LOW, HIGH, 800);  //X nach links, Y nach unten
  }

  for (int i = 0; i < 20; i++){
    Serial.println("1 cm nach oben Nr." + String(i + 1));
    moveY(step1Cm, LOW, 800);    //Richtung nach oben
    count--;
    
    Serial.print("Bewege diagonal nach rechts oben");
    moveDiagonal(step1Cm * count, HIGH, LOW, 800);  //X nach rechts, Y nach oben

    Serial.println("1 cm nach links Nr." + String(i + 1));
    moveX(step1Cm, LOW, 800);    //Richtung nach links
    count--;

    Serial.print("Bewege diagonal nach links unten");
    moveDiagonal(step1Cm * count, LOW, HIGH, 800);  //X nach links, Y nach unten
  }
}

void moveLinear(int Xdifference, int Ydifference, int motorDelayFast) {

  bool dirX;
  bool dirY;

  //Richtung für die move x & y funktionen geben
  if (Xdifference > 0) {
    Serial.println("Nach Rechts");
    dirX = HIGH;  //Links LOW, Rechts HIGH

  } else if (Xdifference < 0) {
    Serial.println("Nach Links");
    dirX = LOW;
  }

  if (Ydifference > 0) {
    Serial.println("Nach Oben");
    dirY = LOW;  //oben LOW, unten HIGH

  } else if (Ydifference < 0) {
    Serial.println("Nach Unten");
    dirY = HIGH;
  }

  //Maximale Anzahl Schritten (längere Achse)
  int steps = max(abs(Xdifference), abs(Ydifference));
  
  //Berechne Schritte pro Schleife
  //float xStep = (float)abs(Xdifference) / steps;
  //float yStep = (float)abs(Ydifference) / steps;

  int x = 0;
  int y = 0;
  int error = 0;

  int xStep = Xdifference > 0 ? 1 : -1;  //Schrittgröße für X
  int yStep = Ydifference > 0 ? 1 : -1;  //Schrittgröße für Y

  // Loop über die Gesamtzahl der Schritte
  for (int i = 0; i < steps; i++) {
    if (2 * (error + Ydifference) < Xdifference) {  //Wenn der Fehler kleiner ist, bewege X
      x += xStep;
      moveX(1, dirX, motorDelayFast);  //Bewege den X-Motor um 1 Schritt
      error += Ydifference;  //Aktualisiere den Fehlerwert
    }
    if (2 * (error + Xdifference) >= Ydifference) {  //Wenn der Fehler größer ist, bewege Y
      y += yStep;
      moveY(1, dirY, motorDelayFast);  //Bewege den Y-Motor um 1 Schritt
      error -= Xdifference;  //Aktualisiere den Fehlerwert
    }        
  }
}



