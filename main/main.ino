#include "LiquidCrystal.h"
#include "AnalogSensor.h"

AnalogSensor LM335 = AnalogSensor();

void setup(){
  LM335.Config(A0, 558.558, 763.158, 0, 100);
  Serial.begin(9600);

}


void loop(){
  float temperatura;

  temperatura = LM335.Leer();

  Serial.println("--------------------------------");
  Serial.println("Temp: " + String(temperatura) + " *C");

  delay(500);
  
}