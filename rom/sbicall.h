#pragma once

typedef struct {
    int error;
    int value;
} sbi_ret;

sbi_ret sbicall0(int eid, int fid);
sbi_ret sbicall1(int eid, int fid, int p0);
sbi_ret sbicall2(int eid, int fid, int p0, int p1);
sbi_ret sbicall3(int eid, int fid, int p0, int p1, int p2);
sbi_ret sbicall4(int eid, int fid, int p0, int p1, int p2, int p3);
sbi_ret sbicall5(int eid, int fid, int p0, int p1, int p2, int p3, int p4);
sbi_ret sbicall6(int eid, int fid, int p0, int p1, int p2, int p3, int p4, int p5);

#define SBICALL_NAME(a, b, c, d, e, f, n, ...) sbicall##n
#define SBICALL(eid, fid, ...) SBICALL_NAME(__VA_ARGS__ __VA_OPT__(,) 6, 5, 4, 3, 2, 1, 0)(eid, fid, ##__VA_ARGS__)
