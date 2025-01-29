with open("sbicall.c", "w") as f:
    f.write("#include \"sbicall.h\"\n")
    for arg_count in range(7):
        f.write("\n")
        args = ["int eid", "int fid"]
        args += [f"int p{i}" for i in range(arg_count)]
        f.write(f"sbi_ret sbicall{arg_count}({", ".join(args)}) {{\n")
        f.write("    register int a7 __asm__(\"a7\") = eid;\n")
        f.write("    register int a6 __asm__(\"a6\") = fid;\n")
        for i in range(arg_count):
            f.write(f"    register int a{i} __asm__(\"a{i}\") = p{i};\n")
        if arg_count < 1:
            f.write("    register int a0 __asm__(\"a0\");\n")
        if arg_count < 2:
            f.write("    register int a1 __asm__(\"a1\");\n")
        asm_args = ['"r"(a7)', '"r"(a6)']
        asm_args += [f'"r"(a{i})' for i in range(arg_count)]
        f.write(f"""    asm("ecall"
        : "=r"(a0), "=r"(a1)
        : {", ".join(asm_args)}
        : "memory");
    return (sbi_ret){{.error = a0, .value = a1}};
""")
        f.write("}\n")
