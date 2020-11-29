#!/bin/bash

./lab4b --log=test_output --scale=C & 
pid=$!
sleep 10
kill $pid

tr " " "-" < test_output > translated_out
output=$(cat translated_out)
rm -rf test_output
rm -rf translated_out

pass="PASS"
#regex for each line
#08:39:17 71.1
regex="^[0-9]{2}:[0-9]{2}:[0-9]{2}-[0-9]+\.[0-9]{1}$"

for line in $output
do
	if [[ $line =~  $regex ]]
	then
		continue #is true
	else
		pass="FAILED" #failed
	fi
done

echo "$pass"