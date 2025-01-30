#pragma once

int ecall0(int id);
int ecall1(int id, int p0);
int ecall2(int id, int p0, int p1);
int ecall3(int id, int p0, int p1, int p2);
int ecall4(int id, int p0, int p1, int p2, int p3);
int ecall5(int id, int p0, int p1, int p2, int p3, int p4);
int ecall6(int id, int p0, int p1, int p2, int p3, int p4, int p5);
int ecall7(int id, int p0, int p1, int p2, int p3, int p4, int p5, int p6);

#define ECALL_NAME(a, b, c, d, e, f, g, n, ...) ecall##n
#define ECALL(id, ...) ECALL_NAME(__VA_ARGS__ __VA_OPT__(,) 7, 6, 5, 4, 3, 2, 1, 0)(id __VA_OPT__(,) __VA_ARGS__)
