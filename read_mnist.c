#include <stdio.h>
#include <stdlib.h>

int reverseInt (int i) 	
{
	//reverse data for magic number, row and column
	//integer value are read in reverse way (magic number, n_row, n_col)
	unsigned char c1, c2, c3, c4;

	c1 = i & 255;
	c2 = (i >> 8) & 255;
	c3 = (i >> 16) & 255;
	c4 = (i >> 24) & 255;

	return ((int)c1 << 24) + ((int)c2 << 16) + ((int)c3 << 8) + c4;
}

void read_data(unsigned char* data, char* name, int is_data){
	FILE *mnist;
	int magic_number;
	int number_of_images;
	int n_rows;
	int n_cols;
	mnist = fopen(name, "r");	
	if(mnist){
		
		fread(&magic_number, sizeof(magic_number), 1, mnist);
		magic_number= reverseInt(magic_number);
		fread(&number_of_images,sizeof(number_of_images), 1, mnist);
		number_of_images= reverseInt(number_of_images);
		if(is_data){
			//data read
			fread(&n_rows,sizeof(n_rows), 1, mnist);
			n_rows= reverseInt(n_rows);
			fread(&n_cols,sizeof(n_cols), 1, mnist);
			n_cols= reverseInt(n_cols);
			//column size and row size is included only in data file
			for(int i = 0; i < number_of_images; i++){
				for(int j = 0; j < n_rows; j++){
					for(int k = 0; k < n_cols; k++){
						fread(&data[i*n_rows*n_cols + j*n_cols + k], sizeof(data[0]), 1, mnist);	
					}
				
				}
			}
		}
		else{
			//pixel read
			for(int i = 0; i < number_of_images; i++){
				fread(&data[i], sizeof(data[0]), 1, mnist);
			}
		
		}
		fclose(mnist);
	
	}
	else{
		printf("error occured");
	
	}
	

}

unsigned char* train_data(){
	//read mnist train-set pixel data
	unsigned char* data = (unsigned char*)malloc(sizeof(unsigned char)*60000*28*28);
	read_data(data, "data/train-images-idx3-ubyte", 1);
	return data;
}
unsigned char* test_data(){
	//read mnist test-set pixel data
	unsigned char* data = (unsigned char*)malloc(sizeof(unsigned char)*10000*28*28);
	read_data(data, "data/t10k-images-idx3-ubyte", 1);
	return data;
}

unsigned char* train_label(){
	//read mnist train-set label
	unsigned char* label = (unsigned char*)malloc(sizeof(unsigned char)*60000);	
	read_data(label, "data/train-labels-idx1-ubyte", 0);
	return label;
}

unsigned char* test_label(){
	//read mnist test-set label
	unsigned char* label = (unsigned char*)malloc(sizeof(unsigned char)*60000);	
	read_data(label, "data/t10k-labels-idx1-ubyte", 0);
	return label;
}

