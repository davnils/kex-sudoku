#!/bin/bash
#
head -n 5000 ../../../testdata/minlex | tail -n 1000 > inputTests5
while read line
do
echo -e "$line\n" | ./main
done <inputTests5
rm inputTests5
