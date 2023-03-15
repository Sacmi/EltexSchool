#!/bin/bash

FILE="/tmp/counter.log"

touch $FILE

COUNT=$(wc -l $FILE | awk '{print $1}')
COUNT=$((COUNT+1))

echo "$(date) I run $COUNT times" >> $FILE

at now + 1 minute -f task5.sh
