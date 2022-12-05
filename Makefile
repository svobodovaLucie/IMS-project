# File:          Makefile
# Institution:   FIT BUT
# Academic year: 2022/2023
# Course:        IMS - Modelling and Simulation
# Author:        Lucie Svobodová, xsvobo1x@stud.fit.vutbr.cz
#
# IMS project, variant 10: Acute Normovolemic Hemodilution

CC=g++
CFLAGS=-Wall -Wextra -g -Werror
LFLAGS=-lsimlib -lm
EXEC= 10_xsvobo1x

all: $(EXEC)

$(EXEC): 10_xsvobo1x.o
	$(CC) $(CFLAGS) -o $@ $^ $(LFLAGS)

10_xsvobo1x.o: 10_xsvobo1x.cpp 10_xsvobo1x.h
	$(CC) $(CFLAGS) -c $< $(LFLAGS)

run: 
	./$(EXEC)

clean:
	rm $(EXEC) 10_xsvobo1x.o 10_xsvobo1x_*.dat