#!/bin/bash
#
cat ../../../testdata/minlex > inputTests5
#head -n 5000 samples | tail -n 1000 > inputTests5
while read line
do
echo -e "$line\n" | ./rulebased
done <inputTests5
rm inputTests5
