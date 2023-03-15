#!/bin/bash

if [ $# -ne 2 ]; then
	echo "Использование: $0 <рингтон> <время>"
	exit 1
fi

MP3_FILE=$1
TIME=$2

echo "mpv $MP3_FILE" | at $TIME

