#include "tnn_model.h"

int find_min(int* values, int size){

	int min = -1;
	int k = 0;
	for(; k < size; k++){
		if(values[k] != -1){
			min = values[k];
			break;
		}
	}
	for(int i = k + 1; i < size; i++){
		if(values[i] < min && values[i] != -1){
			min = values[i];
		}
	}

	return min;
}
int compare_int_array(int* a, int* b, int size){
	for(int i = 0; i < size; i++){
		if(a[i] != b[i]){
			return 0;
		}
	}
	return 1;
}

metrics_t compare_cluster_id(int** cluster_set, int cluster_count\
    , int** validation_set, int validation_size\
    , unsigned char* max_label, unsigned char* validation_label, int line_size){
	metrics_t result;
	int correct = 0;
	int cover = 0;
	float accuracy;
	float coverage;
	for(int i = 0; i < validation_size; i++){
		int is_null = 0;
		for(int j = 0; j < line_size; j++){
			if(validation_set[i][j] != 0){
				j = line_size;
			}
			else if(j == line_size - 1){
				is_null = 1;	
			}
		}
		if(!is_null){	
			for(int j = 0; j < cluster_count; j++){
				if(compare_int_array(validation_set[i], cluster_set[j], line_size)){
					if(max_label[j] == validation_label[i]){
						correct++;	
					}
					cover++;
					j = cluster_count - 1;						
				}
			}	
		}
	}
	coverage = (float)cover/validation_size;
	accuracy = (float)correct/cover;
	result.coverage = coverage;
	result.accuracy = accuracy;
	return result;

}


void training_neuron(neuron_t* neuron, volley_t* input_set\
    , int input_size, float time_max, int weight_max){

	int synapse_count = neuron->synapse_count; 
	int* trained_weights = (int*)malloc(sizeof(int)*synapse_count);
	for(int i = 0; i < synapse_count; i++){
		trained_weights[i] = 0;
	}
	for(int i = 0; i < input_size; i++){
		int min = find_min(input_set[i].values, synapse_count);
		for(int j = 0; j < synapse_count; j++){
            if(input_set[i].values[j] != -1)
    			trained_weights[j] += input_set[i].values[j] - min;
		}
	}
	for(int i = 0; i < synapse_count; i++){
		trained_weights[i] = trained_weights[i] / input_size;
		trained_weights[i] = (time_max - trained_weights[i]) * weight_max / time_max;       
        int temp = trained_weights[i];
        trained_weights[i] = temp;
	}

	neuron->weights = trained_weights;
}

void training_column(column_t* column, volley_t* input_set\
    , int input_size, float time_max, int weight_max){

	int line_count = input_set[0].line_count;
	int synapse_count = column->excitatory_column->neuron_set[0].synapse_count;
	int neuron_count = column->excitatory_column->neuron_count;
	column->excitatory_column->input_size = line_count;

    for(int i = 0; i < neuron_count; i++){
        column->excitatory_column->neuron_set[i].srcs \
            = (int*)malloc(sizeof(int)*synapse_count);
	}



	for(int i = 0; i < neuron_count; i++){
        int* temp_list = (int*)malloc(sizeof(int)*line_count);
        for(int j = 0; j < line_count; j++){
            temp_list[j] = j;//(line_count - j - 1);
        }
        for(int j = 0; j < line_count; j++){
            int k = j + rand() % (line_count - j);
            int temp = temp_list[j];
            temp_list[j] = temp_list[k];
            temp_list[k] = temp;
        }
		for(int j = 0; j < synapse_count; j++){
			column->excitatory_column->\
                intra_column_interconnection[i][j] \
                = temp_list[j];//rand() % line_count;            

            column->excitatory_column->\
                neuron_set[i].srcs[j] = \
                column->excitatory_column->
                    intra_column_interconnection[i][j];

		}
        free(temp_list);
	}

        
	//value inside intra_column_interconnection[i][j] = ith neuron and jth synapse is connected to value(th) line in input volley
	
	volley_t** training_input = (volley_t**)malloc(sizeof(volley_t*)*neuron_count);
	for(int i = 0; i < neuron_count; i++){
		training_input[i] = (volley_t*)malloc(sizeof(volley_t)*input_size);
	}
	for(int i = 0; i < neuron_count; i++){
		for(int j = 0; j < input_size; j++){
			training_input[i][j].line_count = synapse_count;
			training_input[i][j].values = (int*)malloc(sizeof(int)*synapse_count);	
			for(int k = 0; k < synapse_count; k++){
				int index = column->excitatory_column->intra_column_interconnection[i][k];
				int value = input_set[j].values[index];
				training_input[i][j].values[k] = value;
			}
		}
		neuron_t* train_neuron = &(column->excitatory_column->neuron_set[i]);
		training_neuron(train_neuron, training_input[i], input_size, time_max, weight_max);

        for(int j = 0; j < input_size; j++){
            free(training_input[i][j].values);
        }
        free(training_input[i]);
    }
    free(training_input);
}

