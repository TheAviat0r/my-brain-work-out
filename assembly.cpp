#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>
#include <assert.h>
//!==================================================================
#ifndef NODEBUG
    #define assert_comm(cond) if (!cond)
#endif
#define SLASHES printf("==========================================\n");
#define EMPT printf("\n");
//!==================================================================
#define DEF_CMD(name, num, code) CMD_##name = num,
enum Commands_t
{
    #include "command.h"
    CMD_MAX
};
#undef DEF_CMD
#define DBG printf("#");
//!==================================================================
const int ax_code = 100499;
const int bx_code = 100498;
const int cx_code = 100497;
const int dx_code = 100496;
const int ex_code = 100495;
const int fx_code = 100494;
const int LABEL_LIM = 10;
const int NOEXIST   = -1;
//!==================================================================
//!                    HERE ARE STR CONSTANTS
//{!==================================================================
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
//}==================================================================
struct label_t
{
    char name[20];
    int adress;
};
//!==================================================================
void ProcessAsm(void);
//!==================================================================
    int cntComm(FILE *input, FILE *output);
    void dump_Comm(double *buffer, int buffer_len, int position);
    void formCommands(FILE *input, double arr_comm[], label_t arr_label[], int comm_size);
    void initiateLabels(label_t arr_label[]);
    int getLabel(label_t arr_label[], char target[]);
    int getEnd(double arr_comm[], int end_position);
//!==================================================================
int main()
{
    SLASHES
    printf("program had started!\n");
    ProcessAsm();
    return 0;
}
//!==================================================================
void dump_Comm(double *buffer, label_t arr_label[], int buffer_len, int position)
{
    assert(buffer != NULL && buffer_len != NULL);
    assert(position <= buffer_len);

    EMPT SLASHES
    printf("\tCOMMAND DUMP\n");
    SLASHES
    printf("arr_comm = [0x%p]\n", buffer);
    printf("comm_size = <%d>\n", buffer_len);
    printf("position = <%d>\n", position);
    SLASHES

    for (int i = 0; i < buffer_len; ++i)
    {
        assert(0 <= i && i < buffer_len);
        if (i < 10)
        {
            if (i != position)
                printf("   [%d] = <%lg> \n", i, buffer[i]);
            else
                printf(" ->[%d] = <%lg> \n", i, buffer[i]);
        }
        else
        {
            if (i != position)
                printf("  [%d] = <%lg> \n", i, buffer[i]);
            else
                printf("->[%d] = <%lg> \n", i, buffer[i]);
        }
    }
    SLASHES

    printf("\tDUMP OF LABELS\n");
    SLASHES

    for (int i = 0; i < LABEL_LIM; ++i)
    {
        assert(0 <= i && i < LABEL_LIM);
        printf("   %d) <%s> [%d]\n", i, arr_label[i].name, arr_label[i].adress);
    }
    SLASHES
}
//!==================================================================
void ProcessAsm(void)
{
    FILE *input = 0;
    if (input = fopen("input.txt", "r"))
    {
        SLASHES;
        printf("input.txt successfully opened\n");
        //!=======================================
        FILE *output = fopen("asmed.txt", "w");
        //!         INITIALISE WORKING VALUES
        //{=======================================
        char command[10] = {};
        char reg[10] = {};
        double value = 0;
        int jump_value = 0;
        int cnt_comm = 0;

        //}=======================================

        int comm_size = 2 * cntComm(input, output) + 1;
        assert(comm_size != NULL);
        double * arr_comm = (double *) calloc(comm_size, sizeof(double));
        assert(arr_comm != NULL);
        label_t arr_label[LABEL_LIM] = {};

        initiateLabels(arr_label);
        formCommands(input, arr_comm, arr_label, comm_size);
        int end_position = getEnd(arr_comm, comm_size);

        fprintf(output, "\n");
        for (int i = 0; i <= end_position; i++)
        {
            assert(0 <= i && i <= end_position);
            fprintf(output, "%lg ", arr_comm[i]);
        }

        //!======================================
        free(arr_comm);
        free(arr_label);
        fclose(input);
        fclose(output);
    }
    else
    {
        SLASHES;
        printf("ERROR! Unable to open input.txt!\n");
    }
}
//!====================================
int cntComm(FILE *input, FILE *output)
{
    assert(input != NULL && output != NULL);
    char command[20] = {};
    double value = 0;
    int cnt_comm = 0;

    fscanf(input, "%s", command);

    #define DEF_CMD(name, num, code) if(!strcmp(command, str_##name)) cnt_comm++;
    while (strcmp(command, str_END))
    {
        #include "command.h"
        fscanf(input, "%s", command);
    }
    #undef DEF_CMD

    SLASHES DBG printf("commands are counted: <%d>\n", cnt_comm);

    rewind(input);
    fprintf(output, "%s %d\n", COMMAND, cnt_comm);

    return cnt_comm;
}

