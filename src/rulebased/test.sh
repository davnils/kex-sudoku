#!/bin/bash
#
cat ../../testdata/minlex > inputTests 
while read line
do
echo -e "$line\n" | ./rulebased1
echo -e "\n\n"
done <inputTests 
rm inputTests
