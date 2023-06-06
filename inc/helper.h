#ifndef HELPER_H_
#define HELPER_H_

#include <stdio.h>
#include <unicorn/unicorn.h>
#include <unicorn/x86.h>

struct register_state {
    int r_ecx;
    int r_ebx;
    int r_eax;
    int r_edx;
    int r_ebp;
    int r_edi;
    int r_esi;
    int r_eip;
    int r_esp;
};


/**
 * populate Register_state struct and dump values.
 * params: pointer to uc_engine
 * return register_state struct
 */
struct register_state stackdump(uc_engine *uc, bool debug) {

    struct register_state reg;

    uc_reg_read(uc, UC_X86_REG_ECX, &reg.r_ecx);
    uc_reg_read(uc, UC_X86_REG_EAX, &reg.r_eax);
    uc_reg_read(uc, UC_X86_REG_EDX, &reg.r_edx);
    uc_reg_read(uc, UC_X86_REG_EBX, &reg.r_ebx);
    uc_reg_read(uc, UC_X86_REG_EBP, &reg.r_ebp);
    uc_reg_read(uc, UC_X86_REG_EDI, &reg.r_edi);
    uc_reg_read(uc, UC_X86_REG_ESI, &reg.r_esi);
    uc_reg_read(uc, UC_X86_REG_EIP, &reg.r_eip);
    uc_reg_read(uc, UC_X86_REG_ESP, &reg.r_esp);

    if (debug) {
        printf(">> EAX: 0x%04x\n", reg.r_eax);
        printf(">> EBX: 0x%04x\n", reg.r_ebx);
        printf(">> EDX: 0x%04x\n", reg.r_edx);
        printf(">> ECX: 0x%04x\n", reg.r_ecx);
        printf(">> ESI: 0x%04x\n", reg.r_esi);
        printf(">> EDI: 0x%04x\n", reg.r_edi);
        printf(">> ESP: 0x%04x\n", reg.r_esp);
        printf(">> EBP: 0x%04x\n", reg.r_ebp);
        printf(">> EIP: 0x%04x\n", reg.r_eip);
    }

    return reg;

}


#endif // HELPER_H_
