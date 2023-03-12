#!/bin/bash

if [ $# -ne 3 ]; then
	echo "Использование: $0 <место_расположения_директории> <количество_директорий> <шаблон_имени>"
	exit 1
fi

DIR_LOCATION=$1
NUM_DIRECTORIES=$2
NAME_TEMPLATE=$3

echo "Создаю $NUM_DIRECTORIES папок в $DIR_LOCATION"

for ((i=1; i<=$NUM_DIRECTORIES; i++)); do
	DIR_NAME=$(printf "$NAME_TEMPLATE" "$i")
	DIR_PATH="$DIR_LOCATION/$DIR_NAME"
	mkdir -p $DIR_PATH/child-{0..100}

	echo "Создаю файлы в родительской папке $DIR_PATH..."
	for CHILD_DIR in $DIR_PATH/child-*; 
	do 
		touch $CHILD_DIR/file-{0..200}
  	done
done
