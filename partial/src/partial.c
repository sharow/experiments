/* -*- Mode: c; tab-width: 4; c-basic-offset: 4; indent-tabs-mode: nil; -*- */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

#include <sys/mman.h>

#define CODE_SIZE_LEAST (64)


void *partial(void *function, void *first_arg)
{
    void *partialized = mmap(NULL, CODE_SIZE_LEAST,
                             PROT_READ | PROT_WRITE | PROT_EXEC,
                             MAP_PRIVATE | MAP_ANON, -1, 0);
    if (partialized == (void *)-1) {
        return NULL;
    }

    unsigned char *p = partialized;
    assert(sizeof(uintptr_t) == 8);
    *p++ = 0x41; *p++ = 0x51;  // push r9
    *p++ = 0x4d; *p++ = 0x89; *p++ = 0xc1;  // mov r9, r8
    *p++ = 0x49; *p++ = 0x89; *p++ = 0xc8;  // mov r8, rcx
    *p++ = 0x48; *p++ = 0x89; *p++ = 0xd1;  // mov rcx, rdx
    *p++ = 0x48; *p++ = 0x89; *p++ = 0xf2;  // mov rdx, rsi
    *p++ = 0x48; *p++ = 0x89; *p++ = 0xfe;  // mov rsi, rdi
    *p++ = 0x48; *p++ = 0xbf; *(uintptr_t *)p = (uintptr_t)first_arg; p += sizeof(uintptr_t);  // movabs rdi, first_arg
    *p++ = 0x48; *p++ = 0xb8; *(uintptr_t *)p = (uintptr_t)function; p += sizeof(uintptr_t);  // movabs rax, function
    *p++ = 0xff; *p++ = 0xd0;  // call rax
    *p++ = 0x41; *p++ = 0x59;  // pop r9
    *p++ = 0xc3;  // ret
    return partialized;
}

void unpartial(void *partialized)
{
    munmap(partialized, CODE_SIZE_LEAST);
}
