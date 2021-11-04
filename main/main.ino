#include "LiquidCrystal.h"
#include <Wire.h>
#include "RTClib.h"
RTC_DS3231 rtc;
#include "dht.h"
dht DHT;
#define DHT11_PIN 8

// Funcion de escritura desde el NodeMCU
// Almacenamiento d datos no volatiles
// RTC Arduino DS1307/3231 "Luis Llamas"

// DELCARACION VARIABLES RTC
//  rtc.adjust(DateTime(2016, 1, 21, 3, 0, 0));
String daysOfTheWeek[7] = {"Domingo", "Lunes", "Martes", "Miercoles", "Jueves", "Viernes", "Sabado"};
String monthsNames[12] = {"Enero", "Febrero", "Marzo", "Abril", "Mayo", "Junio", "Julio", "Agosto", "Septiembre", "Octubre", "Noviembre", "Diciembre"};

// DECLARACION DE FUNCION PARA IMRESION DE FECHA
void printDate(DateTime date)
{
  Serial.print(date.year(), DEC);
  Serial.print('/');
  Serial.print(date.month(), DEC);
  Serial.print('/');
  Serial.print(date.day(), DEC);
  Serial.print(" (");
  Serial.print(daysOfTheWeek[date.dayOfTheWeek()]);
  Serial.print(") ");
  Serial.print(date.hour(), DEC);
  Serial.print(':');
  Serial.print(date.minute(), DEC);
  Serial.print(':');
  Serial.print(date.second(), DEC);
  Serial.println();
}

// DELCARACION PINES LCD
byte lcd0 = 6;
byte lcd1 = 5;
byte lcd2 = 4;
byte lcd3 = 13;
byte lcdrs = 7;
byte lcdenable = 9;

// CONFIGURACION LCD
LiquidCrystal lcd(lcdrs, lcdenable, lcd0, lcd1, lcd2, lcd3);

// CREACION BYTE "°"
byte celsius[8] = {
    B00111,
    B00101,
    B00111,
    B00000,
    B00000,
    B00000,
    B00000,
};

// DECLARACION VARIABLES DE CONTROL
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
  
  if (!rtc.begin())
  {
    Serial.println(F("Couldn't find RTC"));
    while (1)
      ;
  }

  if (rtc.lostPower())
  {
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

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

  DateTime now = rtc.now();
  printDate(now);

  // LEER VALORES DEL SENSOR
  DHT.read11(DHT11_PIN);
  currenthumedad = DHT.humidity;
  currenttemperatura = DHT.temperature;

  // DISPLAY TEMPERATURA
  lcd.print("Temperatura:");
  lcd.print(String(DHT.temperature));
  lcd.print(" C");
  lcd.write(byte(0));

  // DISPLAY HUMEDAD
  lcd.setCursor(0, 2);
  lcd.print("Humedad:");
  lcd.print(String(DHT.humidity));
  lcd.print("%");
  lcd.home();

  // ENCENDIDO/APAGADO DE TEMPERATURA CON MARGEN
  if (currenttemperatura >= (tempset + margentemp))
  {
    tempstate = true;
    // Serial.println("Temperatura = " + String(currenttemperatura) + ": HIGH");
  }

  if (currenttemperatura <= (tempset - margentemp))
  {
    tempstate = false;
    // Serial.println("Temperatura = " + String(currenttemperatura) + ": LOW");
  }

  // ENCENDIDO/APAGADO DE HUMEDAD CON MARGEN
  if (currenthumedad >= (humedadset + margenhumedad))
  {
    humedadstate = true;
    // Serial.println("Humedad = " + String(currenthumedad) + ": HIGH");
  }

  if (currenthumedad <= (humedadset - margenhumedad))
  {
    humedadstate = false;
    // Serial.println("Humedad = " + String(currenthumedad) + ": LOW");
  }

  delay(100);
}
