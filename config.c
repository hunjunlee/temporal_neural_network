#ifndef __CONFIG_H__
#define __CONFIG_H__

#include <stdbool.h>

//(Tr in msec)
const float layer1_rise_time = 55;
//Tr and Tf ratio
const float layer1_rho = 4;
//threshold of neuron voltage
const float layer1_threshold = 40;
//attenuation value for neuron model2
const float layer1_attenuation = 0.0;


//weight maximum(number of synapses)
const int layer1_weight_max = 8;
//maximum time window
const float image_pixel_max = 255;
const float layer1_time_max = 8;
const int training_size = 60000;
const int test_size = 100;
const int n_rows = 28;
const int n_cols = 28;

const int layer1_column_count = 144;
const int layer1_neuron_count = 24;
const int layer1_synapse_count = 25;

const bool ffi_enable = true;
const bool li_enable = false;

float lateral_t;
float lateral_k;
int f_t_list[20];
int f_k_list[26];
int f_t_dat[144] = {23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 22, 23, 23, 23, 22, 20, 19, 20, 24, 24, 11, 21, 21, 22, 23, 23, 21, 10, 24, 24, 24, 24, 24, 23, 21, 23, 23, 22, 17, 10, 24, 24, 0, 24, 24, 21, 21, 23, 23, 21, 4, 3, 24, 24, 22, 24, 24, 24, 23, 23, 23, 22, 12, 24, 24, 24, 24, 24, 24, 24, 23, 23, 23, 23, 22, 24, 24, 24, 24, 24, 24, 24, 23, 23, 23, 23, 23, 24, 24, 24, 24, 24, 24, 24, 22, 23, 23, 23, 23, 24, 24, 1, 24, 0, 24, 17, 23, 23, 23, 23, 23, 24, 24, 24, 24, 24, 24, 21, 23, 23, 23, 21, 20, 23, 24, 24, 24, 24, 20, 17, 22, 23, 23, 21, 17, 21, 20, 24, 20, 19, 18, 21, 22, 23};
int feedforward_t;
int feedforward_k;


//Inhibition parameters

int cluster_window = 2;



#endif
