#include "LiquidCrystal_I2C.h"
#include <Wire.h>
#include "RTClib.h"
RTC_DS3231 rtc;
#include "dht.h"
dht DHT;
#define DHT11_PIN 8
#include <EEPROM.h>

// DELCARACION VARIABLES RTC
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

// CONFIGURACION LCD
LiquidCrystal_I2C lcd(0x27,20,4);

// CREACION BYTE "°"
byte Celcius[] = {
  B01110,
  B01010,
  B01110,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000
};

// DECLARACION VARIABLES DE CONTROL
int currenttemperatura;
int currenthumedad;

bool riegostate = false;

float tempset;
float humedadset;
int margentemp;
int margenhumedad;

boolean tempstate;
boolean humedadstate;

void setup()
{

  // INICIALIZACION DEL SERIAL
  Serial.begin(9600);

  // CONFIGURACION INICIAL LCD
  lcd.init();
  lcd.backlight();

  lcd.createChar(0, Celcius);

  lcd.print("Inicializando.");
  lcd.print(".");
  delay(10);
  lcd.print(".");
  delay(10);
  lcd.print(".");
  lcd.clear();

  // CHECKEO ESTADO DEL RTC
  if (!rtc.begin())
  {
    Serial.println(F("Couldn't find RTC"));
    while (1)
      ;
  }

  // COMPRUEBA SI ESTA PROGRAMADO EL ENCENDIDO DEL RIEGO
  bool isScheduledON(DateTime date)
  {
    int weekDay = date.dayOfTheWeek();
    float hours = date.hour() + date.minute() / 60.0;
    // De 09:30 a 11:30 y de 21:00 a 23:00
    bool hourCondition = (hours > 9.50 && hours < 11.50) || (hours > 21.00 && hours < 23.00);
    // Miercoles, Sabado o Domingo
    bool dayCondition = (weekDay == 3 || weekDay == 6 || weekDay == 0);
    if (hourCondition && dayCondition)
    {
      return true;
    }
    return false;
  }

  // SI SE APAGO ESTABLECE LOS VALORES ACTUALES DEL RELOJ
  if (rtc.lostPower())
  {
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  // SETEO DE LOS PARAMETROS DE CONTROL
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
  lcd.write(0);

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
  }

  if (currenttemperatura <= (tempset - margentemp))
  {
    tempstate = false;
  }

  // ENCENDIDO/APAGADO DE HUMEDAD CON MARGEN
  if (currenthumedad >= (humedadset + margenhumedad))
  {
    humedadstate = true;
  }

  if (currenthumedad <= (humedadset - margenhumedad))
  {
    humedadstate = false;
  }

  // ENCENDIDO Y APAGADO DEL RIEGO EN FUNCION DE LA HORA ESTABLECIDA
  if (riegostate == false && isScheduledON(now))
  {
    riegostate = true;
    Serial.print("Activado");
  }
  else if (riegostate == true && !isScheduledON(now))
  {
    riegostate = false;
    Serial.print("Desactivar");
  }

  delay(100);
}
