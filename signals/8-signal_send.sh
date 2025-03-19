#!/bin/bash
# sends the signal SIGQUIT to a process, given its PID

if [ $1 -ne 1 ]; then
	echo "Usage: $0 <pid>"
	exit 1
fi

if [[ $1 =~ ^[0-9]+$ ]]; then
	kill -3 $1
	exit 0
else
	exit 1
fi
