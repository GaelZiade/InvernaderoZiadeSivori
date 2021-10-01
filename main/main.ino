#include "LiquidCrystal.h"
#include "dht.h""

byte lcd0 = 6;
byte lcd1 = 5;
byte lcd2 = 4;
byte lcd3 = 13;
byte lcdrs = 7;
byte lcdenable = 9;

LiquidCrystal lcd (lcdrs, lcdenable, lcd0, lcd1, lcd2, lcd3);

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

void setup() {

  Serial.begin(9600);

  lcd.createChar(0, celsius);
  lcd.begin(20, 4);
  lcd.home();

  tempset = 31;
  humedadset = 20;

}


void loop() {

  //currenttemperatura = ????;

  lcd.print ("Temperatura: ");
  lcd.print (String(int(currenttemperatura)));
  lcd.print (" C");
  lcd.write(byte(0));
  lcd.home ();

  if (currenttemperatura > tempset) {
    overtemp = true;
    digitalWrite(13, HIGH);
  }

  else {
    overtemp = false;
    digitalWrite(13, LOW);
  }

    currenthumedad = analogRead(23);
    Serial.println(currenthumedad);

  delay(100);

}