#include <stdio.h>
#include <assert.h>
#include "linked_list.h"

#define TODO assert(0 && "Not Implemented")
#define SHRINK_FACTOR 1.247
#define RANDOM_RANGE 100

LinkedList *createEmptyList() {
    LinkedList *list = malloc(sizeof(LinkedList));
    list->head = NULL;
    list->tail = NULL;

    return list;
}

void pushElement(int element, LinkedList *list) {
    Node *newNode = (Node *) malloc(sizeof(Node));
    newNode->value = element;
    newNode->next = NULL;

    if (list->tail == NULL) {
        list->head = newNode;
        list->tail = newNode;
    } else {
        list->tail->next = newNode;
        list->tail = newNode;
    }
}

size_t getLength(const LinkedList *list) {
    size_t length = 0;
    const Node *currentNode = list->head;

    while (currentNode != NULL) {
        length++;
        currentNode = currentNode->next;
    }

    return length;
}

size_t calculatePosition(int index, const LinkedList *list) {
    size_t length = getLength(list);

    if (abs(index) > length) {
        fprintf(stderr, "ERROR: out of range\n");
        exit(1);
    }

    return index >= 0 ? index : length + index;
}

void insertElement(int index, int element, LinkedList *list) {
    Node *newNode = malloc(sizeof(Node));
    newNode->value = element;
    size_t position = calculatePosition(index, list);

    if (position == 0) {
        newNode->next = list->head;
        list->head = newNode;

        return;
    }

    Node *currentNode = list->head;

    for (int i = 1; i < position; ++i) {
        currentNode = currentNode->next;
    }

    newNode->next = currentNode->next;
    currentNode->next = newNode;
}

void popHead(LinkedList *list) {
    Node *head = list->head;
    list->head = head->next;
    free(head);
}

Node *getNode(size_t index, const LinkedList *list) {
    Node *currentNode = list->head;

    for (size_t i = 0; i < index; ++i) {
        currentNode = currentNode->next;
    }

    return currentNode;
}

void popElement(int index, LinkedList *list) {
    size_t position = calculatePosition(index, list);

    if (position == 0) {
        return popHead(list);
    }

    assert(position >= 1 && "Out of range");

    Node *node = getNode(position - 1, list);
    Node *rNode = node->next;
    node->next = rNode->next;

    free(rNode);
}

int getElement(int index, const LinkedList *list) {
    size_t position = calculatePosition(index, list);
    Node *node = getNode(position, list);

    return node->value;
}

void editElement(int index, int changedValue, const LinkedList *list) {
    size_t position = calculatePosition(index, list);
    Node *node = getNode(position, list);

    node->value = changedValue;
}

void printList(const LinkedList *list) {
    const Node *currentNode = list->head;

    if (currentNode == NULL) {
        printf("List is empty\n");
        return;
    }

    while (1) {
        if (currentNode->next == NULL) {
            printf("%d\n", currentNode->value);
            return;
        } else {
            printf("%d%s", currentNode->value, PRINT_DELIMITER);
            currentNode = currentNode->next;
        }
    }
}

void generateList(size_t size, LinkedList *list) {
    for (size_t i = 0; i < size; ++i) {
        pushElement(rand() % RANDOM_RANGE, list);
    }
}

void cleanList(LinkedList *list) {
    Node *currentNode = list->head;

    while (currentNode != NULL) {
        Node *nextNode = currentNode->next;
        free(currentNode);
        currentNode = nextNode;
    }

    list->head = NULL;
    list->tail = NULL;
}

LinkedList *combineLists(const LinkedList *left, const LinkedList *right) {
    LinkedList *newList = createEmptyList();
    Node *currentNodeLeft = left->head;

    while (currentNodeLeft != NULL) {
        pushElement(currentNodeLeft->value, newList);
        currentNodeLeft = currentNodeLeft->next;
    }

    Node *currentNodeRight = right->head;

    while (currentNodeRight != NULL) {
        pushElement(currentNodeRight->value, newList);
        currentNodeRight = currentNodeRight->next;
    }

    return newList;
}

LinkedList *intersectArrays(const LinkedList *left, const LinkedList *right) {
    LinkedList *newList = createEmptyList();
    Node *currentNodeLeft = left->head;

    while (currentNodeLeft != NULL) {
        Node *currentNodeRight = right->head;

        while (currentNodeRight != NULL) {
            if (currentNodeLeft->value == currentNodeRight->value) {
                pushElement(currentNodeLeft->value, newList);
            }

            currentNodeRight = currentNodeRight->next;
        }

        currentNodeLeft = currentNodeLeft->next;
    }

    return newList;
}

LinkedList *differenceList(const LinkedList *left, const LinkedList *right) {
    LinkedList *newList = createEmptyList();
    Node *currentNodeLeft = left->head;

    while (currentNodeLeft != NULL) {
        Node *currentNodeRight = right->head;
        int isFound = 0;

        while (currentNodeRight != NULL) {
            if (currentNodeLeft->value == currentNodeRight->value) {
                isFound = 1;
                break;
            }

            currentNodeRight = currentNodeRight->next;
        }

        if (!isFound) {
            pushElement(currentNodeLeft->value, newList);
        }

        currentNodeLeft = currentNodeLeft->next;
    }

    Node *currentNodeRight = right->head;

    while (currentNodeRight != NULL) {
        int isFound = 0;
        currentNodeLeft = left->head;

        while (currentNodeLeft != NULL) {
            if (currentNodeRight->value == currentNodeLeft->value) {
                isFound = 1;
                break;
            }

            currentNodeLeft = currentNodeLeft->next;
        }

        if (!isFound) {
            pushElement(currentNodeRight->value, newList);
        }

        currentNodeRight = currentNodeRight->next;
    }

    return newList;
}

void sortUp(LinkedList *list) {
    Node *currentNode = list->head;

    while (currentNode != NULL) {
        Node *nextNode = currentNode->next;

        while (nextNode != NULL) {
            if (currentNode->value > nextNode->value) {
                int temp = currentNode->value;
                currentNode->value = nextNode->value;
                nextNode->value = temp;
            }

            nextNode = nextNode->next;
        }

        currentNode = currentNode->next;
    }

}

void sortDown(LinkedList *list) {
    size_t gap = getLength(list);
    int sorted = 0;

    while (!sorted) {
        gap = (size_t) (gap / SHRINK_FACTOR);

        if (gap <= 1) {
            gap = 1;
            sorted = 1;
        }

        Node *currentNode = list->head;

        while (currentNode != NULL) {
            Node *nextNode = currentNode->next;

            for (size_t i = 0; i < gap; ++i) {
                if (nextNode == NULL) {
                    break;
                }

                if (currentNode->value < nextNode->value) {
                    int temp = currentNode->value;
                    currentNode->value = nextNode->value;
                    nextNode->value = temp;
                    sorted = 0;
                }

                nextNode = nextNode->next;
            }

            currentNode = currentNode->next;
        }
    }
}