void sort_array(float* arr, int size){
	
	for(int i = 0; i < size - 1; i++){
		for(int j = 0; j < size - i - 1; j++){
			if(arr[j] > arr[j + 1]){
				float temp = arr[j];
				arr[j] = arr[j + 1];
				arr[j + 1] = temp;
			}
		}
	}
}

int search_in_sorted(float* arr, float search, int size){
	int index = -1;
	for(int i = 0; i < size; i++){
		if(arr[i] == search){
			index = i;
		}
	}
	return index;

}

float neuron_evaluation(neuron_t* neuron, volley_t* input\
    , float rise_time, float rho, float threshold\
    , float attenuation, float max_weight\
    , float max_time, int is_training, int nid){
	
    int size = neuron->synapse_count;


    // Add REFERENCE DSRM0 model
    float time_window_size = max_time;
    int num_steps = time_window_size * 2;


    float* accum_weight_time = (float*)malloc(sizeof(float) * num_steps);
    for(int i = 0; i < num_steps; i++)
        accum_weight_time[i] = 0;

    int weight_granularity = 1;
    for(int i = 0; i < size; i++){
        if(input->values[i] != -1){
            
            if(is_training == 0)
                incoming_spikes += 1;

            int arrive_time_step = input->values[i];
            int weight_step = neuron->weights[i];

            accum_weight_time[arrive_time_step + weight_step] \
                += (10. / rise_time) * weight_step;
        }
    }

    float v_t = 0;
    float gE_t = 0;
    float spike_time = -1;

    float decay_gE = 1. - 1. / 5.0;
    float decay_v = 1. - 1. / 40.0;

	for(int step = 0; step < num_steps; step++){
        float accumulated_weight = accum_weight_time[step];
        if(is_training == 0){
            if(accumulated_weight != 0){
                step_w_spike += 1;
            }
            else{
                step_wo_spike += 1;
            }

        }
        gE_t *= decay_gE;
        v_t *= decay_v;

        v_t += gE_t;
        gE_t += accumulated_weight;

        if(v_t >= threshold){
            spike_time = step;
            //spike_time /= 10.0;
            break;
        }
	}    
    if(is_training == 0){
        total_step += num_steps;
        if(spike_time != -1)
            generated_spikes += 1;
    }
    free(accum_weight_time);
	
//    if(is_training == 0)
//        printf("spike: %f\n", spike_time);


	return spike_time;	
}

