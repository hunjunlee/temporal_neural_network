#ifndef __READ_MNIST__
#define __READ_MNIST__

int reverseInt(int i);
void read_data(unsigned char* data, char* name, int is_data);

unsigned char* train_data();
unsigned char* test_data();
unsigned char* train_label();
unsigned char* test_label();


#endif
