with open("ecall.c", "w") as f:
    f.write("#include \"ecall.h\"\n")
    for arg_count in range(8):
        f.write("\n")
        args = ["int id"]
        args += [f"int p{i}" for i in range(arg_count)]
        f.write(f"int ecall{arg_count}({", ".join(args)}) {{\n")
        f.write("    register int a7 __asm__(\"a7\") = id;\n")
        for i in range(arg_count):
            f.write(f"    register int a{i} __asm__(\"a{i}\") = p{i};\n")
        if arg_count < 1:
            f.write("    register int a0 __asm__(\"a0\");\n")
        asm_args = ['"r"(a7)']
        asm_args += [f'"r"(a{i})' for i in range(arg_count)]
        f.write(f"""    asm("ecall"
        : "=r"(a0)
        : {", ".join(asm_args)}
        : "memory");
    return a0;
""")
        f.write("}\n")
