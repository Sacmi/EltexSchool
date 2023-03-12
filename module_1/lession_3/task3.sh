#!/bin/bash

if [ $# -ne 2 ]; then
	echo "Использование: $0 <имя_процесса> <периодичность>"
	exit 1
fi

PROCESS_NAME=$1
PERIOD_SECS=$2

while true
do
	if pgrep $PROCESS_NAME > /dev/null
	then
		sleep $PERIOD_SECS
	else
		echo "$PROCESS_NAME был остановлен. Запускаю его снова..."
		$PROCESS_NAME &
	fi
done

