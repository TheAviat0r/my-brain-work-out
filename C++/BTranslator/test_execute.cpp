#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <sys/mman.h>
#include <assert.h>
#include <iostream>

using std::string;
using std::vector;

#include "commands.h"

void emit_to_rw_run_from_rx();

const int SIZE = 1024;

void emit_code(void * buf)
{
    vector<unsigned char> code;
    code.push_back(PUSH_BYTE);
    code.push_back(20);

    code.push_back(0x58);       //! pop rax
    code.push_back(RET);

    /*for (int i = 0; i < code.size(); i++)
    {
        printf("%x ", code[i]);
    }*/

    std:: cout << "\n";

    memcpy(buf, code.data(), code.size());

    for (int i = 0; i < code.size(); i++)
    {
        printf("%x ", *(unsigned char *)(buf + i));
    }
}

void* alloc_writable_memory(size_t size)
{
    void* ptr = mmap(0, size,
                    PROT_READ | PROT_WRITE,
                    MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (ptr == (void*)-1)
    {
        perror("mmap");
        return NULL;
    }

    return ptr;
}

int make_memory_executable(void* m, size_t size)
{
    if (mprotect(m, size, PROT_READ | PROT_EXEC) == -1)
    {
        perror("mprotect");
        return -1;
    }

    return 0;
}

void load_bin_code (unsigned char * code, void * buffer)
{
    assert(code);
    assert(buffer);
    memcpy(buffer, code, sizeof(code));
}

void JIT_compile (FILE* input);

int main()
{
    //FILE * input = fopen("test.o", "rb+");
    //if (!input)
        //return 1;

    emit_to_rw_run_from_rx();

    return 0;
}

void emit_to_rw_run_from_rx()
{
    void* m = alloc_writable_memory(SIZE);
    emit_code(m);
    make_memory_executable(m, SIZE);

    //printf("char (%c%c)\n", s[0], s[1]);
    int (*func)(int) = (int (*)(int))m;
    printf("result = %d\n", func(0));
}

