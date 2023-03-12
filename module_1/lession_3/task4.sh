#!/bin/bash

if [ $# -ne 3 ]; then
	echo "Использование: $0 <имя_процесса> <начало_промежутка> <конец_промежутка>"
	echo -e "\nПромежутки должны задаваться в формате - Mon DD".

	exit 1
fi

PROCESS_NAME=$1
START_DATE=$2
END_DATE=$3

grep $PROCESS_NAME /var/log/auth.log | awk -v start_date="$START_DATE" -v end_date="$END_DATE" '$0 > start_date && $0 < end_date' 
