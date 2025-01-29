#include "sbicall.h"

void put_char(char ch) {
    SBICALL(1, 0, ch);
}

__attribute__((used)) void shutdown(void) {
    SBICALL(8, 0);
}

const char* str = "Hello, world! I'm testing with a really long string because why the hell not?\n";

__attribute__((used)) void start(void) {
    for (const char *c = str; *c; c++) {
        put_char(*c);
    }
}

__attribute__((section(".text.boot"), naked, used)) void boot(void) {
    __asm__("jal start\n\tj shutdown");
}
