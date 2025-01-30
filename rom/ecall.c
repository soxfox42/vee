#include "ecall.h"

int ecall0(int id) {
    register int a7 __asm__("a7") = id;
    register int a0 __asm__("a0");
    asm("ecall"
        : "=r"(a0)
        : "r"(a7)
        : "memory");
    return a0;
}

int ecall1(int id, int p0) {
    register int a7 __asm__("a7") = id;
    register int a0 __asm__("a0") = p0;
    asm("ecall"
        : "=r"(a0)
        : "r"(a7), "r"(a0)
        : "memory");
    return a0;
}

int ecall2(int id, int p0, int p1) {
    register int a7 __asm__("a7") = id;
    register int a0 __asm__("a0") = p0;
    register int a1 __asm__("a1") = p1;
    asm("ecall"
        : "=r"(a0)
        : "r"(a7), "r"(a0), "r"(a1)
        : "memory");
    return a0;
}

int ecall3(int id, int p0, int p1, int p2) {
    register int a7 __asm__("a7") = id;
    register int a0 __asm__("a0") = p0;
    register int a1 __asm__("a1") = p1;
    register int a2 __asm__("a2") = p2;
    asm("ecall"
        : "=r"(a0)
        : "r"(a7), "r"(a0), "r"(a1), "r"(a2)
        : "memory");
    return a0;
}

int ecall4(int id, int p0, int p1, int p2, int p3) {
    register int a7 __asm__("a7") = id;
    register int a0 __asm__("a0") = p0;
    register int a1 __asm__("a1") = p1;
    register int a2 __asm__("a2") = p2;
    register int a3 __asm__("a3") = p3;
    asm("ecall"
        : "=r"(a0)
        : "r"(a7), "r"(a0), "r"(a1), "r"(a2), "r"(a3)
        : "memory");
    return a0;
}

int ecall5(int id, int p0, int p1, int p2, int p3, int p4) {
    register int a7 __asm__("a7") = id;
    register int a0 __asm__("a0") = p0;
    register int a1 __asm__("a1") = p1;
    register int a2 __asm__("a2") = p2;
    register int a3 __asm__("a3") = p3;
    register int a4 __asm__("a4") = p4;
    asm("ecall"
        : "=r"(a0)
        : "r"(a7), "r"(a0), "r"(a1), "r"(a2), "r"(a3), "r"(a4)
        : "memory");
    return a0;
}

int ecall6(int id, int p0, int p1, int p2, int p3, int p4, int p5) {
    register int a7 __asm__("a7") = id;
    register int a0 __asm__("a0") = p0;
    register int a1 __asm__("a1") = p1;
    register int a2 __asm__("a2") = p2;
    register int a3 __asm__("a3") = p3;
    register int a4 __asm__("a4") = p4;
    register int a5 __asm__("a5") = p5;
    asm("ecall"
        : "=r"(a0)
        : "r"(a7), "r"(a0), "r"(a1), "r"(a2), "r"(a3), "r"(a4), "r"(a5)
        : "memory");
    return a0;
}

int ecall7(int id, int p0, int p1, int p2, int p3, int p4, int p5, int p6) {
    register int a7 __asm__("a7") = id;
    register int a0 __asm__("a0") = p0;
    register int a1 __asm__("a1") = p1;
    register int a2 __asm__("a2") = p2;
    register int a3 __asm__("a3") = p3;
    register int a4 __asm__("a4") = p4;
    register int a5 __asm__("a5") = p5;
    register int a6 __asm__("a6") = p6;
    asm("ecall"
        : "=r"(a0)
        : "r"(a7), "r"(a0), "r"(a1), "r"(a2), "r"(a3), "r"(a4), "r"(a5), "r"(a6)
        : "memory");
    return a0;
}
