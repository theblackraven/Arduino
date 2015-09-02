/*
	Thermistor100k.cpp
*/

#include Thermistor100k.h

//Consrucotr
THERMO100K::THERMO100K(int pin)
{
	_pin = pin;
}
	{
 int adc_value=analogRead(pin); //150901 Thermistor added
   for(int16_t i=1; i<table_size; i++) {
   if(table[i-1] < adc_value && table[i] > adc_value) {
      temp_actual = (i*temp_step_size + start_temp) + ((adc_value-table[i-1])/(table[i]-adc_value)*temp_step_size);
	  return temp_actual
	}
   


	