void select_min_k(float* value_set, int* index_set, int size, int limit, volley_t* volley){
	
	float* min_k = (float*)malloc(sizeof(float)*limit);
	int* min_k_index = (int*)malloc(sizeof(int)*limit);
	for(int i = 0; i < limit; i++){
		min_k[i] = -1;
	}
	for(int i = 0; i < size; i++){
		for(int j = limit-1; j >= 0; j--){
			if(min_k[j] < value_set[i] && min_k[j] != -1){
				if(j != limit - 1){
					for(int k = limit - 2; k > j ; k--){
						min_k[k + 1] = min_k[k];
						min_k_index[k + 1] = min_k_index[k];
					}
					min_k[j + 1] = value_set[i];
					min_k_index[j + 1] = i;
				}
				j = 0;
			}
			else if(j == 0){
				for(int k = limit - 2; k >= 0; k--){
					min_k[k + 1] = min_k[k];
					min_k_index[k + 1] = min_k_index[k];
				}
				min_k[0] = value_set[i];
				min_k_index[0] = i;
			}
		}

	}
	for(int i = 0; i < size; i++){
		int check = index_set[i];
		for(int j = 0; j < limit; j++){
			if(check == min_k_index[j]){
				j = limit - 1;
			}
			else if(j == limit - 1){
				volley->values[check] = -1;
			}
		}
	}
	free(min_k);
	free(min_k_index);


}



void lateral_inhibition(column_t* column, volley_t* volley){
	lateral_ic_t* lateral_ic = column->lateral_ic;	
	//float time = lateral_ic->time;
	//float count = lateral_ic->count;
	float time = 1;
	int count = 2;
	int non_null = 0;
	float* value_set = (float*)malloc(sizeof(float)*volley->line_count);
	int* index_set = (int*)malloc(sizeof(int)*volley->line_count);

	float min = find_min(volley->values, volley->line_count);
	for(int i = 0; i < volley->line_count; i++){
		if(volley->values[i] > min + time || volley->values[i] == -1){
			volley->values[i] = -1;
		}
		else{
			value_set[non_null] = volley->values[i];
			index_set[non_null] = i;
			non_null++;
		}
	}
	if(non_null > count){
		select_min_k(value_set, index_set, non_null, count, volley);	
	}

    free(value_set);
    free(index_set);
	
}

void feedforward_inhibition(column_t* column, volley_t* volley){
	int* values = volley->values;
	int detect = 0;
	int min = find_min(values, volley->line_count);

	for(int i = 0; i < volley->line_count; i++){
		
		if(values[i] < min + feedforward_t){
			detect++;
		}	
	}
	if(detect >= feedforward_k){
		for(int i = 0; i < volley->line_count; i++){
			values[i] = -1;
		}
	}

}


volley_t* column_evaluation(column_t* column, volley_t* input_set, float rise_time, float rho, float threshold, float attenuation, float max_weight, float max_time, int is_training){


	volley_t* output = (volley_t*)malloc(sizeof(volley_t));
	int neuron_count = column->excitatory_column->neuron_count;
	int synapse_count = column->excitatory_column->neuron_set->synapse_count;
	int* output_value = (int*)malloc(sizeof(float)*neuron_count);

	
	output->values = output_value;
	output->line_count = neuron_count;


	volley_t* evaluation_input = (volley_t*)malloc(sizeof(volley_t)*neuron_count);
    if(ffi_enable)
	    feedforward_inhibition(column, input_set);


    // get poisson input from the external

	for(int i = 0; i < neuron_count; i++){
		evaluation_input[i].line_count = synapse_count;
		evaluation_input[i].values = (int*)malloc(sizeof(int)*synapse_count);	

		for(int k = 0; k < synapse_count; k++){
			int index = column->excitatory_column->intra_column_interconnection[i][k];
			int value = input_set->values[index];
			evaluation_input[i].values[k] = value;
		}
		int min = find_min(evaluation_input[i].values, synapse_count);
		for(int k = 0; k < synapse_count; k++){
			if(evaluation_input[i].values[k] != -1){
				evaluation_input[i].values[k] -= min;	
			}
		}


		neuron_t* eval_neuron = &(column->excitatory_column->neuron_set[i]);
		int spike_time = neuron_evaluation(eval_neuron\
            , &evaluation_input[i], rise_time, rho\
            , threshold, attenuation, max_weight, max_time, is_training, i);
		if(spike_time != -1){
			spike_time += min;
		}
		output_value[i] = spike_time;
        free(evaluation_input[i].values);
	}
    free(evaluation_input);

	//lateral_inhibition(column, output);	
	return output;
}



