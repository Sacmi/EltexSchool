#include <stdio.h>

#define CAPACITY_1 8
#define CAPACITY_2 5
#define CAPACITY_3 3

#define REQUIRED 1

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

    while (medium.currentValue != REQUIRED && smallest.currentValue != REQUIRED) {
        transfer(&biggest, &smallest);

        if (biggest.currentValue == REQUIRED){
            printf("hello\n");
            break;
        }

        transfer(&smallest, &medium);
        transfer(&biggest, &smallest);
        transfer(&smallest, &medium);
        transfer(&medium, &biggest);;
        transfer(&smallest, &medium);
    }

    printf("1 - %d | 2 - %d | 3 - %d\n", biggest.currentValue, medium.currentValue, smallest.currentValue);
}
