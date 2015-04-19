#include <iostream>
#include <vector>
#include <assert.h>
#include <stdio.h>

using std::vector;

void dword_rev(vector<unsigned char>& result, int source);

void dword_rev(vector<unsigned char>& result, int source)
{
    assert(result.empty());
    int work = 0;

    while (source > 0)
    {
        work = source;
        work = work & 0xFF;

        printf("work = (0x%x)\n", work);
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
}

