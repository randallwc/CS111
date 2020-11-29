#!/bin/bash
# ./lab4b --log=test_output --scale=C & 
# pid=$!
# sleep 5
# kill $pid

# tr " " "-" < test_output > translated_out
# output=$(cat translated_out)
# rm -rf test_output
# rm -rf translated_out

# pass="PASS"
# #regex for each line
# #08:39:17 71.1
# regex="^[0-9]{2}:[0-9]{2}:[0-9]{2}-[0-9]+\.[0-9]{1}$"

# for line in $output
# do
# 	if [[ $line =~  $regex ]]
# 	then
# 		#is true
# 		continue
# 	else
# 		#failed
# 		pass="FAILED"
# 	fi
# done

# echo "$pass"


./lab4b --period=2 --scale=C --log=smoketest_res &
pid=$!
sleep 5
kill $pid

tr " " "_" < smoketest_res > smoketest_result
rm smoketest_res
test=$(cat smoketest_result)
rm smoketest_result

result="Passes smoketest"
expected="[0-9]{2}:[0-9]{2}:[0-9]{2}_[0-9]+\.[0-9]$"
for line in $test
do
	if [[ $line =~ $expected ]]
	then
		continue
	else
		result="Fails smoketest"
	fi
done

echo "$result"