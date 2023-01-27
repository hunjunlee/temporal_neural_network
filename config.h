#ifndef __CONFIG_H__
#define __CONFIG_H__

#include <stdbool.h>

const bool ffi_enable;
const bool li_enable;

const float layer1_rise_time;
//(Tr in msec)
const float layer1_rho;
//Tr and Tf ratio
const int layer1_weight_max;
const float image_pixel_max;
//weight maximum(number of synapses)
const float layer1_time_max;
//maximum time window
const float layer1_threshold;
//threshold of neuron voltage
const float layer1_attenuation;
//attenuation value for neuron model2
const int training_size;
const int test_size;
const int n_rows;
const int n_cols;

const int layer1_column_count;
const int layer1_neuron_count;
const int layer1_synapse_count;

float lateral_t;
float lateral_k;
int f_t_list[20];
int f_k_list[26];
int f_t_dat[144];
int feedforward_t;
int feedforward_k;
//Inhibition parameters

int cluster_window;



#endif
