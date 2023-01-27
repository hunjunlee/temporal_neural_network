#include "tnn_model.h"
#include "read_mnist.h"
#include <time.h>

int main(int argc, char *argv[]){

    //cluster_window *= (layer1_time_max / image_pixel_max * 10);

	unsigned char* training_data;
	unsigned char* training_label;

	unsigned char* test_dat;
	unsigned char* test_lab;
	training_data = train_data();
	training_label = train_label();
	
	test_dat = test_data();
	test_lab = test_label();
	
	printf("read data done\n");

	layer_t* layer1 = layer_initialization(layer1_column_count, layer1_neuron_count, layer1_synapse_count);
	volley_t** input_set = (volley_t**)malloc(sizeof(volley_t*)*layer1_column_count);
	for(int i = 0; i < layer1_column_count; i++){
		input_set[i] = (volley_t*)malloc(sizeof(volley_t)*training_size);
	}

	input_encoding(input_set, training_size, training_data, layer1_time_max);	

	volley_t** test_set = (volley_t**)malloc(sizeof(volley_t*)*layer1_column_count);
	for(int i = 0; i < layer1_column_count; i++){
		test_set[i] = (volley_t*)malloc(sizeof(volley_t)*training_size);
	}

	input_encoding(test_set, test_size, test_dat, layer1_time_max);	

	printf("encoding done\n");
	
	int**** validation_set = (int****)malloc(sizeof(int***)*12);
	metrics_t** result = (metrics_t**)malloc(sizeof(metrics_t*)*12);
	for(int i = 0; i < 12; i++){
		validation_set[i] = (int***)malloc(sizeof(int**)*12);
		result[i] = (metrics_t*)malloc(sizeof(metrics_t)*12);
	}

	int**** training_set = (int****)malloc(sizeof(int***)*12);
	for(int i = 0; i < 12; i++){
		training_set[i] = (int***)malloc(sizeof(int**)*12);
	}

	int** total;
	int** max;
	total = (int**)malloc(sizeof(int*)*12);
	max = (int**)malloc(sizeof(int*)*12);
	for(int i = 0; i < 12; i++){
		max[i] = (int*)malloc(sizeof(int)*12);
		total[i] = (int*)malloc(sizeof(int)*12);
	}

    for(int i = 0; i < 20; i++){
        f_t_list[i] = (i + 1);
    }
    for(int i = 0; i < 26; i++){
        f_k_list[i] = (i + 1);
    }

    
    srand(0);

	for(int i = 0; i < 12; i++){
		for(int  j = 0; j < 12; j++){
			training_column(&layer1->column_set[i*12+j]\
                , input_set[i*12 + j], training_size\
                , layer1_time_max, layer1_weight_max);
            
            
            column_t* col = &layer1->column_set[i*12+j];
		}
	}
    
    printf("training done\n");

	for(int start = 0; start < 12; start++){
		for(int end = 0; end < 12; end++){
            total_step = 0;
            step_w_spike = 0;
            step_wo_spike = 0;
            incoming_spikes = 0;
            generated_spikes = 0;

            feedforward_t = 1;
            feedforward_k = f_t_dat[start * 12 + end];
            //printf("t: %d, k: %d\n", feedforward_t, feedforward_k);
            //printf("start: %d, end: %d\n", start, end);

			training_set[start][end] = cluster_all(training_size\
                , layer1_neuron_count, &layer1->column_set[start*12 + end]\
                , input_set[start*12 + end], layer1_rise_time, layer1_rho\
                , layer1_threshold, layer1_attenuation\
                , layer1_weight_max, layer1_time_max, cluster_window, 1);
            

	        //printf("clustering training data done!\n");
	        int*** cluster_count = (int***)malloc(sizeof(int**)*12);
	        cluster_identifier_t*** cluster = (cluster_identifier_t***)malloc(sizeof(cluster_identifier_t**)*12);

	        for(int i = 0; i < 12; i++){
	        	cluster_count[i] = (int**)malloc(sizeof(int*)*12);
	        	cluster[i] = (cluster_identifier_t**)malloc(sizeof(cluster_identifier_t*)*12);
	        }
	        for(int i = 0; i < 12; i++){
	        	for(int j = 0; j < 12; j++){
	        		cluster_count[i][j] = (int*)malloc(sizeof(int));
                    cluster_count[i][j][0] = 0;
	        	}
	        }

	        int**** cluster_set = (int****)malloc(sizeof(int***)*12);
	        int**** count = (int****)malloc(sizeof(int***)*12);
	        unsigned char*** max_label = (unsigned char***)malloc(sizeof(unsigned char**)*12);
	        for(int i = 0; i < 12; i++){
	        	cluster_set[i] = (int***)malloc(sizeof(int**)*12);
	        	count[i] = (int***)malloc(sizeof(int**)*12);
	        	max_label[i] = (unsigned char**)malloc(sizeof(unsigned char*)*12);
	        }

	        //printf("setting done!\n");
	        for(int i = start; i < start+1; i++){
	        	for(int j = end; j < end+1; j++){
	        		cluster[i][j] = volley_analyzer(training_size\
                        , layer1_neuron_count, training_set[i][j]\
                        , cluster_count[i][j], training_label);

	        		count[i][j] = cluster[i][j]->label_count;
	        		cluster_set[i][j] = cluster[i][j] ->identifier_set;
	        		max_label[i][j] = (unsigned char*)malloc(sizeof(unsigned char)*(*cluster_count[i][j]));

	        	}
	        }


	        //printf("\n");
	        //printf("volley analyzer done!\n");
	        

	        for(int i = start; i < start+1; i++){
	        	for(int j = end; j < end+1; j++){
                    //printf("cluster_count: %d\n", *cluster_count[i][j]);
	        		for(int k = 0; k < *cluster_count[i][j]; k++){
	        			int max_num = 0;
	        			for(int l = 0; l < 10; l++){
	        				if(count[i][j][k][l] > max_num){
	        					max_num = count[i][j][k][l];
	        					max_label[i][j][k] = l;
	        				}
	        				total[i][j] += count[i][j][k][l];
	        			}
	        			max[i][j] += max_num;
	        		}
	        	}
	        }


	        for(int i = start; i < start+1; i++){
	        	for(int j = end; j < end+1; j++){
	        		float accuracy = (float)max[i][j] / (float)total[i][j];
	        		float coverage = (float)total[i][j] / training_size;
	        		//printf("%d\t %f\t %d\t %d %f\n"\
                    //    , *cluster_count[i][j], accuracy\
                    //    , max[i][j], total[i][j], coverage);
	        	}

	        }
	        
	        for(int i = start; i < start+1; i++){
	        	for(int j = end; j < end+1; j++){
	        		validation_set[i][j] = cluster_all(test_size\
                        , layer1_neuron_count, &layer1->column_set[i*12 + j]\
                        , test_set[i*12 + j], layer1_rise_time, layer1_rho\
                        , layer1_threshold, layer1_attenuation\
                        , layer1_weight_max, layer1_time_max, cluster_window, 0);

	        		result[i][j] = compare_cluster_id\
                        (cluster_set[i][j], *cluster_count[i][j]\
                        , validation_set[i][j], test_size\
                        , max_label[i][j], test_lab, 24);

                    int train_input_size = training_size;
                    int test_input_size = test_size;
                    for(int k = 0; k < train_input_size; k++){
                        free(training_set[i][j][k]);
                    }
                    free(training_set[i][j]);
                    for(int k = 0; k < test_input_size; k++){
                        free(validation_set[i][j][k]);
                    }
                    free(validation_set[i][j]);
                    free(max_label[i][j]);
                    cluster_count[i][j][0] = 0;
	        	}                
	        }
	        

            printf("%f %f\n", result[start][end].accuracy, result[start][end].coverage);

            float incoming_spikes_f = incoming_spikes;
            float test_size_f = test_size;
            float neuron_count_f = layer1_neuron_count;

            float average_incoming_spikes = \
                incoming_spikes_f / test_size_f / neuron_count_f;
            //printf("num incoming spikes: %f\n", average_incoming_spikes);
            
            float generated_spikes_f = generated_spikes;

            float average_generated_spikes = \
                generated_spikes_f / test_size_f;
            //printf("avg generated spikes per volley: %f\n", average_generated_spikes);
		}
		
	}

    
    
    


	for(int i = 0; i < layer1_column_count; i++){
		free(test_set[i]);
	}
	free(test_set);
	for(int i = 0; i < layer1_column_count; i++){
		free(input_set[i]);
	}	
	free(input_set);

}
