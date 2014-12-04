# An example makefile for C++ code

OBJS = main.o  Direct.o  wrapper.o  Encoded.o
CC = g++
CFLAGS = -Wall -c -g
LFLAGS = -Wall -g

cmps : $(OBJS)
	$(CC)  $(LFLAGS)  $(OBJS)  -o  demo
# Each command must begin with a TAB.

main.o : main.cc  Organism.hh  template.hh  wrapper.hh
	$(CC)  $(CFLAGS)  main.cc

Direct.o : Direct.cc  Direct.hh  wrapper.hh
	$(CC)  $(CFLAGS)  Organism.cc

wrapper.o : wrapper.cc  wrapper.hh
	$(CC)  $(CFLAGS)  wrapper.cc

Encoded.o : Encoded.cc  Encoded.hh  wrapper.hh
	$(CC)  $(CFLAGS)  Encoded.cc





# Removes all .o files and executables.
# The backslash '\' stops "rm" from prompting the user
# for whether to remove the file.

clean : 
	\rm  *.o  demo
