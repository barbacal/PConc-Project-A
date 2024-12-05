CC = gcc
CFLAGS = -I. -g -Wall -Wextra -std=c11 -v -pedantic
INCLUDES = #
LIBS = -lgd -lpthread -lrt
DEPS = #
EXEC = ../old-photo-parallel-A

all: $(EXEC)

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS) $(INCLUDES)
	@echo Compiled 

$(EXEC): old-photo-parallel-A.o Processa_ficheiros.o image-lib.o
	$(CC) -o $(EXEC) old-photo-parallel-A.o Processa_ficheiros.o image-lib.o $(LIBS)
	@echo Built

clean:
	rm -fr *.o
	@echo Cleaned

clean-all:
	rm -fr *~ core $(EXEC) *.o
	@echo Cleaned all

run:
	./$(EXEC) $(ARGS)
	@echo Ran
