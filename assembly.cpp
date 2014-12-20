#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <io.h>
#include <assert.h>
//!==================================================================
#define SLASHES printf("---------------------------------------------\n");
#define FSLASHES fprintf(output, "---------------------------------------------\n");
#define FEMPT fprintf(output, "\n");
#define EMPT printf("\n");
#define streq(name, arg) !strcmp(name, arg)
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
const int STRLIM = 100;
const int VARLIM = 100;
const int MAXCMDLEN = 20;
const int NOEXIST   = -1;
const int NUMBER_TAG = 1;
const int VAR_TAG = 2;
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
const char str_IN[] =       "in";
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
const char str_VAR[] =      "var";
const char str_PUSHV[] =    "pushv";
const char str_POPV[] =     "popv";
const char str_OUTV[] =     "outv";
const char COMMAND[] =      "commands:";
//}==================================================================
struct label_t
{
    char name[20];
    int adress;
};
//!----------------------------------------
struct stroke_t
{
    char symb[STRLIM];
};
//!----------------------------------------
struct vars_t
{
    stroke_t data[VARLIM];
    unsigned int cnt;
};
//!----------------------------------------
vars_t G_Var = {};
//!----------------------------------------
//!==================================================================
void ProcessAsm(void);
//!==================================================================
    int cntComm(FILE *input, FILE *output);
    void dump_Comm(double *buffer, int buffer_len, int position);
    void formCommands(FILE *input, double arr_comm[], label_t arr_label[], int comm_size);
    void initiateLabels(label_t arr_label[]);
    void treatCall(FILE *input, char command[], double arr_comm[],  int comm_size, int * i_comm, int pass,\
                                                                    label_t arr_label[]);
    void treatLabel(FILE *input, char command[], int command_len,   int pass, int * cnt_label,\
                                                                    label_t arr_label[], int * i_comm);
    void treatVar (FILE *input, char command[], int pass, double arr_comm[], int * i_comm, int comm_size);
    void treatPushPopV (FILE *input, char command[], int pass, double arr_comm[], int * i_comm, int comm_size);

    void dumpVar(FILE *output);
    int searchVar(char word[]);
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
        //!----------------------------------------
        FILE *output = fopen("asmed.txt", "w");
        //!----------------------------------------
        int comm_size = 4 * cntComm(input, output) + 1;
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

        dump_Comm (arr_comm, arr_label, comm_size, end_position);
        //!--------------------------------------
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
//!----------------------------------
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
//!------------------------------------
//!     formCommands() reads commands from txt and fills it in an array of commands
//!------------------------------------
void formCommands(FILE *input, double arr_comm[], label_t arr_label[], int comm_size)
{
    assert(arr_comm != NULL && input != NULL && comm_size != NULL && arr_label != NULL);
    //!            INITIALIZE WORKING VARIABLES
    //{===============================================
    char command[MAXCMDLEN] = {};
    char reg[MAXCMDLEN] = {};
    char label_name [MAXCMDLEN] = {};
    char call_arg [MAXCMDLEN] = {};
    char jump_label [MAXCMDLEN] = {};

    double push_value = 0;
    int i_comm = 0;
    int cnt_label = 0;
    int command_len = 0;
    //}===============================================

    //!----------------------------------
    #include "asm_defines.h"
    //!----------------------------------
    for (int pass = 1; pass <= 2; ++pass)
    {
        assert(1 <= pass && pass <= 2);
        fscanf(input, "%s", command);
        //!----------------------------------------------------------
        while (!streq(command, str_END))
        {
            //printf("command = (%s)\n", command);
            command_len = strlen(command);
            assert(command_len > 0);
            if (command_len <= 1)
            {
                printf("WRONG INPUT! COMMAND LENGTH SHOULD BE > 1\n");
                exit(10);
            }
            //!----------------------------------------------------------
            //dump_Comm(arr_comm, arr_label, comm_size, i_comm);
            assert(0 <= i_comm && i_comm < comm_size);
            //!----------------------------------------------------------
            if (streq(command, str_PUSH))
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

            if (streq(command, str_VAR))
            {
                treatVar(input, command, pass, arr_comm, &i_comm, comm_size);
                continue;
            }

            if (streq(command, str_PUSHV) || streq(command, str_POPV) || streq(command, str_OUTV))
            {
                treatPushPopV(input, command, pass, arr_comm, &i_comm, comm_size);
                continue;
            }

            //!----------------------------------------------------------
            CMP(PUSHR)
            CMP(POP)
            CMP(OUT)
            CMP(IN)

            //{!============<OPERATIONS WITHOUT ARGUMENTS>===============
            OPER(command, ADD)
            OPER(command, MIN)
            OPER(command, MUL)
            OPER(command, DIV)
            OPER(command, SQR)
            OPER(command, SQRT)
            OPER(command, RET)
            //}!============================================================

            if (command[command_len - 1] == ':')
            {
                treatLabel(input, command, command_len, pass, &cnt_label, arr_label, &i_comm);
                continue;
            }

            if (streq(command, str_CALL))
            {
                treatCall(input, command, arr_comm, comm_size, &i_comm, pass, arr_label);
                continue;
            }
            //{====================================================================
            GETJMP(JMP)
            GETJMP(JB)
            GETJMP(JBC)
            GETJMP(JA)
            GETJMP(JAC)
            GETJMP(JE)
            GETJMP(JNE)
            //}========================================================================
            SLASHES DBG printf("ERR_<command_input>! WRONG COMMAND <%s>!\n", command);
            dump_Comm(arr_comm, arr_label, comm_size, i_comm);
            exit(1488);
        }

        arr_comm[i_comm] = CMD_END;
        rewind(input);
        i_comm = 0;
    }
    #undef PROCESSREG
    #undef OPER
    #undef GETJMP
    #undef CMP
    DBG printf("commands are formed!\n");
    dumpVar(stdout);
    //dump_Comm(arr_comm, arr_label, comm_size, 0);
}

