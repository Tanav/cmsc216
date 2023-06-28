#include <stdio.h>

int valid_character(char symbol) {
    if (symbol == '*' || symbol == '%' || symbol == '#') {
        return 1;
    }

    return 0;
}

int draw_rectangle(char symbol, int width, int length) {
    if (valid_character(symbol) && width > 0 && length > 0) {
        int i, j;
        for (i = 0; i < width; i++) {
            for (j = 0; j < length; j++) {
                printf("%c", symbol);
            }
            printf("\n");
        }
        return 1;
    }

    printf("Invalid data provided.\n");
    return 0;
}

int draw_triangle(char symbol, int size) {
    if (valid_character(symbol) && size > 0) {
        int i, j, k;
        for (i = 1; i <= size; i++) {
            for (j = 1; j <= size - i; j++) {
                printf(" ");
            }
            for (k = 1; k <= (2 * i - 1); k++) {
                printf("%c", symbol);
            }
            printf("\n");
        }
        return 1;
    }

    printf("Invalid data provided.\n");
    return 0;
}

int main() {
    char symbol;
    int option = 100, width, length, size;

    while (option != 0) {
        printf("Enter 1(rectangle), 2(triangle), 3(other), 0(quit): ");
        scanf(" %d", &option);

        if (option == 1) {
            printf("Enter character, width and length: ");
            scanf(" %c %d %d", &symbol, &width, &length);
            draw_rectangle(symbol, width, length);
        } else if (option == 2) {
            printf("Enter character and size: ");
            scanf(" %c %d", &symbol, &size);
            draw_triangle(symbol, size);
        } else if (option != 0) {
            printf("Invalid choice.\n");
        }
    }

    printf("Bye Bye.");
    return 0;
}