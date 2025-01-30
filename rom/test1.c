#include "core.h"

const char* str = "Hello, world! I'm testing with a really long string because why the hell not?\n";

void start(void) {
    for (const char *c = str; *c; c++) {
        put_char(*c);
    }
}
