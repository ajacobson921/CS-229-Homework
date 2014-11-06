#!/bin/sh
# Start a file with the above line (/bin/sh is a symbolic link to the shell's executable).
# Type Unix commands into the file.
# Make the file executable (e.g. chmod u+x z.sh) and then run it,
# Any line beginning with the character '#' is a comment and is ignored.
# The only exception is the first line in the file.
# Type "z.sh v" to compile and run with valground; type "z.sh" to run without it.
if [ $# -gt 0 ]
then
g++ -g  proj2.part1.cc
sleep 1s
valgrind --tool=memcheck --leak-check=yes a.out sample.seq1.txt sample.seq2.txt -20 40 2 > sample.out
else
g++  proj2.part1.cc
sleep 1s
a.out sample.seq1.txt sample.seq2.txt -20 40 2 > sample.out
fi

# $0 is the command name.
# $1 is the first argument, $2 the second, etc.
# $# is the number of arguments not counting the command name.
# n1 -eq n2   True if the integers n1 and n2 are algebraically equal.
# n1 -ne n2   True if the integers n1 and n2 are not algebraically equal.
# n1 -gt n2   True if the integer n1 is algebraically greater than the integer n2.
# n1 -ge n2   True if the integer n1 is algebraically greater than or equal to the integer n2.
# n1 -lt n2   True if the integer n1 is algebraically less than the integer n2.
# n1 -le n2   True if the integer n1 is algebraically less than or equal to the integer n2.
# s1          True if s1 is not the null string.
# s1 = s2     True if strings s1 and s2 are identical.
# s1 != s2    True if strings s1 and s2 are not identical.
