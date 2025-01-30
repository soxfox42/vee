#include "core.h"


int op(int a, int b) {
    return a ^ b;
}

void start(void) {
    put_num(op(42, 7));
}

