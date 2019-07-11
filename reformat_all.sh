#!/bin/bash

CLANG_FORMAT=$(clang-format -version)
if [[ $CLANG_FORMAT == *"8.0"* ]]; then
	echo "C++ code Formatting"
	files=`find . -iname "*.cpp" -print`
	files+=$'\n'
	files+=`find . -iname "*.hpp" -print`

	while read -r line; do
		if [ -f "$line" ]; then
			echo "Formatting $line"
			clang-format -i "$line"
		fi
	done <<< "$files"
else
	echo "clang-format-8.0 is not available. C++ code can't be formatted"
fi

autopep8 --version  &> /dev/null
if [ $? -eq 0 ]; then
	echo "Python code formatting"
	files=`find . -iname *.py`
	while read -r line; do
		if [ -f "$line" ]; then
			echo "Formatting $line"
			autopep8 -i "$line"
		fi
	done <<< "$files"
else
	echo "autopep8 is not available. Python code can't be formatted"
fi
