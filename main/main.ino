#include "LiquidCrystal.h"
#include "dht.h"
dht DHT;
#define DHT11_PIN 8

// Funcion de escritura desde el NodeMCU
// Almacenamiento d datos no volatiles
// RTC Arduino DS1307/3231 "Luis Llamas"
// Banda Proporcional

//DELCARACION PINES LCD
byte lcd0 = 6;
byte lcd1 = 5;
byte lcd2 = 4;
byte lcd3 = 13;
byte lcdrs = 7;
byte lcdenable = 9;

//CONFIGURACION LCD
LiquidCrystal lcd(lcdrs, lcdenable, lcd0, lcd1, lcd2, lcd3);

//CREACION BYTE "°"
byte celsius[8] = {
    B00111,
    B00101,
    B00111,
    B00000,
    B00000,
    B00000,
    B00000,
};

//DECLARACION VARIABLES DE CONTROL
int currenttemperatura;
int currenthumedad;

float tempset;
float humedadset;
int margentemp;
int margenhumedad;

boolean tempstate;
boolean humedadstate;

void setup()
{

  Serial.begin(9600);

  lcd.createChar(0, celsius);
  lcd.begin(20, 4);
  lcd.home();

  tempset = 25;
  humedadset = 60;
  margentemp = 5;
  margenhumedad = 5;

}

void loop()
{

  //LEER VALORES DEL SENSOR
  DHT.read11(DHT11_PIN);
  currenthumedad = DHT.humidity;
  currenttemperatura = DHT.temperature;

  //DISPLAY TEMPERATURA
  lcd.print("Temperatura:");
  lcd.print(String(DHT.temperature));
  lcd.print(" C");
  lcd.write(byte(0));

  //DISPLAY HUMEDAD
  lcd.setCursor(0, 2);
  lcd.print("Humedad:");
  lcd.print(String(DHT.humidity));
  lcd.print("%");
  lcd.home();

  //ENCENDIDO/APAGADO DE TEMPERATURA CON MARGEN
  if (currenttemperatura >= (tempset + margentemp)){
    tempstate = true;
    Serial.println("Temperatura = " + String(currenttemperatura) + ": HIGH");
  }

  if (currenttemperatura <= (tempset - margentemp)){
    tempstate = false;
    Serial.println("Temperatura = " + String(currenttemperatura) + ": LOW");
  }

  //ENCENDIDO/APAGADO DE HUMEDAD CON MARGEN
  if (currenthumedad >= (humedadset + margenhumedad)){
    humedadstate = true;
    Serial.println("Humedad = " + String(currenthumedad) + ": HIGH");
  }

  if (currenthumedad <= (humedadset - margenhumedad)){
    humedadstate = false;
    Serial.println("Humedad = " + String(currenthumedad) + ": LOW");
  }

  delay(100);
}