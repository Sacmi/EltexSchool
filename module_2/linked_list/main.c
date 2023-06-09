#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "linked_list.h"

int main() {
    srand(time(NULL));

    LinkedList linkedList = {.head = NULL, .tail = NULL};
    generateList(10, &linkedList);

    printList(&linkedList);

    printf("Inserting 888 at index -5\n");
    insertElement(-5, 888, &linkedList);
    printList(&linkedList);

    printf("Adding 200 to the beginning of the list\n");
    insertElement(0, 200, &linkedList);
    printList(&linkedList);

    printf("Removing element at index 0\n");
    popElement(0, &linkedList);
    printList(&linkedList);

    printf("Removing element at index -1\n");
    popElement(-1, &linkedList);
    printList(&linkedList);

    printf("Changing element at index 0 to 777\n");
    editElement(0, 777, &linkedList);
    printList(&linkedList);

    printf("Element at index 0: %d\n", getElement(0, &linkedList));

    printf("Cleaning list... (freeing memory)\n");
    cleanList(&linkedList);
    printList(&linkedList);

    printf("\n --- \n");

    LinkedList left = {.head = NULL, .tail = NULL};
    LinkedList right = {.head = NULL, .tail = NULL};

    generateList(10, &left);
    generateList(10, &right);

    printf("Left list: ");
    sortUp(&left);
    printList(&left);
    printf("Right list: ");
    sortUp(&right);
    printList(&right);

    LinkedList *combined = combineLists(&left, &right);
    printf("Combined list: ");
    sortUp(combined);
    printList(combined);

    LinkedList *intersected = intersectArrays(&left, &right);
    printf("Intersected list: ");
    sortUp(intersected);
    printList(intersected);

    LinkedList *difference = differenceList(&left, &right);
    printf("Difference list: ");
    sortUp(difference);
    printList(difference);

    cleanList(combined);
    cleanList(intersected);
    cleanList(difference);

    free(combined);
    free(intersected);
    free(difference);

    printf("\n --- \n");

    generateList(10, &linkedList);
    printf("Unsorted list: ");
    printList(&linkedList);

    sortUp(&linkedList);
    printf("Sorted up list: ");
    printList(&linkedList);

    sortDown(&linkedList);
    printf("Sorted down list: ");
    printList(&linkedList);

    return 0;
}
