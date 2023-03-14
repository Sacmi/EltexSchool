#!/bin/bash

file="alice.txt"

letters=$(grep -oP "[А-я]|\w" alice.txt | wc -l)
echo "Количество букв в книге: $letters"

words=$(cat alice.txt | wc -w)
echo "Количество слов в книге: $words"

queen_sentence=$(grep -o -m 1 -E '[^\.]*Королев[а-яё]*[^\.]*\.' alice.txt)
echo "Первое предложение, в котором упоминается Королева: $queen_sentence"

tea_party=$(grep -o -E '(Шляпа)|(Заяц)|(Садовая Соня)' alice.txt | sort | uniq)
echo "Участники чаепития: $tea_party"
