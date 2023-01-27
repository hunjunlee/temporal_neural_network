SRCFILES = main.c config.c read_mnist.c tnn_model.c
CFLAGS 			= -Wall -g -O3 -lpthread -pthread
SRCS            = .c 

TARGET          = main
LIBS            = 

all: main.c $(OBJFILES) 
	$(CC) $(CFLAGS) $(SRCFILES) $(LDFLAGS)
	mv a.out $(TARGET)
clean:
	rm $(TARGET)
