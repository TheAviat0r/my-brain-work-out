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

void dword_rev(vector<unsigned char>& result, int source);

void dword_rev(vector<unsigned char>& result, int source)
{
    assert(result.empty());

    cout << "\nREVERSE NUMBER - " << source << endl;

    int work = 0;

    while (source > 0)
    {
        work = source;
        work = work & 0xFF;

        result.push_back(work);

        source = source >> 8;
    }

    for (size_t i = result.size(); i < sizeof(int); i++)
        result.push_back(0);

    for (size_t i = 0; i < result.size(); i++)
    {
        if (result[i])
            printf("%x ", result[i]);
        else
            printf("00 ");
    }

    cout << endl << endl;
}


