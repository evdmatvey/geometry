#include <libgeometry/utilities.h>

size_t index_of(const char* str, const char* substr)
{
    char* ptr = strstr(str, substr);

    if (ptr != NULL)
        return ptr - str;

    return -1;
}
