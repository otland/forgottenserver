#!/bin/bash

modified=$(git ls-files --modified ..)

if [[ $modified ]]; then
	echo "The following files are not following a guideline:"
	echo $modified
	exit -1
else
	exit 0
fi
