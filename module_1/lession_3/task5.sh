#!/bin/bash

CONFIG_FILE="task5.cfg"

if [ ! -f "$CONFIG_FILE" ]; then
	echo "Не удалось найти файл конфигурации \"$CONFIG_FILE\""
	exit 1
fi

source "$CONFIG_FILE"

grep $PROCESS_NAME $LOG_FILE | awk -v start_date="$START_DATE" -v end_date="$END_DATE" '$0 > start_date && $0 < end_date'
