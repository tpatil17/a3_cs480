#*************************************************************
# Name: Tanishq Patil
# Red Id: 132639686
# Assignment 3
#*************************************************************

#Makefile

#.RECIPEPREFIX +=

# Specify compiler
CC = gcc
# Compiler flags, if you want debug info, add -g
CCFLAGS = -std=c11 -g -Wall -c
CFLAGS = -g -c

# object files
OBJS = main.o BitMasker.o log.o PageTableLevel.o tracereader.o

# Program name
PROGRAM = pagingwithatc

# The program depends upon its object files
$(PROGRAM) : $(OBJS)
	$(CC) -o $(PROGRAM) $(OBJS)

main.o : main.c 
	$(CC) $(CCFLAGS) main.c -lm

PageTableLevel.o : PageTableLevel.c PageTableLevel.h 
	$(CC) $(CCFLAGS) PageTableLevel.c

BitMasker.o : BitMasker.c BitMasker.h
	$(CC) $(CCFLAGS) BitMasker.c

tracereader.o : tracereader.c tracereader.h
	$(CC) $(CCFLAGS) tracereader.c

log.o : log.c log.h
	$(CC) $(CCFLAGS) log.c

# Once things work, people frequently delete their object files.
# If you use "make clean", this will do it for you.
# As we use gnuemacs which leaves auto save files termintating
# with ~, we will delete those as well.
clean :
	rm -rf $(OBJS) *~ $(PROGRAM)