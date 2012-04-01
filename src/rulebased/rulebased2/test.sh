#!/bin/bash
#
head -n 10000 ../../../testdata/minlex > inputTests 
while read line
do
echo -e "$line\n" | ./main
done <inputTests 
rm inputTests