void cluster_identifier(int* identifier, int* output\
    , int line_size, float time_window, int is_training){

	int min = find_min(output, line_size);
	for(int i = 0; i < line_size; i++){
		if(output[i] == -1){
			identifier[i] = 0;
		}
		else if(output[i] < min + time_window){
			identifier[i] = 1;
		}
		else{
			identifier[i] = 0;
		}
	}	
    
}

int** cluster_all(int input_size, int line_count\
    , column_t* column, volley_t* input_set\
    , float rise_time, float rho, float threshold\
    , float attenuation, float max_weight\
    , float max_time, float time_window, int is_training){

	int** identifier_set = (int**)malloc(sizeof(int*) * input_size);
	for(int i = 0; i < input_size; i++){
		identifier_set[i] = (int*)malloc(sizeof(int) * line_count);
        for(int j = 0; j < line_count; j++)
            identifier_set[i][j] = 0;
	}
	for(int i = 0; i < input_size; i++){
		volley_t* output_volley = column_evaluation(column\
            , &input_set[i], rise_time, rho, threshold\
            , attenuation, max_weight, max_time, is_training);

		cluster_identifier(identifier_set[i], output_volley->values\
            , line_count, time_window, is_training);

        free(output_volley->values);
        free(output_volley);
	}

	return identifier_set;
}

cluster_identifier_t* volley_analyzer(int input_size, int line_count, int** input_set, int* cluster_count, unsigned char* label){
	cluster_count[0] = 0;
	int** identifier_set = (int**)malloc(sizeof(int*) * input_size);
	for(int i = 0; i < input_size; i++){
		identifier_set[i] = (int*)malloc(sizeof(int) * line_count);
	}
	int** label_count = (int**)malloc(sizeof(int*) * input_size);
	for(int i = 0; i < input_size; i++){
		label_count[i] = (int*)malloc(sizeof(int) * 10);
	}

	for(int i = 0; i < input_size; i++){
		for(int j = 0; j < 10; j++){
			label_count[i][j] = 0;
		}
	}

	cluster_identifier_t* cluster_identifier = (cluster_identifier_t*)malloc(sizeof(cluster_identifier_t));


	cluster_identifier->identifier_set = identifier_set;
	cluster_identifier->label_count = label_count;

	for(int i = 0; i < line_count; i++){
		identifier_set[cluster_count[0]][i] = input_set[0][i];
	}
	int cluster_label = label[0];
	label_count[0][cluster_label] += 1;
	cluster_count[0] = cluster_count[0] + 1;
	for(int i = 1; i < input_size; i++){
		int is_null = 0;

		for(int j = 0; j < line_count; j++){
			if(input_set[i][j] != 0){
				j = line_count - 1;
			}
			else if(j == line_count - 1){
				is_null = 1;	
			}
		}
		
		if(!is_null){
			for(int j = 0; j < cluster_count[0]; j++){
				int is_same = 1;

				for(int k = 0; k < line_count; k++){


					if(input_set[i][k] != identifier_set[j][k]){
						is_same = 0;
					}
				}
				if(is_same == 0 && j == cluster_count[0] - 1){
					for(int k = 0; k < line_count; k++){
						identifier_set[cluster_count[0]][k] = input_set[i][k];
					}
					int cluster_label = label[i];
					label_count[j][cluster_label] += 1;
					cluster_count[0] = cluster_count[0] + 1;
					j = cluster_count[0] - 1;
				}
				if(is_same == 1){
					int cluster_label = label[i];
					label_count[j][cluster_label] += 1;
					j = cluster_count[0] - 1;
				}
			}
		}
	}
	return cluster_identifier;
}




