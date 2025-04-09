#include "utils.h"

#include <ctype.h>
#include <string.h>
#include <math.h>

u8 is_number(const char* str) {
    u32 length = strlen(str);
    if(!isdigit(str[0]) && str[0] != '-')
        return false;
    for(int i = 1; i < length; i++) {
        if(!isdigit(str[i]))
            return false;
    }
    return true;
}

float square_root(float x) {
    return sqrtf(x); //TODO: write your own
}
