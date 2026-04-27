#include <stdio.h>
#include <stdlib.h>

int main() {
    FILE *fp = fopen("output.txt", "r");
    if (fp == NULL) {
        perror("Error opening file");
        return EXIT_FAILURE;
    }

    char line[256];
    while (fgets(line, sizeof(line), fp)) {
        printf("%s", line);
    }

    fclose(fp);
    return EXIT_SUCCESS;
}
