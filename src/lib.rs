use std::{
    fmt::{self, Display, Formatter},
    io::{self, Write},
};

pub struct Vee {
    cpu: Cpu,
    ram: Vec<u8>,
}

const DEBUG: bool = false;

const PROGRAM_BASE: usize = 0x1000;
const RAM_SIZE: usize = 0x10000;

const TICK_STEPS: usize = 1000;

impl Vee {
    pub fn new(program: &[u8]) -> Self {
        assert!(program.len() < RAM_SIZE - PROGRAM_BASE);

        let mut ram = Vec::with_capacity(RAM_SIZE);
        ram.resize(PROGRAM_BASE, 0);
        ram.extend_from_slice(program);
        ram.resize(RAM_SIZE, 0);

        Self {
            cpu: Cpu::new(),
            ram,
        }
    }

    pub fn tick(&mut self) {
        for _ in 0..TICK_STEPS {
            if !self.cpu.running {
                break;
            }
            self.cpu.step(&mut self.ram);
            if DEBUG {
                println!("{}", self.cpu);
            }
        }
    }
}

const OPCODE_SIZE: u32 = 7;
const OPCODE_SHIFT: u32 = 0;
const OPCODE_MASK: u32 = (1 << OPCODE_SIZE) - 1 << OPCODE_SHIFT;

const RD_SIZE: u32 = 5;
const RD_SHIFT: u32 = 7;
const RD_MASK: u32 = (1 << RD_SIZE) - 1 << RD_SHIFT;

const FUNCT3_SIZE: u32 = 3;
const FUNCT3_SHIFT: u32 = 12;
const FUNCT3_MASK: u32 = (1 << FUNCT3_SIZE) - 1 << FUNCT3_SHIFT;

const RS1_SIZE: u32 = 5;
const RS1_SHIFT: u32 = 15;
const RS1_MASK: u32 = (1 << RS1_SIZE) - 1 << RS1_SHIFT;

const RS2_SIZE: u32 = 5;
const RS2_SHIFT: u32 = 20;
const RS2_MASK: u32 = (1 << RS2_SIZE) - 1 << RS2_SHIFT;

struct Cpu {
    running: bool,
    pc: u32,
    regs: [u32; 32],
}

impl Cpu {
    fn new() -> Self {
        Self {
            running: true,
            pc: PROGRAM_BASE as u32,
            regs: [0; 32],
        }
    }

    fn read_reg(&mut self, reg: u32) -> u32 {
        if reg > 0 && reg < 32 {
            self.regs[reg as usize]
        } else {
            0
        }
    }

    fn write_reg(&mut self, reg: u32, value: u32) {
        if reg > 0 && reg < 32 {
            self.regs[reg as usize] = value;
        }
    }

    fn fetch(&mut self, ram: &[u8]) -> u32 {
        let pc = self.pc as usize;
        let value = u32::from_le_bytes(ram[pc..pc + 4].try_into().unwrap());
        if DEBUG {
            println!("Fetched {value:08X}");
        }
        self.pc = self.pc.wrapping_add(4);
        value
    }

    fn sign_extend(value: u32, bits: u32) -> u32 {
        if value & (1 << bits - 1) == 0 {
            value
        } else {
            let shift = 32 - bits;
            ((value as i32) << shift >> shift) as u32
        }
    }

    fn i_type_imm(inst: u32) -> u32 {
        let value = (inst >> 20) & 0xFFF;
        Self::sign_extend(value, 12)
    }

    fn s_type_imm(inst: u32) -> u32 {
        let value = (inst >> 7) & 0x1F | (inst >> 20) & 0xFE0;
        Self::sign_extend(value, 12)
    }

    // Ew. This is actually good in hardware, it *simplifies* the decoder. Not here.
    fn b_type_imm(inst: u32) -> u32 {
        let value =
            (inst >> 7) & 0x1E | (inst >> 20) & 0x7E0 | (inst << 4) & 0x800 | (inst >> 19) & 0x1000;
        Self::sign_extend(value, 13)
    }

    // This one's super simple, but I'm keeping it for symmetry.
    fn u_type_imm(inst: u32) -> u32 {
        inst & 0xFFFFF000
    }

    // See b_type_imm.
    fn j_type_imm(inst: u32) -> u32 {
        let value =
            (inst >> 20) & 0x7FE | (inst >> 9) & 0x800 | inst & 0xFF000 | (inst >> 11) & 0x100000;
        Self::sign_extend(value, 21)
    }

    fn step(&mut self, ram: &mut [u8]) {
        assert!(self.pc & 3 == 0, "Misaligned PC");
        let inst = self.fetch(ram);
        let opcode = (inst & OPCODE_MASK) >> OPCODE_SHIFT;
        match opcode {
            0b0000011 => self.step_load(inst, ram),
            0b0010011 => self.step_op_imm(inst),
            0b0100011 => self.step_store(inst, ram),
            0b0110111 => self.step_lui(inst),
            0b1100011 => self.step_branch(inst),
            0b1100111 => self.step_jalr(inst),
            0b1101111 => self.step_jal(inst),
            0b1110011 => self.step_system(inst),
            _ => panic!("Unsupported instruction {inst:08X}"),
        }
    }

