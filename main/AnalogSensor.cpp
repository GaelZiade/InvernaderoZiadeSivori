#include "AnalogSensor.h"

void AnalogSensor::Config(int pin1,float x1,float y1,float x2,float y2){
pin=pin1;
x1_=x1;
y1_=y1;
x2_=x2;
y2_=y2;

}

float AnalogSensor::Leer(){
float input;
input = analogRead(pin);

return (((input - x1_) * (y2_ - x2_)) / (y1_ - x1_)) + x2_;

}