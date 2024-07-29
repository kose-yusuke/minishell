#ifndef DEBUG_FREE_H
#define DEBUG_FREE_H

#include <stdio.h>
#include <stdlib.h>

void my_free(void **ptr, const char *file, int line);

#define free(ptr) my_free(ptr, __FILE__, __LINE__)

#endif // DEBUG_FREE_H
