#!/usr/bin/bash

for (( i = 1; i <= 35; i++ ))
do
	let prevToken=i+1000
	let newToken=257+i
	echo $i 
	echo $prevToken
	echo $newToken
	find . -regex ".*" -exec sed -i "s/$prevToken /$newToken /g" {} \;
done 