void dumpVar(FILE *output)
{
    assert(output);
    FEMPT FSLASHES fprintf(output, "\t VARS DUMP\n");
    FSLASHES fprintf(output, "Var.cnt = %u\n", G_Var.cnt);

    FSLASHES
    for (int i = 0; i < G_Var.cnt; i++)
    {
        assert(0 <= i && i < G_Var.cnt);
        fprintf(output, "%d) <%s>\n", i, G_Var.data[i].symb);
    }
}

int searchVar(char word[])
{
    assert(word);

    for (int i = 0; i < G_Var.cnt; i++)
    {
        assert(0 <= i && i < G_Var.cnt);
        //printf("G_var[%d] (%s) vs (%s)\n", i, G_Var.data[i].symb, word);
        if (strcmp(G_Var.data[i].symb, word) == NULL)
            return i;
    }

    return NOEXIST;
}

void treatCall(FILE *input, char command[], double arr_comm[],  int comm_size, int * i_comm, int pass,\
                                                                label_t arr_label[])
{
    assert(input != NULL);
    assert(command != NULL);
    assert(arr_comm != NULL);
    assert(comm_size != NULL);
    assert(i_comm != NULL);
    assert(pass != NULL);
    assert(arr_label != NULL);

    char call_arg[MAXCMDLEN] = {};
    if (fscanf(input, "%s", call_arg) == 1)
    {
        arr_comm[*i_comm] = CMD_CALL;
        ++*i_comm;

        assert(0 <= *i_comm && *i_comm <= comm_size);
        int callNum = getLabel(arr_label, call_arg);

        if (callNum == NOEXIST && pass == 2)
        {
            SLASHES DBG printf("ERR_<call_label>! Unable to find label <%s>!\n", call_arg);
            exit(CMD_CALL);
        }

        if (callNum >= 0 && arr_label[callNum].adress != NOEXIST)
            arr_comm[*i_comm] = arr_label[callNum].adress;
        else
            arr_comm[*i_comm] = NOEXIST;

        ++*i_comm;

        fscanf(input, "%s", command);
        return;
    }
}

