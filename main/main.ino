#include "LiquidCrystal.h"
#include "dht.h"
dht DHT;
#define DHT11_PIN 8

// Funcion de escritura desde el NodeMCU
// Almacenamiento d datos no volatiles
// RTC Arduino DS1307/3231 "Luis Llamas"
// Banda Proporcional

byte lcd0 = 6;
byte lcd1 = 5;
byte lcd2 = 4;
byte lcd3 = 13;
byte lcdrs = 7;
byte lcdenable = 9;

LiquidCrystal lcd(lcdrs, lcdenable, lcd0, lcd1, lcd2, lcd3);

byte celsius[8] = {
    B00111,
    B00101,
    B00111,
    B00000,
    B00000,
    B00000,
    B00000,
};

float currenttemperatura;
float currenthumedad;

float tempset;
float humedadset;

boolean overtemp;
boolean overhumedad;

void setup()
{

  Serial.begin(9600);

  lcd.createChar(0, celsius);
  lcd.begin(20, 4);
  lcd.home();

  tempset = 31;
  humedadset = 20;
}

void loop()
{

  DHT.read11(DHT11_PIN);
  currenthumedad = DHT.humidity;
  currenttemperatura = DHT.temperature;

  lcd.print("Temperatura:");
  lcd.print(String(DHT.temperature));
  lcd.print(" C");
  lcd.write(byte(0));

  lcd.setCursor(0, 2);
  Serial.println(currenthumedad);
  lcd.print("Humedad:");
  lcd.print(String(DHT.humidity));
  lcd.print("%");
  lcd.home();

  if (currenttemperatura > tempset)
  {
    overtemp = true;
    digitalWrite(13, HIGH);
  }

  else
  {
    overtemp = false;
    digitalWrite(13, LOW);
  }

  if (currenthumedad > humedadset)
  {
    overhumedad = true;
    Serial.println("AAAAAAH QUE HUMEDAD!!");
  }

  else
  {
    overtemp = false;
    Serial.println("AAAAAAH QUE HUMEDAN'T!!");
  }

  delay(100);
}