void initiateLabels(label_t arr_label[])
{
    assert(arr_label != NULL);

    for (int i = 0; i < LABEL_LIM; ++i)
    {
        assert (0 <= i && i < LABEL_LIM);
        arr_label[i].adress = NOEXIST;
    }
}
//!====================================
//!     formCommands() reads commands from txt and fills it in an array of commands
//!====================================
void formCommands(FILE *input, double arr_comm[], label_t arr_label[], int comm_size)
{
    assert(arr_comm != NULL && input != NULL && comm_size != NULL && arr_label != NULL);
    //!            INITIALIZE WORKING VARIABLES
    //{===============================================
    char command[10] = {};
    char reg[10] = {};
    char label_name [20] = {};
    char call_arg [20] = {};
    char jump_label [20] = {};

    double push_value = 0;
    int i_comm = 0;
    int cnt_label = 0;
    int command_len = 0;
    //}===============================================

    for (int pass = 1; pass <= 2; ++pass)
    {
        assert(1 <= pass && pass <= 2);
        fscanf(input, "%s", command);

        while (strcmp(command, str_END))
        {
            command_len = strlen(command);
            assert(command_len > 0);
            if (command_len <= 1)
            {
                printf("WRONG INPUT! COMMAND LENGTH SHOULD BE > 1\n");
                exit(10);
            }

            assert(0 <= i_comm && i_comm < comm_size);

            if (!strcmp(command, str_PUSH))
            {
                arr_comm[i_comm] = CMD_PUSH;
                i_comm++;

                assert(0 <= i_comm && i_comm < comm_size);

                if (fscanf(input, "%lf", &push_value) == 1)
                {
                    arr_comm[i_comm] = push_value;
                    i_comm++;
                    fscanf(input, "%s", command);
                    continue;
                }
                else
                {
                    DBG printf("ERR_<push_argument>! UNABLE TO READ VALUE!\n");
                    dump_Comm(arr_comm, arr_label, comm_size, i_comm);
                    exit(1);
                }
            }

            if (!strcmp(command, str_PUSHR))
            {
                arr_comm[i_comm] = CMD_PUSHR;
                i_comm++;

                if (fscanf(input, "%s", reg) == 1)
                {
                    #define PUSHR(reg, name)\
                                            if (!strcmp(reg, str_##name)) \
                                            {\
                                                arr_comm[i_comm] = name##_code;\
                                                i_comm++;\
                                                fscanf(input, "%s", command);\
                                                continue;\
                                            }
                    PUSHR(reg, ax)
                    PUSHR(reg, bx)
                    PUSHR(reg, cx)
                    PUSHR(reg, dx)
                    PUSHR(reg, ex)
                    PUSHR(reg, fx)

                    #undef PUSHR

                    SLASHES DBG printf("ERR_<pushr_argument>! WRONG REGISTER NAME!\n");
                    dump_Comm(arr_comm, arr_label, comm_size, i_comm);
                    exit(1);
                }
                else
                {
                    SLASHES DBG printf("ERR_<pushr_argument>! UNABLE TO READ REGISTER!\n");
                    dump_Comm(arr_comm, arr_label, comm_size, i_comm);
                    exit(1);
                }
            }

            if (!strcmp(command, str_POP))
            {
                arr_comm[i_comm] = CMD_POP;
                i_comm++;

                #define POPR(reg, name) if (!strcmp(reg, str_##name)) \
                                        {\
                                            arr_comm[i_comm] = name##_code;\
                                            i_comm++;\
                                            fscanf(input, "%s", command);\
                                            continue;\
                                        }
                if (fscanf(input, "%s", reg) == 1)
                {
                    POPR(reg, ax)
                    POPR(reg, bx)
                    POPR(reg, cx)
                    POPR(reg, dx)
                    POPR(reg, ex)
                    POPR(reg, fx)

                    SLASHES DBG printf("ERR_<pop_argument>! WRONG REGISTER NAME!\n");
                    dump_Comm(arr_comm, arr_label, comm_size, i_comm);
                    exit(CMD_POP);
                }
                else
                {
                    SLASHES DBG printf("ERR_<pop_input>! UNABLE TO READ REGISTER\n");
                    exit(CMD_POP + 1);
                }
                #undef POPR
            }

            //{!============<ARITHMETIC OPERATIONS ARE BELOW>===============
            #define OPER(command, name) if (!strcmp(command, str_##name))\
                                        {\
                                            arr_comm[i_comm] = CMD_##name;\
                                            i_comm++;\
                                            fscanf(input, "%s", command);\
                                            continue;\
                                        }
            OPER(command, ADD)
            OPER(command, MIN)
            OPER(command, MUL)
            OPER(command, DIV)
            OPER(command, SQR)
            OPER(command, SQRT)
            #undef OPER
            //}!============================================================

            if (command[command_len - 1] - ':' == 0)
            {
                command[command_len - 1] = 0;
                strcpy(label_name, command);
                //printf("label = <%s>\n", label_name);
                if (command[0] != NULL)
                {
                    if (pass == 1)
                    {
                        strcpy(arr_label[cnt_label].name, label_name);

                        if (cnt_label < LABEL_LIM)
                        {
                            arr_label[cnt_label].adress = i_comm;
                            cnt_label++;

                            fscanf(input, "%s", command);
                            continue;
                        }
                        else
                        {
                            printf("ERROR! ARRAY OF LABELS IS FULL! RECOMPILE PROGRAM\n!");
                            exit(1488);
                        }
                    }

                    fscanf(input, "%s", command);
                    continue;
                }
                else
                {
                    SLASHES DBG printf("ERR_<label_argument>! UNABLE TO READ LABEL!\n");
                    dump_Comm(arr_comm, arr_label, comm_size, i_comm);
                    exit(1488);
                }
            }

            if (!strcmp(command, str_CALL))
            {
                if (fscanf(input, "%s", call_arg) == 1)
                {
                    arr_comm[i_comm] = CMD_CALL;
                    i_comm++;
                    assert(0 <= i_comm && i_comm < comm_size);
                    int callNum = getLabel(arr_label, call_arg);
                    printf("callnum = %d\n", callNum);

                    if (callNum == NOEXIST && pass == 2)
                    {
                        SLASHES DBG printf("ERR_<call_label>! UNABLE TO FIND LABEL <%s>!\n", call_arg);
                        exit(CMD_CALL);
                    }

                    if (callNum >= 0 && arr_label[callNum].adress != NOEXIST)
                        arr_comm[i_comm] = arr_label[callNum].adress;
                    else
                        arr_comm[i_comm] = NOEXIST;
                    i_comm++;

                    fscanf(input, "%s", command);
                    continue;
                }
                else
                {
                    SLASHES DBG printf("ERR_<call_argument!>! UNABLE TO READ CALL!\n");
                    dump_Comm(arr_comm, arr_label, comm_size, i_comm);
                    exit(CMD_CALL + 1);
                }
            }
            //{====================================================================
            #define GETJMP(command, name)   if(!strcmp(command, str_##name))\
                                            {\
                                                if (fscanf(input, "%s", jump_label) == 1)\
                                                {\
                                                    arr_comm[i_comm] = CMD_##name;\
                                                    i_comm++;\
                                                    assert(0 <= i_comm && i_comm < comm_size);\
                                                    int jumpNum = getLabel(arr_label, jump_label);\
                                                    if (jumpNum == NOEXIST && pass == 2)\
                                                    {\
                                                        DBG printf("ERR_<jump_label!> UNABLE TO FIND\
                                                                                LABEL <%s>!\n", jump_label);\
                                                        exit(CMD_##name);\
                                                    }\
                                                    \
                                                    if (jumpNum >= 0 && arr_label[jumpNum].adress != NOEXIST)\
                                                        arr_comm[i_comm] = arr_label[jumpNum].adress;\
                                                    else\
                                                        arr_comm[i_comm] = NOEXIST;\
                                                    i_comm++;\
                                                    \
                                                    fscanf(input, "%s", command);\
                                                    continue;\
                                                }\
                                            }


            GETJMP(command, JMP);
            GETJMP(command, JB);
            GETJMP(command, JBC);
            GETJMP(command, JA);
            GETJMP(command, JAC);
            GETJMP(command, JE);
            GETJMP(command, JNE);
            #undef GETJMP
            //}========================================================================

            if (!strcmp(command, str_OUT))
            {
                if (fscanf(input, "%s", reg) == 1)
                {
                    arr_comm[i_comm] = CMD_OUT;
                    i_comm++;
                    #define OUTR(reg, name)\
                                            if (!strcmp(reg, str_##name)) \
                                            {\
                                                arr_comm[i_comm] = name##_code;\
                                                i_comm++;\
                                                fscanf(input, "%s", command);\
                                                continue;\
                                            }
                    OUTR(reg, ax);
                    OUTR(reg, bx);
                    OUTR(reg, cx);
                    OUTR(reg, dx);
                    OUTR(reg, ex);
                    OUTR(reg, fx);
                    #undef OUTR

                    SLASHES DBG printf("ERR_<out_argument>! WRONG REGISTER NAME! TRY AGAIN!\n");
                    dump_Comm(arr_comm, arr_label, comm_size, i_comm);
                    exit(11);
                }
                else
                {
                    SLASHES DBG printf("ERR_<out_read>! UNABLE TO READ REGISTER!\N");
                    dump_Comm(arr_comm, arr_label, comm_size, i_comm);
                    exit(10);
                }
            }

            if (!strcmp(command, str_RET))
            {
                arr_comm[i_comm] = CMD_RET;
                i_comm++;
                fscanf(input, "%s", command);
                continue;
            }


            SLASHES DBG printf("ERR_<command_input>! WRONG COMMAND <%s>!\n", command);
            dump_Comm(arr_comm, arr_label, comm_size, i_comm);
            exit(1488);
        }

        arr_comm[i_comm] = CMD_END;
        rewind(input);
        i_comm = 0;
    }

    dump_Comm(arr_comm, arr_label, comm_size, 0);

}

int getLabel(label_t arr_label[], char target[])
{
    assert(arr_label != NULL);

    for (int cnt_label = 0; cnt_label < LABEL_LIM; ++cnt_label)
    {
        assert(0 <= cnt_label && cnt_label < LABEL_LIM);
        if (!strcmp(target, arr_label[cnt_label].name))
            return cnt_label;
    }

    return -1;
}

int getEnd(double arr_comm[], int comm_size)
{
    for (int i = 0; i < comm_size; ++i)
    {
        assert(0 <= i && i < comm_size);
        if (arr_comm[i] == CMD_END)
            return i;
    }
    printf("UNABLE TO FIND END CODE!\n");
    exit(10);
}

