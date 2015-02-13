#@author Aaron Jacobson ajacob1

OBJS = wrapper.o  Direct.o  Matrix.o  Alignment.o  Encoded.o  main.o
CC = g++
CFLAGS = -Wall -c -g
LFLAGS = -Wall -g

cmps : $(OBJS)
	$(CC)  $(LFLAGS)  $(OBJS)  -o  cmps
# Each command must begin with a TAB.

wrapper.o : wrapper.cc  wrapper.hh
	$(CC)  $(CFLAGS)  wrapper.cc

Direct.o : Direct.cc  Direct.hh  wrapper.hh 
	$(CC)  $(CFLAGS)  Direct.cc

Matrix.o : Matrix.cc  Matrix.hh  wrapper.hh  prototype.hh
	$(CC)  $(CFLAGS)  Matrix.cc

Alignment.o : Alignment.cc  Alignment.hh  wrapper.hh  Matrix.hh  Direct.hh  prototype.hh
	$(CC)  $(CFLAGS)  Alignment.cc

Encoded.o : Encoded.cc  Encoded.hh  Alignment.hh  wrapper.hh  Matrix.hh  Direct.hh  prototype.hh
	$(CC)  $(CFLAGS)  Encoded.cc

main.o : main.cc  template.stack.min.hh  Encoded.hh  Alignment.hh  wrapper.hh  Matrix.hh  Direct.hh  prototype.hh
	$(CC)  $(CFLAGS)  main.cc


# Removes all .o files and executables.
# The backslash '\' stops "rm" from prompting the user
# for whether to remove the file.

clean : 
	\rm  *.o  cmps
