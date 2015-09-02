/*
	Thermistor100k.h
*/
#ifndef Thermistor100k_H
#define Thermistor100k_H
#include <Arduino.h>

#define table_size 100

class Thermistor100k {
	public:
	double get_temp(int pin);
	
	
	private:
	double start_temp = 0; // at what temp value does the table start
	double temp_step_size = 1; // whats the step size from one value to the next
	double temp_actual;
	int _pin;
	int adc_value;
	int table[table_size] = {
	235, 244,  254,  264,  275,  285,  296,  306,  317,  328,  339,  351,  362,  373,  385,  396,  408,  420,  431,  443,  454,  466,  478,  489,  501,  512,  523,  535,  546,  557,  568,  579,  589,  600,  610,  620,  630,  640,  650,  660,  669,  678,  688,  696,  705,  714,  722,  730,  738,  746,  754,  761,  768,  775,  782,  789,  796,  802,  808,  814,  820,  826,  831,  837,  842,  847,  852,  857,  862,  866,  871,  875,  879,  883,  887,  891,  895,  898,  902,  905,  909,  912,  915,  918,  921,  924,  926,  929,  932,  934,  937,  939,  941,  943,  946,  948,  950,  952,  954,  955
	};
};

#endif

	