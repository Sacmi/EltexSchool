#include <stdio.h>

#define CAPACITY_1 8
#define CAPACITY_2 5
#define CAPACITY_3 3

#define REQUIRED 4

#define LOOP_LIMIT 100000

//#define CAPACITY_1 16
//#define CAPACITY_2 11
//#define CAPACITY_3 6
//
//#define REQUIRED 8

typedef struct {
    int capacity;
    int currentValue;
} Container;

void transfer(Container *from, Container *to) {
    int remained = to->capacity - to->currentValue;
    int liters = remained > from->currentValue ? from->currentValue : remained;

    from->currentValue -= liters;
    to->currentValue += liters;
}

int main() {
    Container biggest = {.capacity = CAPACITY_1, .currentValue = CAPACITY_1};
    Container medium = {.capacity = CAPACITY_2, .currentValue = 0};
    Container smallest = {.capacity = CAPACITY_3, .currentValue = 0};
    size_t counter = 0;

    while (counter < LOOP_LIMIT && medium.currentValue != REQUIRED && smallest.currentValue != REQUIRED) {
        transfer(&biggest, &smallest);

        if (biggest.currentValue == REQUIRED)
            break;

        transfer(&smallest, &medium);
        transfer(&biggest, &smallest);
        transfer(&smallest, &medium);
        transfer(&medium, &biggest);;
        transfer(&smallest, &medium);

        counter++;
    }

    if (counter == LOOP_LIMIT) {
        fprintf(stderr, "ERROR: Скорее всего данная задача не имеет решения");
    } else {
        printf("1 - %d | 2 - %d | 3 - %d\n", biggest.currentValue, medium.currentValue, smallest.currentValue);
    }
}
