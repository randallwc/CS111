#!/bin/bash

./lab4b --log=test_output --scale=C & 
pid=$!
sleep 10
kill $pid

output=$(cat test_output)
rm -rf test_output

pass="PASS"
#regex for each line
#08:39:17 71.1
regex="^[[:digit:]] {2}:[[:digit:]] {2}:[[:digit:]] {2} [[:digit:]] +\.[[:digit:]] {1}$"

for line in $(output)
do
	if [[ $line =~  $regex]]
	then
		#is true
	else
		#failed
		pass="FAILED"
	fi
done

echo "$pass"