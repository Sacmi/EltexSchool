#!/bin/bash

echo "# Отчет" > Output.md

make

echo "Test file" > test.txt

echo -e "## Test file:" >> Output.md
echo "\`\`\`" >> Output.md
cat test.txt >> Output.md
echo "\`\`\`" >> Output.md

echo -e "## Running fork" >> Output.md
echo "\`\`\`" >> Output.md
./fork 1 2 3 4 >> Output.md
echo "\`\`\`" >> Output.md

echo -e "## Running square" >> Output.md
echo "\`\`\`" >> Output.md
./square 1 2 3 4 >> Output.md
echo "\`\`\`" >> Output.md

echo "## Running myshell" >> Output.md
echo "Чтобы выйти из myshell, нажмите Ctrl-C"
./myshell

echo -e "## Running unbuf_writer" >> Output.md
echo "Напишите строку для unbuf_writer:"
./unbuf_writer test.txt

echo -e "## Running unbuf_reader" >> Output.md
echo "\`\`\`" >> Output.md
./unbuf_reader test.txt >> Output.md
echo "\`\`\`" >> Output.md

echo -e "## Test file:" >> Output.md
echo "\`\`\`" >> Output.md
cat test.txt >> Output.md
echo "\`\`\`" >> Output.md

echo -e "## Running buf_writer" >> Output.md
echo "Напишите строку для buf_writer"
./buf_writer test.txt

echo -e "## Running buf_reader" >> Output.md
echo "\`\`\`" >> Output.md
./buf_reader test.txt >> Output.md
echo "\`\`\`" >> Output.md

echo -e "## Running random" >> Output.md
echo "\`\`\`" >> Output.md
./random 5 >> Output.md
echo "\`\`\`" >> Output.md

echo "## Running pipeshell"
echo "Чтобы выйти из pipeshell, нажмите Ctrl-C"
./pipeshell

echo "Для последнего задания нужны права суперпользователя!"

echo -e "## Running fifo_writer in background" >> Output.md
sudo ./fifo_writer | sudo tee -a Output.md &

echo -e "## Running fifo_reader" >> Output.md
echo "\`\`\`" >> Output.md
sudo ./fifo_reader | sudo tee -a Output.md
echo "\`\`\`" >> Output.md

wait

make clean
rm test.txt

echo "Отчет сгенерирован в файле Output.md"
