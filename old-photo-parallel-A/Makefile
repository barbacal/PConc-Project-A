# Define required macros here
# SHELL = /bin/bash

CC = gcc
CFLAGS = -I. -g -Wall -std=c11 -v -pedantic -lgd -lpthread
INCLUDES =#
LIBS =#
#OBJ_DIR = ../build
DEPS = #Processa_ficheiros.h image-lib.h
EXEC = ../old-photo-parallel-A

all: $(EXEC)

%.o: %.c ${DEPS}
	${CC} -c -o $@ $< $(CFLAGS) ${INCLUDES}
	@echo Compiled 

$(EXEC): old-photo-parallel-A.o Processa_ficheiros.o image-lib.o
	${CC} -o old-photo-parallel-A old-photo-parallel-A.o Processa_ficheiros.o image-lib.o ${LIBS}


clean:
	rm -f *.o *~ core old-photo-parallel-A
	@echo Cleanned
   
clean-data:#????
	rm -fr *.out* ./*-dir
	@echo Cleanned data
run:
	./$(EXEC) $(ARGS)
	@echo Ran