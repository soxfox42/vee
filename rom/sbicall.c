#include "sbicall.h"

sbi_ret sbicall0(int eid, int fid) {
    register int a7 __asm__("a7") = eid;
    register int a6 __asm__("a6") = fid;
    register int a0 __asm__("a0");
    register int a1 __asm__("a1");
    asm("ecall"
        : "=r"(a0), "=r"(a1)
        : "r"(a7), "r"(a6)
        : "memory");
    return (sbi_ret){.error = a0, .value = a1};
}

sbi_ret sbicall1(int eid, int fid, int p0) {
    register int a7 __asm__("a7") = eid;
    register int a6 __asm__("a6") = fid;
    register int a0 __asm__("a0") = p0;
    register int a1 __asm__("a1");
    asm("ecall"
        : "=r"(a0), "=r"(a1)
        : "r"(a7), "r"(a6), "r"(a0)
        : "memory");
    return (sbi_ret){.error = a0, .value = a1};
}

sbi_ret sbicall2(int eid, int fid, int p0, int p1) {
    register int a7 __asm__("a7") = eid;
    register int a6 __asm__("a6") = fid;
    register int a0 __asm__("a0") = p0;
    register int a1 __asm__("a1") = p1;
    asm("ecall"
        : "=r"(a0), "=r"(a1)
        : "r"(a7), "r"(a6), "r"(a0), "r"(a1)
        : "memory");
    return (sbi_ret){.error = a0, .value = a1};
}

sbi_ret sbicall3(int eid, int fid, int p0, int p1, int p2) {
    register int a7 __asm__("a7") = eid;
    register int a6 __asm__("a6") = fid;
    register int a0 __asm__("a0") = p0;
    register int a1 __asm__("a1") = p1;
    register int a2 __asm__("a2") = p2;
    asm("ecall"
        : "=r"(a0), "=r"(a1)
        : "r"(a7), "r"(a6), "r"(a0), "r"(a1), "r"(a2)
        : "memory");
    return (sbi_ret){.error = a0, .value = a1};
}

sbi_ret sbicall4(int eid, int fid, int p0, int p1, int p2, int p3) {
    register int a7 __asm__("a7") = eid;
    register int a6 __asm__("a6") = fid;
    register int a0 __asm__("a0") = p0;
    register int a1 __asm__("a1") = p1;
    register int a2 __asm__("a2") = p2;
    register int a3 __asm__("a3") = p3;
    asm("ecall"
        : "=r"(a0), "=r"(a1)
        : "r"(a7), "r"(a6), "r"(a0), "r"(a1), "r"(a2), "r"(a3)
        : "memory");
    return (sbi_ret){.error = a0, .value = a1};
}

sbi_ret sbicall5(int eid, int fid, int p0, int p1, int p2, int p3, int p4) {
    register int a7 __asm__("a7") = eid;
    register int a6 __asm__("a6") = fid;
    register int a0 __asm__("a0") = p0;
    register int a1 __asm__("a1") = p1;
    register int a2 __asm__("a2") = p2;
    register int a3 __asm__("a3") = p3;
    register int a4 __asm__("a4") = p4;
    asm("ecall"
        : "=r"(a0), "=r"(a1)
        : "r"(a7), "r"(a6), "r"(a0), "r"(a1), "r"(a2), "r"(a3), "r"(a4)
        : "memory");
    return (sbi_ret){.error = a0, .value = a1};
}

sbi_ret sbicall6(int eid, int fid, int p0, int p1, int p2, int p3, int p4, int p5) {
    register int a7 __asm__("a7") = eid;
    register int a6 __asm__("a6") = fid;
    register int a0 __asm__("a0") = p0;
    register int a1 __asm__("a1") = p1;
    register int a2 __asm__("a2") = p2;
    register int a3 __asm__("a3") = p3;
    register int a4 __asm__("a4") = p4;
    register int a5 __asm__("a5") = p5;
    asm("ecall"
        : "=r"(a0), "=r"(a1)
        : "r"(a7), "r"(a6), "r"(a0), "r"(a1), "r"(a2), "r"(a3), "r"(a4), "r"(a5)
        : "memory");
    return (sbi_ret){.error = a0, .value = a1};
}
