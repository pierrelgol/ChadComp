#include <stdio.h>

int main() {
    int x = 10;
    int y = 20;
    int sum = x + y;

    if (sum > 15) {
        printf("The sum is greater than 15.\n");
    } else {
        printf("The sum is not greater than 15.\n");
    }

    for (int i = 0; i < 5; i++) {
        printf("Iteration %d\n", i);
    }

    return 0;
}
