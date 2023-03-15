#!/bin/bash

CONFIG_FILE=".task1.cfg"

# Конфигурация резервного копирования
if [ -f $CONFIG_FILE ]; then
    source $CONFIG_FILE
else
    echo "Выберите способ резервного копирования (git, dd или tar):"
    read METHOD

    echo "Введите что нужно забэкапить:"
    read SOURCE

    echo "Укажите путь для сохранения бэкапа:" 
    read DESTINATION

    echo "Как часто нужно выполнять бэкап? (ежедневно, еженедельно или ежемесячно)"
    read FREQUENCY

    echo "METHOD=$METHOD" > $CONFIG_FILE
    echo "SOURCE=$SOURCE" >> $CONFIG_FILE
    echo "DESTINATION=$DESTINATION" >> $CONFIG_FILE
    echo "FREQUENCY=$FREQUENCY" >> $CONFIG_FILE
fi

# Осуществление резервного копирования в зависимости от выбранного метода
case $METHOD in
    dd)
    # Резервное копирование с помощью dd
        dd if=$SOURCE of=$DESTINATION bs=4M
        ;;
    # Резервное копирование с помощью tar
    tar) 
        tar -cf $DESTINATION $SOURCE
        ;; 
    # Резервное копирование с помощью git
    git)
        git add $SOURCE
        git commit -m "Backup ($(date +%d-%m-%Y))))"
        git push
        ;;

    *)
        echo "Неправильный метод резервного копирования."
        exit 1 
        ;;
esac

# Осуществление резервного копирования в зависимости от выбранной частоты
case $FREQUENCY in
    ежедневно)
        at now + 1 day -f $0
        ;;
    еженедельно)
        at now + 1 week -f $0
        ;;
    ежемесячно)
        at now + 1 month -f $0
        ;;
    *)
        echo "Неправильная частота резервного копирования."
        exit 1
        ;;
esac

