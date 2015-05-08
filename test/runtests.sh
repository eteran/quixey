#!/bin/sh

for FILE in $(ls *.qc); do
	TEST=$(basename $FILE .qc)

	cat input/$TEST.txt | ../quixey $TEST.qc | diff - output/$TEST.txt 1>/dev/null
	R=$?
	if [ $R -ne 0 ]; then
		echo "$TEST FAILED!"
		exit 1
	fi
done

echo "PASS"





