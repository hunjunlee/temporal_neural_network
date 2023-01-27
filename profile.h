#include <stdio.h>
#include <stdlib.h>

#ifndef __PROFILE_H__
#define __PROFILE_H__

long long int total_step;
long long int step_w_spike;
long long int step_wo_spike;

long long int incoming_spikes;
long long int incoming_spikes;

long long int generated_spikes;

FILE* poisson_input;
FILE* connection;
FILE* spike_out;
FILE* voltage_out;

#endif
