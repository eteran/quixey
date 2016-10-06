#!/bin/sh

DIR=$(cd "$(dirname "${BASH_SOURCE[0]}" )" && pwd)

for FILE in $(ls $DIR/*.qc); do
	TEST=$(basename $FILE .qc)

	cat $DIR/input/$TEST.txt | $DIR/../build/quixey $DIR/$TEST.qc | diff - $DIR/output/$TEST.txt
	R=$?
	if [ $R -ne 0 ]; then
		echo "$TEST FAILED!"
		exit 1
	fi
done

echo "PASS"