void treatVar (FILE *input, char command[], int pass, double arr_comm[], int * i_comm, int comm_size)
{
    assert(input);
    assert(command);
    assert(pass == 1 || pass == 2);
    assert(arr_comm);
    assert(i_comm);

    //arr_comm[*i_comm] = CMD_VAR;
    /*(*i_comm)++;*/

    assert(0 <= *i_comm && *i_comm <= comm_size);

    fscanf(input, "%s", command);

    if (pass == 1)
    {
        if (searchVar(command) != NOEXIST)
        {
            printf("ERROR! Redefenition of %s!\n", command);
            dumpVar(stdout);
            exit(3);
        }

        if (isalpha(command[0]))
        {
            strcpy(G_Var.data[G_Var.cnt].symb, command);
            //arr_comm[*i_comm] = G_Var.cnt;
            G_Var.cnt++;
        }
        else
        {
            printf("ERROR! Commands should begin with alpha! But not like this '%s'\n", command);
            dumpVar(stdout);
            exit(3);
        }
    }

    /*(*i_comm)++;*/
    fscanf(input, "%s", command);
}

void treatPushPopV (FILE *input, char command[], int pass, double arr_comm[], int * i_comm, int comm_size)
{
    assert(input);
    assert(command);
    assert(pass == 1 || pass == 2);
    assert(arr_comm);
    assert(i_comm);
    assert(comm_size);

    switch(command[2])
    {
        case 's':
            arr_comm[*i_comm] = CMD_PUSHV;
            (*i_comm)++;
            break;
        case 'p':
            arr_comm[*i_comm] = CMD_POPV;
            (*i_comm)++;
            break;
        case 't':
            arr_comm[*i_comm] = CMD_OUTV;
            (*i_comm)++;
            break;
        default:
            printf("ERROR! FAIL IN COMPARISON BETWEEN PUSHV AND POPV!\n");
            exit(4);
            break;
    }

    assert(0 <= *i_comm && *i_comm <= comm_size);

    fscanf(input, "%s", command);
    if (searchVar(command) == NOEXIST)
    {
        printf("ERROR! Undeclared var %s!\n", command);
        dumpVar(stdout);
        exit(4);
    }

    arr_comm[*i_comm] = searchVar(command);
    (*i_comm)++;

    int position = 0;
    int check = fscanf(input, "%d", &position);
    if (check)
    {
        arr_comm[*i_comm] = NUMBER_TAG;
        (*i_comm)++;
        arr_comm[*i_comm] = position;
        (*i_comm)++;
        /*printf("ERROR! Unable to scan position!\n");
        dumpVar(stdout);
        exit(4);*/
    }
    else
    {
        arr_comm[*i_comm] = VAR_TAG;
        (*i_comm)++;
        fscanf(input, "%s", command);
        arr_comm[*i_comm] = searchVar(command);
        (*i_comm)++;
    }

    //printf("next check is passed!\n");

    //arr_comm[*i_comm] = position;
    /*(*i_comm)++;*/

    fscanf(input, "%s", command);
}

void treatLabel(FILE *input,    char command[], int command_len, int pass,\
                                int * cnt_label, label_t arr_label[], int * i_comm)
{
    assert (input != NULL && command_len != NULL && pass != NULL);
    assert (cnt_label != NULL && *cnt_label >= 0 && arr_label != NULL && i_comm != NULL);

    if (command[0] != NULL)
    {
        if (pass == 1)
        {
            if (*cnt_label < LABEL_LIM)
            {
                strncpy(arr_label[*cnt_label].name, command, command_len - 1);
                arr_label[*cnt_label].adress = *i_comm;
                ++*cnt_label;

                fscanf(input, "%s", command);
                return;
            }
            else
            {
                printf("ERROR! ARRAY OF LABELS IS FULL! RECOMPILE PROGRAM!\n");
                exit(1488);
            }
        }

        fscanf(input, "%s", command);
        return;
    }
    else
    {
        SLASHES DBG printf("ERR_<label_argument>! UNABLE TO READ LABEL!\n");
        exit(1488);
    }
}


int getLabel(label_t arr_label[], char target[])
{
    assert(arr_label != NULL && target != NULL);

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

