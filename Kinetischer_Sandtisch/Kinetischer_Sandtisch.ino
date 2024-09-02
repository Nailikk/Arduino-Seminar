//include DHT_sensor_library
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

#define DHTTYPE DHT11  // DHT 11 (Version)

#define DHTPIN 11  //Sensor auf Digital Pin 11

DHT_Unified dht(DHTPIN, DHTTYPE);

uint32_t delayMS;

const int limitSwitchYPin = 7;  // X-Achse Limit Switch
const int limitSwitchXPin = 8;  // Y-Achse Limit Switch

const int motorLDirPin = 2;  // X-Achse Richtung
const int motorLStepPin = 3; // X-Achse Schritt
const int motorRDirPin = 4;  // Y-Achse Richtung
const int motorRStepPin = 5; // Y-Achse Schritt

const int motorDelay = 1000;  // Geschwindigkeit Motor (höherer = langsamer)


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


  autoHome();

  clearboard();

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
  
}

void autoHome(){

  Serial.println("Starten von Auto-Home");

  //Richtung setzen Y-Achse
  digitalWrite(motorLDirPin, HIGH); 
  digitalWrite(motorRDirPin, LOW);

  //Homing Y-Achse
  Serial.println("Starten Y-Achse Home");
  while (digitalRead(limitSwitchYPin) == HIGH) {  // Läuft bis der Switch erreicht ist
    //
    digitalWrite(motorLStepPin, HIGH);
    delayMicroseconds(motorDelay);
    digitalWrite(motorRStepPin, HIGH);
    delayMicroseconds(motorDelay);
    //
    digitalWrite(motorLStepPin, LOW);
    delayMicroseconds(motorDelay);
    digitalWrite(motorRStepPin, LOW);
    delayMicroseconds(motorDelay);
  }
  Serial.println("Y-Achse Fertig!");

  //Homing X-Achse 
  Serial.println("Starten X-Achse Home");

  //Richtung setzen X-Achse
  digitalWrite(motorLDirPin, HIGH); 
  digitalWrite(motorRDirPin, HIGH);

  while (digitalRead(limitSwitchXPin) == HIGH) {  // Läuft bis der Switch erreicht ist
    //
    digitalWrite(motorLStepPin, HIGH);
    delayMicroseconds(motorDelay);
    digitalWrite(motorRStepPin, HIGH);
    delayMicroseconds(motorDelay);
    //
    digitalWrite(motorLStepPin, LOW);
    delayMicroseconds(motorDelay);
    digitalWrite(motorRStepPin, LOW);
    delayMicroseconds(motorDelay);
  }
  Serial.println("X-Achse Fertig!");

  Serial.println("Auto-Home Fertig!");

}

void clearboard(){
  
}