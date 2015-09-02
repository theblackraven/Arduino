/*
	Thermistor100k.cpp
*/

#include <Thermistor100k.h>

//Consrucotr

double Thermistor100k::get_temp(int pin)
{


   adc_value=analogRead(pin); //150901 Thermistor added
   for(int16_t i=1; i<table_size; i++) {
   if(table[i-1] <= adc_value && table[i] >= adc_value) {
      temp_actual = (i*temp_step_size + start_temp);
	  return  temp_actual;
	}
   }
}
   


	