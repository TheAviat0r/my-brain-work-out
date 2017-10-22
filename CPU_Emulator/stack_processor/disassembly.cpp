#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <io.h>
#include <assert.h>

#define SLASHES printf("==========================================\n");
#define EMPT printf("\n");
//{!==================================
const int ax_code = 100499;
const int bx_code = 100498;
const int cx_code = 100497;
const int dx_code = 100496;
const int ex_code = 100495;
const int fx_code = 100494;
const int LABEL_LIM = 10;
const int NOEXIST   = -1;
//!==================================
const char str_END[] =      "end";
const char str_PUSH[] =     "push";
const char str_PUSHR[] =    "pushr";
const char str_POP[] =      "pop";
const char str_ADD[] =      "add";
const char str_MIN[] =      "min";
const char str_MUL[] =      "mul";
const char str_DIV[] =      "div";
const char str_SQR[] =      "sqr";
const char str_SQRT[] =     "sqrt";
const char str_OUT[] =      "out";
const char str_JB[] =       "jb";
const char str_JA[] =       "ja";
const char str_JAC[] =      "jac";
const char str_JBC[] =      "jbc";
const char str_JE[] =       "je";
const char str_JNE[] =      "jne";
const char str_JMP[] =      "jmp";
const char str_ax[] =       "ax";
const char str_bx[] =       "bx";
const char str_cx[] =       "cx";
const char str_dx[] =       "dx";
const char str_ex[] =       "ex";
const char str_fx[] =       "fx";
const char str_LABEL[] =    ":";
const char str_CALL[] =     "call";
const char str_RET[] =      "ret";
const char COMMAND[] =      "commands:";
//}!==================================

struct label_t
{
    char name[20];
    int adress;
};

#define DEF_CMD(name, num, code) CMD_##name = num,
enum Commands_t
{
    #include "command.h"
    CMD_MAX
};
//!-------------------------------
void ProcessDis(void);
    void firstPass(FILE *input, int arr_label[]);
    void secondPass(FILE *input);
    int findPosition(int target, int arr_label[]);
//!-------------------------------
    int compareLabels(const void *a, const void *b);
    void initiateLabels(int arr_label[]);
    void dump_label(int arr_label[]);
    int cntLabels(int arr_label[]);
    void getReal (int arr_label[], int real_label[], int cnt);
//!-------------------------------
int main()
{
    ProcessDis();
    return 0;
}

void ProcessDis(void)
{
    FILE *input = 0;
    if (input = fopen("asmed.txt", "r"))
    {
        FILE *output = fopen("disasmed.txt", "w");

        int arr_label[LABEL_LIM] = {};
        initiateLabels(arr_label);

        firstPass(input, arr_label);

        int real_len = cntLabels(arr_label);
        printf("real_len = %d\n", real_len);
        if (real_len > 0)
        {
            int* real_label = (int *) calloc(real_len, sizeof(int));
            getReal(arr_label, real_label, real_len);

            SLASHES
            qsort(real_label, real_len, sizeof(int), &compareLabels);

            for (int i = 0; i < real_len; i++)
            {
                printf("real[%d] = <%d>\n", i, real_label[i]);
            }
        }


        fclose(output);
        fclose(input);
    }
    else
        printf("Unable to input asmed.txt ! Try again!\n");
}
//!--------------------------------------------------------
//!  Main goal of firstPass function - grab all positions in commands like call, jmp and so son
void firstPass(FILE *input, int arr_label[])
{
    assert(input != NULL);
    assert(arr_label != NULL);

    int command = 0;
    int position = 0, command_arg = 0;
    int cnt_label = 0;

    char trash[20] = {};
    int anotherTrash = 0;

    fscanf(input, "%s", trash);
    fscanf(input, "%d", &anotherTrash);

    fscanf(input, "%d", &command);

    #define ONEOPER(command, name)      if (command == CMD_##name)\
                                        {\
                                            fscanf(input, "%d", &command);\
                                            continue;\
                                        }

    #define DOUBLEOPER(command, name)   if (command == CMD_##name)\
                                        {\
                                            fscanf(input, "%d", &command_arg);\
                                            fscanf(input, "%d", &command);\
                                            continue;\
                                        }

    #define JUMPS(command, name)        if (command == CMD_##name)\
                                        {\
                                            fscanf(input, "%d", &position);\
                                            assert(position >= 0);\
                                            \
                                            if (findPosition(position, arr_label) == NOEXIST)\
                                            {\
                                                if (cnt_label < LABEL_LIM)\
                                                {\
                                                    arr_label[cnt_label] = position;\
                                                    cnt_label++;\
                                                    \
                                                    fscanf(input, "%d", &command);\
                                                    continue;\
                                                }\
                                                else\
                                                {\
                                                    printf("ERROR! UNABLE TO ADD POSITION INTO ARR_LABEL!\n");\
                                                    exit(1);\
                                                }\
                                            } \
                                            else\
                                            {\
                                                fscanf(input, "%d", &command);\
                                                continue;\
                                            }\
                                        }\

    while (command != CMD_END)
    {
        DOUBLEOPER(command, PUSH);
        DOUBLEOPER(command, PUSHR);
        DOUBLEOPER(command, POP);
        DOUBLEOPER(command, OUT);

        ONEOPER(command, ADD);
        ONEOPER(command, MIN);
        ONEOPER(command, MUL);
        ONEOPER(command, DIV);
        ONEOPER(command, SQR);
        ONEOPER(command, SQRT);
        ONEOPER(command, RET);

        JUMPS(command, CALL);
        JUMPS(command, JMP);
        JUMPS(command, JB);
        JUMPS(command, JBC);
        JUMPS(command, JA);
        JUMPS(command, JAC);
        JUMPS(command, JE);
        JUMPS(command, JNE);
    }

    dump_label(arr_label);
    rewind(input);
    #undef ONEOPER
    #undef DOUBLEOPER
}

int findPosition(int target, int arr_label[])
{
    assert(arr_label != NULL);
    int cnt = 0;

    while (cnt < LABEL_LIM)
    {
        assert(cnt < LABEL_LIM);

        if (arr_label[cnt] == target)
            return cnt;
        cnt++;
    }

    return NOEXIST;
}

void initiateLabels(int arr_label[])
{
    assert(arr_label != NULL);
    for (int i = 0; i < LABEL_LIM; i++)
    {
        assert(0 <= i && i < LABEL_LIM);
        arr_label[i] = NOEXIST;
    }
}

int cntLabels(int arr_label[])
{
    assert(arr_label != NULL);
    int cnt = 0;

    while (arr_label[cnt] != NOEXIST)
        cnt++;

    return cnt;
}

int compareLabels (const void *a, const void *b)
{
    assert(a != NULL && b != NULL);
    assert(*(int *)a >= 0 && *(int *)b >= 0);

    return (*(int *)a - *(int *)b);
}

void getReal (int arr_label[], int real_label[], int cnt)
{
    assert(arr_label != NULL && real_label != NULL);

    for (int i = 0; i < cnt; i++)
    {
        assert(0 <= i && i < cnt);
        real_label[i] = arr_label[i];
    }
}

void dump_label(int arr_label[])
{
    EMPT SLASHES printf("arr_label - [0x%p]\n", arr_label);
    SLASHES
    for (int i = 0; i < LABEL_LIM; i++)
    {
        assert(0 <= i && i < LABEL_LIM);
        printf("lab(%d) [0x%p] - <%d>\n", i, arr_label + i , arr_label[i]);
    }
    SLASHES EMPT
}

