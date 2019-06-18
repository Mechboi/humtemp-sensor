/*
Programm für eine kleine Wetterstation mit Temperatur und Feuchtigkeitssensor
Autor:  Niels Nawrot, nani1013
E-Mail: nani1013@hs-karlsruhe.de
Datum:  25.05.2019

Verwendete Bauteile:
- ESP32 Dev-Module
- OLED 1306
- DHT11 Sensor
*/

//==============================
//Bibliotheken
//==============================
//Fuer den Sensor
#include <SimpleDHT.h>            //Version 1.0.2
//Fuer die Anzeige auf dem OLED
#include <Wire.h>
#include <Adafruit_GFX.h>         //Version 1.4.9
#include <Adafruit_SSD1306.h>     //Version 1.2.9

#define OLED_RESET 4
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

//==============================
//Funktionen
//==============================
int pinDHT11 = 2;
SimpleDHT11 dht11;

//setzt den Display zurück und legt die Textgröße fest, wird einmalig beim Start aufgerufen -> setup()
void displaySetup() {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.display();
  display.clearDisplay();           //reinigt den Bildschirm am Anfang
  display.setTextSize(4);           //Textgroesse 4 entspricht dem halbem Bildschirm
  display.setTextColor(INVERSE);    //Inverse des schwarzen Bildschirms -> weiss
}


void setup() {
  Serial.begin(9600); //legt BAUD fuer den seriellen Monitor fest
  displaySetup();
}

//S
void displayOutput(int temp, int hum) {
  Serial.println("===================================");  //Kommentar auf dem seriellen Monitor
  Serial.println("Display wird beschriftet...");

  //Display Ausgabe
  display.setCursor(0,0);
  //Ausgabe der Temperatur in Celsius in einer Zeile
  if(temp < 10) {
    display.print(" ");
    display.print(temp);
    display.println(" C");
  }
  else {
    display.print(temp);
    display.println(" C");
  }
  //Ausgabe der Feuchtigkeit in Prozent in einer Zeile
  display.print(hum);
  display.print(" %");
  display.drawCircle(62, 7, 7, INVERSE);  //Zwei Kreise als °-Zeichen
  display.drawCircle(62, 7, 6, INVERSE);
  display.display();
}

void loop() {
  Serial.println("===================================");
  Serial.println("Lese Daten des DHT11...");
  
  byte temperature = 0;
  byte humidity = 0;
  if (dht11.read(pinDHT11, &temperature, &humidity, NULL)) {
    Serial.print("Fehler beim Lesen des DHT11 Moduls");
    return;
  }

  //Anpassung der Temperatur und Luftfeuchtigkeit, falls verschobe
  int leveledTemp = (int)temperature;
  int leveledHum = (int)humidity;
  
  Serial.print("Lesen erfolgreich:\n");
  Serial.print("Gemessene  Temperatur:       "); Serial.print((int)temperature); Serial.println(" °C, "); 
  Serial.print("Gemessene  Luftfeuchtigkeit: "); Serial.print((int)humidity); Serial.println(" %");
  Serial.print("Angepasste Temperatur:       "); Serial.print((int)temperature); Serial.println(" °C, "); 
  Serial.print("Angepasste Luftfeuchtigkeit: "); Serial.print((int)humidity); Serial.println(" %");

  displayOutput(leveledTemp, leveledHum);
  
  // delay in ms -> 1000  bedeutet alle Sekunde
  delay(1000);
  display.clearDisplay();
  Serial.println("Display bereinigt");
}