    fn step_load(&mut self, inst: u32, ram: &[u8]) {
        let rd = (inst & RD_MASK) >> RD_SHIFT;
        let rs = (inst & RS1_MASK) >> RS1_SHIFT;
        let funct3 = (inst & FUNCT3_MASK) >> FUNCT3_SHIFT;
        let imm = Self::i_type_imm(inst);

        let addr = self.read_reg(rs).wrapping_add(imm) as usize;

        let value = match funct3 {
            0b100 => ram[addr] as u32,
            _ => panic!("Unsupported LOAD instruction {inst:08X}"),
        };
        self.write_reg(rd, value);
    }

    fn step_op_imm(&mut self, inst: u32) {
        let rd = (inst & RD_MASK) >> RD_SHIFT;
        let rs = (inst & RS1_MASK) >> RS1_SHIFT;
        let funct3 = (inst & FUNCT3_MASK) >> FUNCT3_SHIFT;
        let imm = Self::i_type_imm(inst);

        let input = self.read_reg(rs);
        let output = match funct3 {
            0b000 => input.wrapping_add(imm),
            0b001 => input.wrapping_shl(imm),
            0b010 => (input < imm) as u32,
            0b011 => ((input as i32) < imm as i32) as u32,
            0b100 => input ^ imm,
            0b101 => {
                if imm & 0x400 == 0 {
                    input.wrapping_shr(imm)
                } else {
                    ((input as i32) >> (imm & 0x1F)) as u32
                }
            }
            0b110 => input | imm,
            0b111 => input & imm,
            _ => unreachable!(),
        };
        self.write_reg(rd, output);
    }

    fn step_store(&mut self, inst: u32, ram: &mut [u8]) {
        let rs1 = (inst & RS1_MASK) >> RS1_SHIFT;
        let rs2 = (inst & RS2_MASK) >> RS2_SHIFT;
        let funct3 = (inst & FUNCT3_MASK) >> FUNCT3_SHIFT;
        let imm = Self::s_type_imm(inst);

        let addr = self.read_reg(rs1).wrapping_add(imm) as usize;
        let value = self.read_reg(rs2);

        match funct3 {
            0b000 => ram[addr as usize] = (value & 0xFF) as u8,
            _ => panic!("Unsupported STORE instruction {inst:08X}"),
        };
    }

    fn step_lui(&mut self, inst: u32) {
        let rd = (inst & RD_MASK) >> RD_SHIFT;
        let imm = Self::u_type_imm(inst);
        self.write_reg(rd, imm);
    }

    fn step_branch(&mut self, inst: u32) {
        let rs1 = (inst & RS1_MASK) >> RS1_SHIFT;
        let rs2 = (inst & RS2_MASK) >> RS2_SHIFT;
        let addr = Self::b_type_imm(inst);
        let funct3 = (inst & FUNCT3_MASK) >> FUNCT3_SHIFT;
        let a = self.read_reg(rs1);
        let b = self.read_reg(rs2);
        let cond = match funct3 {
            0b000 => a == b,
            0b001 => a != b,
            _ => panic!("Unsupported BRANCH instruction {inst:08X}"),
        };
        if cond {
            self.pc = self.pc.wrapping_add(addr - 4);
        }
    }

    fn step_jalr(&mut self, inst: u32) {
        let funct3 = (inst & FUNCT3_MASK) >> FUNCT3_SHIFT;
        if funct3 != 0 {
            panic!("Unsupported JALR instruction {inst:08X}");
        }

        let rd = (inst & RD_MASK) >> RD_SHIFT;
        let rs = (inst & RS1_MASK) >> RS1_SHIFT;
        let mut offset = (inst & 0xFFF00000) >> 20;
        if offset & 0x800 != 0 {
            offset |= 0xFFFFF000;
        }
        let base = self.read_reg(rs);
        let addr = base.wrapping_add(offset);
        self.write_reg(rd, self.pc);
        self.pc = addr;
    }

    fn step_jal(&mut self, inst: u32) {
        let rd = (inst & RD_MASK) >> RD_SHIFT;
        let addr = Self::j_type_imm(inst);
        self.write_reg(rd, self.pc);
        self.pc = self.pc.wrapping_add(addr - 4);
    }

    fn step_system(&mut self, inst: u32) {
        match inst {
            0x00000073 => {
                let eid = self.regs[17];
                match eid {
                    0x01 => {
                        let ch = self.regs[10];
                        io::stdout().write_all(&[(ch & 0xFF) as u8]).unwrap();
                    }
                    0x08 => {
                        self.running = false;
                    }
                    _ => panic!("Unsupported ECALL EID {eid:08X}"),
                }
            }
            _ => panic!("Unsupported SYSTEM instruction {inst:08X}"),
        }
    }
}

const ABI_LABELS: [&str; 32] = [
    "zero", "ra", "sp", "gp", "tp", "t0", "t1", "t2", "s0", "s1", "a0", "a1", "a2", "a3", "a4",
    "a5", "a6", "a7", "s2", "s3", "s4", "s5", "s6", "s7", "s8", "s9", "s10", "s11", "t3", "t4",
    "t5", "t6",
];

impl Display for Cpu {
    fn fmt(&self, f: &mut Formatter<'_>) -> fmt::Result {
        writeln!(f, "CPU State")?;
        writeln!(f, "PC:  {:08X}", self.pc)?;
        for (i, reg) in self.regs.iter().enumerate() {
            let label = format!("x{i:<2} / {}", ABI_LABELS[i]);
            write!(f, "{label:10} {reg:08X}")?;
            if i % 4 == 3 {
                writeln!(f)?;
            } else {
                write!(f, " | ")?;
            }
        }
        Ok(())
    }
}
