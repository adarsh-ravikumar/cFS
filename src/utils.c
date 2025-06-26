#include "utils.h"
#include <stdio.h>
#include <ctype.h>

int ask_yes_no(const char* question) {
    char input;
    printf("%s (y/n): ", question);
    while (1) {
        input = getchar();
        while (getchar() != '\n'); // clear input buffer

        input = tolower(input);
        if (input == 'y') return 1;
        if (input == 'n') return 0;

        printf("Please enter 'y' or 'n': ");
    }
}