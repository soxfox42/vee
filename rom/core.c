#include "ecall.h"

void shutdown(void) {
    ECALL(1);
}

void put_char(char ch) {
    ECALL(2, ch);
}

void put_num(unsigned int number) {
    ECALL(3, number);
}

void start(void);
__attribute__((section(".text.boot"), naked, used)) void boot(void) {
    __asm__(
        "li sp, 0x10000\n\t"
        "jal start\n\t"
        "j shutdown"
        :
        : "i"(shutdown), "i"(start)
    );
}
