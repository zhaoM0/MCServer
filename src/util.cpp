#include "util.h"
#include <stdio.h>
#include <stdlib.h>

void 
errif(bool cond, const char *errmsg)
{
    if(cond) {
        perror(errmsg);
        exit(EXIT_FAILURE);
    }
}