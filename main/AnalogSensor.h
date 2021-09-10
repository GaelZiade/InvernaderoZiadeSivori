#ifndef AnalogSensor_H
#define AnalogSensor_H

class AnalogSensor{
private:
int pin;
float x1_;
float y1_;
float x2_;
float y2_;

public:
float Leer();
void Config(int pin1, float x1, float y1, float x2, float y2);

};
#endif