// encode input to volley
void input_encoding(volley_t** input_volley, int size, unsigned char* data, float time_max){
    for(int i = 0; i < size; i++){
        for(int  j = 0; j < 12; j++){
            for(int  k = 0; k < 12; k++){
                int* values = (int*)malloc(sizeof(int)*50);
                for(int l = 0; l < 5; l++){
                    for(int  m = 0; m < 5; m++){
                        values[5*l + m] = \
                            data[i*28*28 + (2*j*28 + 2*k) + (l*28 + m)] \
                            * time_max / image_pixel_max;
                    }
                }
                input_volley[j*12 + k][i].line_count = 25;
                input_volley[j*12 + k][i].values = values;
                onoff_encoding(&input_volley[12*j + k][i], time_max);
            }
        }
    }
}



//encode using on-off encoding
void onoff_encoding(volley_t* volley, float time_max){
	volley->line_count = volley->line_count * 2;
	
	for(int i = 0; i < volley->line_count; i++){
		if(i < volley->line_count/2){
            volley->values[i] = time_max - volley->values[i];
            //if(volley->values[i] != 0)
            //    printf("EE\n");
		}
		else{
			volley->values[i] = time_max - volley->values[i-volley->line_count/2];
		}
	}	
}

//initialize layer and allocate memories accordingly
layer_t* layer_initialization(int column_count, int neuron_count, int synapse_count){
	layer_t* layer = (layer_t*)malloc(sizeof(layer_t));	
	column_t* column_set = (column_t*)malloc(sizeof(column_t)*column_count);
	
	feedforward_ic_t* feedforward_ic = (feedforward_ic_t*)malloc(sizeof(feedforward_ic_t)*column_count);
	lateral_ic_t* lateral_ic = (lateral_ic_t*)malloc(sizeof(lateral_ic_t)*column_count);
	
	excitatory_column_t* excitatory_columns = (excitatory_column_t*)malloc(sizeof(excitatory_column_t)*column_count);
	neuron_t* neuron_sets = (neuron_t*)malloc(sizeof(neuron_t)*neuron_count*column_count);	
	//memory allocation for all components in a layer

	layer->column_count = column_count;
	layer->column_set = column_set;
	//layer data allocation

	for(int i = 0; i < column_count; i++){
        if(ffi_enable){
            column_set[i].feedforward_ic = &feedforward_ic[i];
        }
		column_set[i].excitatory_column = &excitatory_columns[i];
        if(li_enable){
            column_set[i].lateral_ic = &lateral_ic[i];
        }
        column_set[i].column_id = i;
	}
	//column data allocation

	for(int i = 0; i < column_count; i++){
		excitatory_columns[i].neuron_set = &neuron_sets[i*neuron_count];
		excitatory_columns[i].neuron_count = neuron_count;
		excitatory_columns[i].intra_column_interconnection = (int**)malloc(sizeof(int*)*neuron_count);
		for(int j = 0; j < neuron_count; j++){
			excitatory_columns[i].intra_column_interconnection[j] = (int*)malloc(sizeof(int)*synapse_count);
		}
	}
	//excitatory column data allocation

	for(int i = 0; i < column_count; i++){
		for(int j = 0; j < neuron_count; j++){
			neuron_sets[i*neuron_count + j].synapse_count = synapse_count;
		}
	}
	//neuron data allocation
	//
	//did not allocate data for inhibition
	

	return layer;

}


//free 4d int array
void free_4d_int(int**** arr, int a, int b, int c){
	for(int i = 0; i < a; i++){
		for(int j = 0; j < b; j++){
			for(int k = 0; k < c; k++){
				free(arr[i][j][k]);
			}
		}
	}
	for(int i = 0; i < a; i++){
		for(int j = 0; j < 12; j++){
			free(arr[i][j]);
		}
	}
	for(int i = 0; i < a; i++){
		free(arr[i]);
	}
	free(arr);
}



void free_layer(layer_t* layer){


}
