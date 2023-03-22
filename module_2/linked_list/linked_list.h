#pragma once

#include <stdlib.h>

#define PRINT_DELIMITER " -> "

typedef struct Node {
    int value;
    struct Node *next;
} Node;

typedef struct {
    Node *head;
    Node *tail;
} LinkedList;

size_t getLength(const LinkedList *list);

int getElement(int index, const LinkedList *list);

void pushElement(int element, LinkedList *list);

void insertElement(int index, int element, LinkedList *list);

void popElement(int index, LinkedList *list);

void editElement(int index, int changedValue, const LinkedList *list);

void printList(const LinkedList *list);

void generateList(size_t size, LinkedList *list);

void cleanList(LinkedList *list);

// ---

LinkedList *combineLists(const LinkedList *left, const LinkedList *right);

LinkedList *intersectArrays(const LinkedList *left, const LinkedList *right);

LinkedList *differenceList(const LinkedList *left, const LinkedList *right);

// ---

void sortUp(LinkedList *list);

void sortDown(LinkedList *list);
