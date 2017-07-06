#!/bin/bash

make clean
make stats

if [ $# -eq 0 ]
then
	echo "Usage: $0 <nframes>"
	exit 1
fi

echo
echo "-------------------------------"
echo "Sort"
echo "-------------------------------"
echo "Rand:"
./virtmem 100 $1 rand sort
echo "Fifo:"
./virtmem 100 $1 fifo sort
echo "Custom:"
./virtmem 100 $1 custom sort

echo
echo "-------------------------------"
echo "Scan"
echo "-------------------------------"
echo "Rand:"
./virtmem 100 $1 rand scan
echo "Fifo:"
./virtmem 100 $1 fifo scan
echo "Custom:"
./virtmem 100 $1 custom scan

echo
echo "-------------------------------"
echo "Focus"
echo "-------------------------------"
echo "Rand:"
./virtmem 100 $1 rand focus
echo "Fifo:"
./virtmem 100 $1 fifo focus
echo "Custom:"
./virtmem 100 $1 custom focus
