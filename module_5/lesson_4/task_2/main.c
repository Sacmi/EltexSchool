#include <stdio.h>

float add(float a, float b);
float sub(float a, float b);

int main() {
    float a = 10, b = 5;

    printf("Sum of numbers %f and %f is %f\n", a, b, add(a, b));
    printf("Subtraction from %f of %f is %f\n", a, b, sub(a, b));
}