#pragma once

#include <stddef.h>
#include <string.h>
#include <malloc.h>

typedef char *otiostr;

inline otiostr otiostr_create(const char *init){
    size_t initLen = (init == NULL) ? 0 : strlen(init);

    otiostr s = (char *) malloc((initLen + 1) * sizeof(char));
    if (initLen && init)
        memcpy(s, init, initLen);
    s[initLen] = '\0';
    return s;
}

inline void otiostr_delete(otiostr s){
    if (s == NULL)return;
    free(s);
    s = NULL;
}