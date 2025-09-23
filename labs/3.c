#include <stdio.h>

union Swap {
    unsigned long long val;
    char array[8];
};

void encrypt(union Swap *swap) {
    for (int i = 1; i < 8; i+= 2) {
        char transfer = swap->array[i-1];
        swap->array[i-1] = swap->array[i];
        swap->array[i] = transfer;
    }
}

int main() {
    union Swap swap;
    printf("enter a longlong value to encrypt: ");
    scanf("%llu", &swap.val);
    printf("pure value: %llu\n", swap.val);

    encrypt(&swap);

    printf("after encryption: %llu\n", swap.val);

    encrypt(&swap);

    printf("after decryption vice versa: %llu\n", swap.val);
}
