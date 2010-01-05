#!/bin/sh
PATH=/bin:$PATH
export PATH

DIFFS=`git diff|wc -l`
if [ $DIFFS -eq 0 ]
then
	git log|head -n 1 > $1/commit.txt
else
	echo "Working copy" > $1/commit.txt
fi
