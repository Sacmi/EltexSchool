#!/bin/bash

echo "Создание файлов file_a, file_b, file_c"
touch file_a file_b file_c

echo "Создание архива archive.tar"
tar -cvf archive.tar file_a
tar -tvf archive.tar

echo "Добавление файла file_b в архив"
tar -rvf archive.tar file_b
tar -tvf archive.tar

echo "Добавление файла file_c в архив"
tar -rvf archive.tar file_c
tar -tvf archive.tar

echo "Удаление файла file_b из архива"
tar -vf archive.tar --delete file_b
tar -tvf archive.tar

echo "Извлечение файлов из архива"
mkdir extracted
tar -xvf archive.tar -C extracted

echo "Вывод распакованных файлов"
ls extracted