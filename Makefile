# makefile for Virtual Memeory Unit (MMU)
#
# usage: make mmu 

CC = gcc
CFLAGS = -Wall

clean:
	rm -rf *.o
	rm -rf mmu
	find *.csv ! -name correct256.csv ! -name correct128.csv -type f -delete

mmu: mmu_driver.o mmu_interpret.o mmu_support.o 
	$(CC) $(CFLAGS) -g -o mmu mmu_driver.o mmu_interpret.o mmu_support.o 

mmu_driver.o: mmu_driver.c
	$(CC) $(CFLAGS) -g -c mmu_driver.c

mmu_interpret.o: mmu_interpret.c
	$(CC) $(CFLAGS) -g -c mmu_interpret.c

mmu_support.o: mmu_support.c mmu_support.h
	$(CC) $(CFLAGS) -g -c mmu_support.c

