#include "simulator.h"

Simulator::Simulator() : pc(0) {
}

void Simulator::run(const int max_steps) {
    int step = 0;
    Status status = Status::AOK;
    for (step = 0; step < max_steps && status == Status::AOK; step++) {
        status = next_instruction();
    }
    report_stopped(step, status);
}

static uint8_t get_hi4(const uint8_t pack) { return (pack >> 4) & 0xF; }
static uint8_t get_lo4(const uint8_t pack) { return pack & 0xF; }

Status Simulator::next_instruction() {
    uint64_t next_pc = pc;

    // get code and function (1 byte)
    const std::optional<uint8_t> codefun = memory.get_byte(next_pc);
    if (!codefun) {
        report_bad_inst_addr();
        return Status::ADR;
    }
    const auto icode = static_cast<InstructionCode>(get_hi4(codefun.value()));
    uint8_t ifun = get_lo4(codefun.value());
    next_pc++;

    // execute the instruction
    switch (icode) {
        case InstructionCode::HALT: // 0:0
        {
            return Status::HLT;
        }
        case InstructionCode::NOP: // 1:0
        {
            pc = next_pc;
            return Status::AOK;
        }
        case InstructionCode::RRMOVQ: // 2:x regA:regB
        {
            const std::optional<uint8_t> regs = memory.get_byte(next_pc);
            if (!regs) {
                report_bad_inst_addr();
                return Status::ADR;
            }

            const uint8_t reg_a = get_hi4(regs.value());
            const uint8_t reg_b = get_lo4(regs.value());
            if (error_invalid_reg(reg_a)) return Status::INS;
            if (error_invalid_reg(reg_b)) return Status::INS;
            next_pc++;

            if (cc.satisfy(static_cast<Condition>(ifun))) {
                registers[reg_b] = registers[reg_a];
            }
            pc = next_pc;
            return Status::AOK;
        }
        case InstructionCode::IRMOVQ: {// 3:0 F:regB imm 
            const std::optional<uint8_t> regs = memory.get_byte(next_pc);
            if (!regs) {
                report_bad_inst_addr();
                return Status::ADR;
            }
            
            const uint8_t reg_a = get_hi4(regs.value());
            const uint8_t reg_b = get_lo4(regs.value());
            if (error_valid_reg(reg_a)) return Status::INS;
            if (error_invalid_reg(reg_b)) return Status::INS;
            next_pc++;

            const std::optional<uint64_t> imm = memory.get_long(next_pc);
            if (!imm) {
                report_bad_inst_addr();
                return Status::ADR;
            }
            next_pc += 8;

            const uint64_t Imm = imm.value();
            registers[reg_b] = Imm;
            pc = next_pc;
            return Status::AOK;
        }
        case InstructionCode::RMMOVQ: { // 4:0 regA:regB imm
            const std::optional<uint8_t> regs = memory.get_byte(next_pc);
            if (!regs) {
                report_bad_inst_addr();
                return Status::ADR;
            }

            const uint8_t reg_a = get_hi4(regs.value());
            const uint8_t reg_b = get_lo4(regs.value());
            if (error_invalid_reg(reg_a)) return Status::INS;
            if (error_invalid_reg(reg_b)) return Status::INS;
            next_pc++;

            const std::optional<uint64_t> imm = memory.get_long(next_pc);
            if (!imm) {
                report_bad_inst_addr();
                return Status::ADR;
            }
            next_pc += 8;

            const uint64_t Imm = imm.value();
            const uint64_t addr = Imm + registers[reg_b];
            if (!memory.set_long(addr, registers[reg_a])) {
                report_bad_data_addr(addr);
                return Status::ADR;
            }
            pc = next_pc;
            return Status::AOK;
        }
        case InstructionCode::MRMOVQ: { // 5:0 regA:regB imm
            const std::optional<uint8_t> regs = memory.get_byte(next_pc);
            if (!regs) {
                report_bad_inst_addr();
                return Status::ADR;
            }

            const uint8_t reg_a = get_hi4(regs.value());
            const uint8_t reg_b = get_lo4(regs.value());
            if (error_invalid_reg(reg_a)) return Status::INS;
            if (error_invalid_reg(reg_b)) return Status::INS;
            next_pc++;

            const std::optional<uint64_t> imm = memory.get_long(next_pc);
            if (!imm) {
                report_bad_inst_addr();
                return Status::ADR;
            }
            next_pc += 8;

            const uint64_t Imm = imm.value();
            const uint64_t addr = Imm + registers[reg_b];
            const std::optional<uint64_t> data = memory.get_long(addr);
            if (!data) {
                report_bad_data_addr(addr);
                return Status::ADR;
            }

            registers[reg_a] = data.value();
            pc = next_pc;
            return Status::AOK;
        }
        case InstructionCode::ALU: { // 6:x regA:regB
            const std::optional<uint8_t> regs = memory.get_byte(next_pc);
            if (!regs) {
                report_bad_inst_addr();
                return Status::ADR;
            }

            const uint8_t reg_a = get_hi4(regs.value());
            const uint8_t reg_b = get_lo4(regs.value());
            if (error_invalid_reg(reg_a)) return Status::INS;
            if (error_invalid_reg(reg_b)) return Status::INS;
            next_pc++;

            uint64_t data = 0, data_a = registers[reg_a], 
                        data_b = registers[reg_b];
            switch(ifun) {
                case 0: data = data_b + data_a;
                break;
                case 1: data = data_b - data_a;
                break;
                case 2: data = data_b & data_a;
                break;
                case 3: data = data_b ^ data_a;
                break;
                default:
                    report_bad_inst(codefun.value());
                    return Status::INS;
            }

            cc = ConditionCodes::compute(static_cast<AluOp>(ifun), data_a, data_b, data);
            registers[reg_b] = data;

            pc = next_pc;
            return Status::AOK;
        }
        case InstructionCode::JMP: { // 7:x imm
            const std::optional<uint64_t> dest = memory.get_long(next_pc);
            if (!dest) {
                report_bad_inst_addr();
                return Status::ADR;
            }
            next_pc += 8;
            
            if (ifun > 6) {
                report_bad_inst(codefun.value());
                return Status::INS;
            }
            
            if (cc.satisfy(static_cast<Condition>(ifun))) {
                next_pc = dest.value();
            }
            pc = next_pc;
            return Status::AOK;
        }
        case InstructionCode::CALL: { // 8:0 imm
            const std::optional<uint64_t> dest = memory.get_long(next_pc);
            if (!dest) {
                report_bad_inst_addr();
                return Status::ADR;
            }
            next_pc += 8;

            uint64_t& addr = registers[RegId::RSP];
            if (!memory.set_long(addr - 8, next_pc)) {
                report_bad_stack_addr(addr - 8);
                return Status::ADR;
            }

            addr -= 8;
            pc = dest.value();
            return Status::AOK;
        }
        case InstructionCode::RET: { // 9:0
            uint64_t& addr = registers[RegId::RSP];
            const std::optional<uint64_t> ReturnAddr = memory.get_long(addr);
            if (!ReturnAddr) {
                report_bad_stack_addr(addr);
                return Status::ADR;
            }

            addr += 8;
            pc = ReturnAddr.value();
            return Status::AOK;
        }
        case InstructionCode::PUSHQ: { // A:0 regA:F
            const std::optional<uint8_t> regs = memory.get_byte(next_pc);
            if (!regs) {
                report_bad_inst_addr();
                return Status::ADR;
            }

            const uint8_t reg_a = get_hi4(regs.value());
            const uint8_t reg_b = get_lo4(regs.value());
            if (error_invalid_reg(reg_a)) return Status::INS;
            if (error_valid_reg(reg_b)) return Status::INS;
            next_pc++;

            uint64_t data = registers[reg_a];
            uint64_t& addr = registers[RegId::RSP];
            if (!memory.set_long(addr - 8, data)) {
                report_bad_stack_addr(addr - 8);
                return Status::ADR;
            }
            addr -= 8;
            pc = next_pc;
            return Status::AOK;
        }
        case InstructionCode::POPQ: { // B:0 regA:F
            const std::optional<uint8_t> regs = memory.get_byte(next_pc);
            if (!regs) {
                report_bad_inst_addr();
                return Status::ADR;
            }

            const uint8_t reg_a = get_hi4(regs.value());
            const uint8_t reg_b = get_lo4(regs.value());
            if (error_invalid_reg(reg_a)) return Status::INS;
            if (error_valid_reg(reg_b)) return Status::INS;
            next_pc++;

            uint64_t& addr = registers[RegId::RSP];
            const std::optional<uint64_t> data = memory.get_long(addr);
            if (!data) {
                report_bad_stack_addr(addr);
                return Status::ADR;
            }
            addr += 8;

            registers[reg_a] = data.value();
            pc = next_pc;
            return Status::AOK;
        }
            return Status::INS; // TODO: unsupported now, replace with your implementation
        default:
            report_bad_inst(codefun.value());
            return Status::INS;
    }
}
