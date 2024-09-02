//include DHT_sensor_library
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

#define DHTTYPE DHT11  // DHT 11 (Version)

#define DHTPIN 11  //Sensor auf Digital Pin 11

DHT_Unified dht(DHTPIN, DHTTYPE);

uint32_t delayMS;

const int limitSwitchXPin = 7;  // X-Achse Limit Switch
const int limitSwitchYPin = 8;  // Y-Achse Limit Switch

const int motorXDirPin = 2;  // X-Achse Richtung
const int motorXStepPin = 3; // X-Achse Schritt
const int motorYDirPin = 4;  // Y-Achse Richtung
const int motorYStepPin = 5; // Y-Achse Schritt

const int motorDelay = 1000;  // Geschwindigkeit Motor (höherer = langsamer)


void setup() {
  
  Serial.begin(9600);


  dht.begin();  //Gerät initialisieren
  sensor_t sensor;
  dht.temperature().getSensor(&sensor); //Temperatur

  dht.humidity().getSensor(&sensor); //Luftfeuchtichkeit

  delayMS = sensor.min_delay / 500; //Setzt abfrage auf 2sek (100 = 10sek, 1000 = 1sek)


  //Pins initialisieren
  pinMode(limitSwitchXPin, INPUT_PULLUP);
  pinMode(limitSwitchYPin, INPUT_PULLUP);
  
  pinMode(motorXDirPin, OUTPUT);
  pinMode(motorXStepPin, OUTPUT);
  pinMode(motorYDirPin, OUTPUT);
  pinMode(motorYStepPin, OUTPUT);


  autoHome();

}

void loop() {
  // put your main code here, to run repeatedly:

}

void autoHome(){


}