#!/bin/sh
# Start a file with the above line (/bin/sh is a symbolic link to the shell's executable).
# Type Unix commands into the file.
# Make the file executable (e.g. chmod u+x z.sh) and then run it,
# Any line beginning with the character '#' is a comment and is ignored.
# The only exception is the first line in the file.
# Type "y.sh v" to run with valgrind"; type "y.sh" to run without it.
if [ $# -gt 0 ]
then
g++ -g  proj2.part1.cc
sleep 1s
valgrind --tool=memcheck --leak-check=yes   a.out  A1.txt  B1.txt  -20  40 2 > A1.B1.out
else
g++  proj2.part1.cc
sleep 1s
a.out  A1.txt  B1.txt  -20  40 2 > A1.B1.out
fi
