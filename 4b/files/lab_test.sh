# #!/bin/bash

# ./lab4b --log=test_output --scale=C & 
# pid=$!
# sleep 10
# kill $pid

# output=$(cat test_output)
# rm -rf test_output

# pass="PASS"
# #regex for each line
# #08:39:17 71.1
# regex="^\d{2}:\d{2}:\d{2} \d+\.\d{1}$"

# for line in $(output)
# do
# 	if [[ $line =~  $regex]]
# 	then
# 		#is true
# 	else
# 		#failed
# 		pass="FAILED"
# 	fi
# done

# echo "$pass"

#!/bin/bash

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
