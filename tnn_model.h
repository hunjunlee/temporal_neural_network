#include <stdio.h>
#include <stdlib.h>
#include "config.h"
#include "profile.h"
#ifndef __TNN_MODEL_H__
#define __TNN_MODEL_H__

typedef struct metrics{
	float accuracy;
	float coverage;
} metrics_t;


typedef struct cluster_identifier{
	int** identifier_set;
	int** label_count;
}cluster_identifier_t;

typedef struct volley{
	int* values;
	int line_count;
} volley_t;

typedef struct neuron{
		
	int* weights;
    int* srcs;
	int synapse_count;
	
} neuron_t;

//check if neuron's synapse count == volley line count, divide all with input size

typedef struct excitatory_column{
	int input_size;
	neuron_t* neuron_set;
	int neuron_count;
	int ** intra_column_interconnection;

} excitatory_column_t;
// function about interconnection training should be made...

typedef struct feedforward_ic{
	float time;
	float count;
	//each tf and kf
} feedforward_ic_t;

typedef struct lateral_ic{
	float time;
	float count;
	//each tl and kl
} lateral_ic_t;

//feedforward inhibition and lateral inhibition should be made later... excitatory first

typedef struct column{

    int column_id;
	feedforward_ic_t* feedforward_ic;
	lateral_ic_t* lateral_ic;
	excitatory_column_t* excitatory_column;


} column_t;

typedef struct layer{
	column_t* column_set;
	int column_count;
} layer_t;


layer_t* layer_initialization(int column_count, int neuron_count, int synapse_count);

void input_encoding(volley_t** input_volley, int size, unsigned char* data, float time_max);
void onoff_encoding(volley_t* volley, float time_max);

void training_neuron(neuron_t* neuron, volley_t* input_set, int input_size, float time_max, int weight_max);
void training_column(column_t* column, volley_t* input_set, int input_size, float time_max, int weight_max);

float neuron_evaluation(neuron_t* neuron, volley_t* volley, float rise_time, float rho, float threshold, float attenuation, float max_weight, float max_time, int is_training, int nid);

volley_t* column_evaluation(column_t* column, volley_t* volley, float rise_time, float rho, float threshold, float attenuation, float max_weight, float max_time, int is_training);

void cluster_identifier(int* identifier, int* output, int line_size, float time_window, int is_training);

cluster_identifier_t* volley_analyzer(int input_size, int line_count, int** input_set, int* cluster_count, unsigned char* label);

int** cluster_all(int input_size, int line_count, column_t* column, volley_t* input_set, float rise_time, float rho, float threshold, float attenuation, float max_weight, float max_time, float time_window, int is_training);

metrics_t compare_cluster_id(int** cluster_set, int cluster_count, int** validation_set, int validation_size, unsigned char* max_label, unsigned char* validation_label, int line_size);

void lateral_inhibition(column_t* column, volley_t* volley);

void feedforward_inhibition(column_t* column, volley_t* volley);

void free_4d_int(int**** arr, int a, int b, int c);



#endif
