OUTPUT_FORMAT(binary)

SECTIONS {
  . = 0x1000;

  .text : {
    *(.text.boot)
    *(.text)
  }

  .data : {
    *(.data .data.* .rodata .rodata.*)
  }
}
