#include "core.h"

const char str[] = {
    36,50,54,54,55,16,59,55,57,54,50,22,16,49,58,58,16,52,58,16,57,52,55,58,54,50,16,54,55,55,53,16,56,57,50,58,58,60,16,49,55,57,57,58,56,58,50,50,16,50,58,50,16,58,55,16,58,52,50,16,58,57,58,55,49,48,58,52,55,55,23,
};

void decode(const char *in, char *out) {
    while (*in) {
        *out++ = (*in++) << 1;
    }
}

void print_str(const char *str) {
    while (*str) {
        put_char(*str++);
    }
}

void start(void) {
    char buf[128];
    decode(str, buf);
    print_str(buf);
}
