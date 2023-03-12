#!/bin/bash

echo "Создаю папки"
mkdir -p dir-{0..50}/child-{0..100}

for PARENT_DIR in dir-*;
do
	echo "Создаю файлы в родительской папке $PARENT_DIR..."

	for CHILD_DIR in $PARENT_DIR/child-*; 
    	do 
        	touch $CHILD_DIR/file-{0..200};
    	done;
done;

# или так
# find dir-* -type d -name 'child-*' -execdir sh -c 'touch file-{0..200}' \;
