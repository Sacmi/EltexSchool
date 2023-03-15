#!/bin/bash

FILE="alice.txt"

LETTERS=$(grep -oP "[А-я]|\w" $FILE | wc -l)
echo "Количество букв в книге: $LETTERS"

WORDS=$(cat $FILE | wc -w)
echo "Количество слов в книге: $WORDS"

QUEEN_SENTENCE=$(grep -o -m 1 -E '[^\.]*Королев[а-яё]*[^\.]*\.' $FILE)
echo "Первое предложение, в котором упоминается Королева: $QUEEN_SENTENCE"

TEA_PARTY=$(grep -o -E '(Шляпа)|(Заяц)|(Садовая Соня)' $FILE | sort | uniq)
echo "Участники чаепития: $TEA_PARTY"
