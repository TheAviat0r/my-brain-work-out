#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <cstdlib>
#include <sys/mman.h>
#include <assert.h>
#include <string.h>

#include <fstream>
#include <vector>
#include <string>
#include <stdexcept>

using std::size_t;
using std::vector;
using std::string;
using std::fstream;
using std::cout;
using std::endl;

#include "commands.h"
#include "reg_status.h"
#include "config.h"

#define DEF_CMD(name, num, code) CMD_##name = num,
enum commands_t
{
    #include "old_commands.h"
    CMD_MAX
};

const int ax_code = 100499;
const int bx_code = 100498;
const int cx_code = 100497;
const int dx_code = 100496;
const int ex_code = 100495;
const int fx_code = 100494;

const int NUM_INDEX = 1;
const int VAR_INDEX = 2;

const int BYTE = 1;
const int DWORD = 2;

void dword_rev(vector<unsigned char>& result, int source);

void dword_rev(vector<unsigned char>& result, int source)
{
    int work = 0;

    printf("\nBEFORE:\n \t %x\n", source);

    for (int i = 0; i < 4; i++)
    {
        work = source;
        work = work & 0xFF;

        result.push_back(work);

        source = source >> 8;
    }

    /*cout << endl;

    printf("\nAFTER:\n \t ");

    for (size_t i = 0; i < sizeof(int); i++)
        printf("%x ", result[i]);

    cout << endl;*/
}
void void_rev(vector<unsigned char>& result, void * buffer)
{
    unsigned long work = 0;

    printf("\n\tBEFORE: %x\n", buffer);

    while (buffer > 0)
    {
        work = (unsigned long) buffer;
        work = ((unsigned long) work & 0xFF);

        unsigned char save = (unsigned char) work;
        result.push_back(save);

        buffer = (void *) ((unsigned long) buffer >> 8);
    }

    for (size_t i = result.size(); i < sizeof(int); i++)
        result.push_back(0);

    printf("\n\tAFTER:   ");

    for (size_t i = 0; i < 4; i++)
        printf("%x ", result[i]);

    cout << endl;
}
