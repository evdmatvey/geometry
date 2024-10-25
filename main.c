#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct circle {
    double x;
    double y;
    double r;
};

void throwError(const char* errorMessage, const char* line, int at)
{
    printf("\n%s\n", line);

    for (int i = 0; i < at; i++) {
        printf(" ");
    }

    printf("^\n%s\n\n", errorMessage);
    exit(1);
}

int isValidNumber(const char* str)
{
    char* endptr;
    strtod(str, &endptr);
    return (errno == 0 && *endptr == '\0');
}

void checkNumbers(const char* line)
{
    const char* delimiters = " \t(),";
    char* input = strdup(line);
    char* current = strtok(input, delimiters);

    if (strcmp(current, "circle") != 0) {
        free(input);
    }

    current = strtok(NULL, delimiters);
    if (!isValidNumber(current)) {
        char* ptr = strstr(line, current);
        free(input);
        throwError("Error. Invalid 'x' value", line, ptr - line);
    }

    current = strtok(NULL, delimiters);
    if (!isValidNumber(current)) {
        char* ptr = strstr(line, current);
        free(input);
        throwError("Error. Invalid 'y' value", line, ptr - line);
    }

    current = strtok(NULL, delimiters);
    if (!isValidNumber(current)) {
        char* ptr = strstr(line, current);
        free(input);
        throwError("Error. Invalid 'z' value", line, ptr - line);
    }

    free(input);
}

void checkFigureName(const char* line, int length)
{
    const char* expectedName = "circle";
    char lineName[7];

    strncpy(lineName, line, length);

    if (strcmp(expectedName, lineName) != 0) {
        throwError("Error: expected 'circle'", line, 0);
    }
}

int checkBrackets(const char* line)
{
    int open = 0, close = 0, firstOpenIndex = 0, len = strlen(line);

    for (int i = 0; i < len; i++) {
        if (line[i] == '(') {
            if (open == 0)
                firstOpenIndex = i;
            open++;

            if (open == 2)
                throwError("Error: expected only one '(' in line", line, i);
        }

        if (line[i] == ')') {
            close++;

            if (line[i + 1] != '\n' && line[i + 1] != '\0')
                throwError("Error: unexpected token", line, i + 1);
        }
    }

    if (open == 0)
        throwError("Error: expected '(' after figure name", line, 6);

    if (close == 0)
        throwError("Error: expected ')' after radius value", line, len);

    return firstOpenIndex;
}

void processCircleString(const char* line)
{
    int figureNameLength = checkBrackets(line);
    checkFigureName(line, figureNameLength);
    checkNumbers(line);
}

void readCirclesFromFile(
        const char* filename, struct circle** circles, int* count)
{
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening the file");
        return;
    }

    *circles = (struct circle*)malloc(100 * sizeof(struct circle));
    int capacity = 100;
    *count = 0;

    char line[100];
    while (fgets(line, sizeof(line), file)) {
        processCircleString(line);

        double x, y, r;
        if (sscanf(line, "circle(%lf %lf, %lf)", &x, &y, &r) == 3) {
            if (*count == capacity) {
                capacity *= 2;
                *circles = (struct circle*)realloc(
                        *circles, capacity * sizeof(struct circle));
            }
            (*circles)[*count].x = x;
            (*circles)[*count].y = y;
            (*circles)[*count].r = r;
            (*count)++;
        }
    }

    fclose(file);
}

void printCircles(struct circle* circles, int* count)
{
    printf("\n");
    for (int i = 0; i < *count; i++) {
        printf("%d. circle(%g %g, %g)\n\n",
               i + 1,
               circles[i].x,
               circles[i].y,
               circles[i].r);
    }
}

int main(int argc, char* argv[])
{
    struct circle* circles
            = (struct circle*)malloc(100 * sizeof(struct circle));
    int count = 0;

    readCirclesFromFile(argv[1], &circles, &count);
    printCircles(circles, &count);
    free(circles);
    return 0;
}
