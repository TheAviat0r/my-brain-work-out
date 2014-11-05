
//!============================================================================
//!     @file       Stack_v1.c
//!     @author     Niatshin Bulat
//!     @task       Write stack with most useful operations like multuply, add and so on
//!     @condition  I don't know what should I write here
//!     @errors     ERR#1 - buffer adress is ZERO
//!                 ERR#2 - buffer is overfilled
//!                 ERR#3 - out of memory area
//!                 ERR#4 - input error
//!============================================================================
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>
//!===========================================
#define DEBUG
#define MINE
#ifdef DEBUG
#define assert_ok(cond) if(!(cond)){\
                        printf("FAIL: %s in %s, %d %;", \
                        #cond, __FILE__, __LINE__, __PRETTY_FUNCTION__);\
                        EMPT; dump_ok(work);};
#else #define assert_ok(cond)
#endif

#ifndef NOCALLDBG
    #define assert_call(cond)   if(!(cond))\
                                {\
                                    printf("FAIL: %s in %s, %d %;", \
                                    #cond, __FILE__, __LINE__, __PRETTY_FUNCTION__);\
                                    EMPT; dump_call(work);\
                                };
#else #define assert_call(cond)
#endif

#define SLASHES printf("===============================================\n");
#ifdef MINE
    #define DGB printf("#\n");
#else
    #define DBG
#endif
#define EMPT printf("\n");
#define BUF work->buffer
#define CNT work->cnt
#define EMPT printf("\n");
//!===========================================
#define DEF_CMD(name, num, code) CMD_##name = num,
enum Commands_t
{
    #include "command.h"
    CMD_MAX
};
#undef DEF_CMD
//!===========================================
const int YES = 1;
const int NO = 0;
const int STARTMEM = 5;
const int ax_code = 100499;
const int bx_code = 100498;
const int cx_code = 100497;
const int dx_code = 100496;
const int ex_code = 100495;
const int fx_code = 100494;
//!===========================================
const char COMMAND[] =      "commands:";
//!===========================================
struct stack_t
{
    int stackSZ;
    int cnt;
    int freemem;
    int dbg;
    double *buffer;

    double *reger;
};

struct label_t
{
    int stackSZ;
    int cnt;
    int freemem;
    int *buffer;
};
//!===========================================
//!             THIS IS CPU FUNCTIONS!
void processComm (void);
//!===========================================
void formCommands (FILE *input, double list_comm[], int list_size);
//!===========================================
void dump_Comm (FILE *input, double list_comm[], int list_size, int position);
//!===========================================
int get_endPosition(double list_comm[], int list_size);

void pushCall(label_t *work, int call_point);
int popCall(label_t *work);
void ctor (label_t *work);
void dtor (label_t *work);
void dump_call (label_t *work);
int call_ok(label_t *work);
//!===========================================
//!             THIS IS STACK FUNCTIONS!
//!===========================================
void dump_ok (stack_t *work);
//!===========================================
int stack_ok (stack_t *work);
//!===========================================
void addMem (stack_t *work);
//!===========================================
void pushData (stack_t *work, double numb);
//!===========================================
double popData (stack_t *work);
//!===========================================
void delData (stack_t *work);
//!===========================================
void divData (stack_t *work);
//!===========================================
void multData (stack_t *work);
//!===========================================
void plusData (stack_t *work);
//!===========================================
void nsumData (stack_t *work);
//!===========================================
void showData (stack_t *work);
//!===========================================
void FAQ(void);
//!===========================================
void greeting(void);
//!===========================================
void resData (stack_t *work);
//!===========================================
void sumData (stack_t *work);
//!===========================================
void sqrData (stack_t *work);
//!===========================================
void sqrtData (stack_t *work);
//!===========================================
int main()
{
    greeting();
    processComm();
    return 0;
}
void processComm(void)
{
    FILE *input = 0;
    //!===========================================
    if (input = fopen("asmed.txt", "r"))
    {
        SLASHES;
        printf("FILE asmed.txt successfully opened!\n");
        SLASHES; EMPT;
        //!             INITIALIZE STACK
        //{===========================================
        stack_t mystack = {};
        mystack.stackSZ = STARTMEM;
        mystack.freemem = STARTMEM;
        mystack.buffer = (double *) calloc(STARTMEM, sizeof(double));
        mystack.reger = (double *) calloc(6, sizeof(double));
        assert(mystack.buffer);
        //}===========================================
        //!             INITIALIZE WORKING VALUES
        //{===========================================
        int command = 0;
        int num_command = 0;
        int reg = 0;
        double value = 0;
        char seek[20] = {};
        int list_size = 0;
        //}===========================================
        double * list_comm = 0;
        //!===========================================
        //!===========================================
        fscanf(input, "%s", seek);
        if (strcmp(seek, "commands:") == NULL)
        {
            fscanf(input, "%d", &num_command);
            printf("num_comm = %d\n", num_command);

            list_size = 2*num_command + 1;
            list_comm = (double *) calloc(list_size, sizeof(double));
        }
        else
        {
            SLASHES;
            printf("### WRONG INPUT! Unable to scan <num_command>! ###\n");
            SLASHES;
            exit(1);
        }
        //!===========================================
        formCommands(input, list_comm, list_size);
        //!===========================================
        int position = 0;
        int end_position = get_endPosition(list_comm, list_size);
        command = list_comm[position];
        SLASHES
        //{===========================================
        label_t call_stack = {};
        call_stack.stackSZ = STARTMEM;
        call_stack.freemem = STARTMEM;
        call_stack.buffer = (int *) calloc(STARTMEM+1, sizeof(int));
        //}===========================================

        #define DEF_CMD(name, num, code) case CMD_##name: {code}; break;
        while (position < end_position)
        {
            //printf("position in cycle = <%d>\n", position);
            //printf("command in cycle = <%d>\n", command);
            assert(0 <= position && position <= list_size);

            switch(command)
            {
                #include "command.h"
                default:
                    printf("unable to process command <%d>! Program is TERMINATED!\n", command);
                    dump_Comm(input, list_comm, list_size, position);
                    exit(1);
                    break;
            }
            command = list_comm[position];
        }
        #undef DEF_CMD
        dump_ok(&mystack);
        dump_call(&call_stack);

        free(mystack.buffer);
        free(call_stack.buffer);
    }
    else
    {
        printf("ERROR! Unable to open asmed.txt! Try again\n");
    }
}
//{!============================================================
//!    formCommands opens "input.txt" and
//!    creates an array of commands written as codes, not words
//}!============================================================
void formCommands(FILE *input, double list_comm[], int list_size)
{
    assert(list_comm != NULL);
    printf("list_size = %d\n", list_size);
    //!            INITIALIZE WORKING VARIABLES
    //{===============================================
    double command = 0;
    int cnt_list = 0;
    //}===============================================

    fscanf(input, "%lf", &command);
    SLASHES
    while (command != CMD_END)
    {
        //printf("cycle commmand = <%d>\n", command);
        list_comm[cnt_list] = command;
        cnt_list++;
        fscanf(input, "%lf", &command);
    }

    list_comm[cnt_list] = CMD_END;

    //dump_Comm(input, list_comm, list_size, 0);
}
//!===========================================
int get_endPosition(double list_comm[], int list_size)
{
    for (int i = 0; i < list_size; ++i)
    {
        assert (0 <= i && i < list_size);
        if (list_comm[i] == CMD_END)
            return i;
    }
}
//!===========================================
void dump_Comm(FILE *input, double list_comm[], int list_size, int position)
{
    EMPT
    SLASHES
    printf("\tDUMP OF ARRAY OF COMMANDS\n"); SLASHES
    printf("FILE *input = [%p]\n", input);
    printf("list_size = <%d>\n", list_size);
    SLASHES;

    EMPT
    for (int i = 0; i < list_size; ++i)
    {
        assert(0 <= i && i < list_size);
        if (i < 10)
        {
            if (i != position)
                printf("  comm  [%d] = <%lg> \n", i, list_comm[i]);
            else
                printf("->comm  [%d] = <%lg> \n", i, list_comm[i]);
        }
        else
        {
            if (i != position)
                printf("  comm [%d] = <%lg> \n", i, list_comm[i]);
            else
                printf("->comm [%d] = <%lg> \n", i, list_comm[i]);
        }
    }
    SLASHES EMPT
}
//!===========================================

void pushCall(label_t *work, int call_point)
{
    assert_call(call_ok(work) == NULL);
    if (work->freemem > 0)
    {
        *(work->buffer + work->cnt)= call_point;
        work->freemem--;
        work->cnt++;
    }
    else
    {
        ctor(work);
        work->buffer[work->cnt]= call_point;
        work->freemem--;
        work->cnt++;
    }
    assert_call(call_ok(work) == NULL);
}

int popCall(label_t *work)
{
    assert_call(call_ok(work) == NULL);
    if (work->cnt > 0)
    {
        double save = work->buffer[work->cnt - 1];
        work->buffer[work->cnt - 1] = 0;
        work->cnt--;
        work->freemem++;
        return save;
    }
    else
    {
        printf("<CALL STACK> is empty! No anything to pop!\n");
        return 0;
    }
    assert_call(call_ok(work) == NULL);
}

void ctor(label_t *work)
{
    assert_call(call_ok(work) == NULL);

    work->freemem += work->stackSZ;
    work->stackSZ *= 2;

    work->buffer = (int *) realloc(work->buffer, (work->stackSZ)*sizeof(int));
    for (int i = work->cnt; i < work->stackSZ; ++i)
        work->buffer[i]= 0;

    assert_call(call_ok(work) == NULL);
}

void dtor(label_t *work)
{
    free(work->buffer);
}

int call_ok(label_t *work)
{
    if (work->buffer == 0)
    {
        SLASHES;
        printf("ERR#1\n");
        return 1;
    }
    if (work->freemem < 0)
    {
        SLASHES;
        printf("ERR#2\n");
        return 2;
    }
    if (CNT < -1)
    {
        SLASHES;
        printf("ERR#3\n");
        return 3;
    }

    return 0;
}

void dump_call(label_t *work)
{
    EMPT SLASHES
    printf("\tDUMP OF CALLS\n");
    SLASHES

    printf("   buffer = [0x%p]\n", work->buffer);
    printf("   callSZ = <%d>\n", work->stackSZ);
    printf("   cnt = <%d>\n", work->cnt);
    SLASHES

    for (int i = 0; i < work->cnt; i++)
    {
        assert(0 <= i && i < work->cnt);
        printf("    ->call[%d] [0x%p] = (%d)\n", i, work->buffer + i, work->buffer[i]);
    }
}

void greeting(void)
{
    SLASHES;
    printf("Aviator MIPT, %s, %s\n", __DATE__, __TIME__);
    SLASHES;
    printf("Hello! I am the model of CPU!\n");
    SLASHES;
}
//!===========================================
void dump_ok(stack_t *work)
{
    EMPT
    SLASHES;
    printf("I AM THE DUMPER!!!\n");

    //work->cnt = 0;
    printf("{\n");
    printf("      buffer adress = [0x%p]\n", work->buffer);
    printf("      stackSZ = <%d>\n", work->stackSZ);
    printf("      cnt = <%d>\n", work->cnt);
    printf("      ax = <%lg>\n", work->reger[0]);
    printf("      bx = <%lg>\n", work->reger[1]);
    printf("      cx = <%lg>\n", work->reger[2]);
    printf("      dx = <%lg>\n", work->reger[3]);
    printf("      ex = <%lg>\n", work->reger[4]);
    printf("      fx = <%lg>\n", work->reger[5]);

    EMPT;
    for (int i = 0; i < (work->cnt); ++i)
    {
        //assert_ok(0 <= work.flag && work->flag < work->stackSZ);
        printf("    ->buffer[%d] [0x%p] = (%lg)\n", i, work->buffer + i, work->buffer[i]);
        //printf("    flag = %d\n", work->flag);
    }
    for (int i = work->cnt; i < work->stackSZ; ++i)
    {
        printf("      buffer[%d] [0x%p] = (%lg)\n", i, work->buffer + i, work->buffer[i]);
    }
    printf("}\n");
    SLASHES;
    EMPT;
}
//!===========================================
int stack_ok(stack_t *work)
{
    //SLASHES;
    //printf("    this is stack_ok\n");
    if (BUF == 0)
    {
        SLASHES;
        printf("ERR#1\n");
        return 1;
    }
    if (work->freemem < 0)
    {
        SLASHES;
        printf("ERR#2\n");
        return 2;
    }
    if (CNT < -1)
    {
        SLASHES;
        printf("ERR#3\n");
        return 3;
    }
    if (work->dbg)
        dump_ok(work);
    return 0;
}
//!===========================================
void addMem(stack_t *work)
{
    //EMPT;
    //SLASHES;
    //printf("I AM addMem!\n");
    //SLASHES;
    stack_ok(work);

    work->freemem += work->stackSZ;
    work->stackSZ *= 2;

    work->buffer = (double *) realloc(work->buffer, (work->stackSZ)*sizeof(double));
    for (int i = work->cnt; i < work->stackSZ; ++i)
        *(work->buffer + i) = 0;

    stack_ok(work);

    //SLASHES;
    //EMPT;
}
//!===========================================
void pushData(stack_t *work, double numb)
{
    assert(stack_ok(work) == NULL);
    if (work->freemem > 0 )
    {
        *(work->buffer + work->cnt) = numb;
        work->freemem--;
        work->cnt++;
    }
    else
    {
        addMem(work);
        *(work->buffer + work->cnt) = numb;
        work->freemem--;
        work->cnt++;
    }
    assert(stack_ok(work) == NULL);
}
//!===========================================
double popData(stack_t *work)
{
    assert_ok(stack_ok(work) == NULL);
    if (work->cnt > 0)
    {
        double save = work->buffer[work->cnt - 1];
        *(work->buffer + work->cnt-1) = 0;
        work->cnt--;
        work->freemem++;
        return save;
    }
    else
    {
        printf("Stack is empty! No anything to pop!\n");
        return 0;
    }
    assert_ok(stack_ok(work) == NULL);
}
//!===========================================
void delData(stack_t *work)
{
    //work->buffer = 0;
    assert_ok(stack_ok(work) == NULL);
    printf("# Stack is empty now!\n");

    work->freemem = STARTMEM;
    work->stackSZ = STARTMEM;
    work->cnt = 0;
    work->buffer = (double *)realloc(work->buffer, STARTMEM*sizeof(double));;;;

    for (int i = 0; i < work->stackSZ; ++i)
    {
        assert(0 <= i && i < work->stackSZ);
        work->buffer[i] = 0;
    }
    assert_ok(stack_ok(work) == NULL);
}
//!===========================================
void divData(stack_t *work)
{
    stack_ok(work);
    if (work->cnt == 1 || work->cnt == 0)
    {
        printf("#Stack is empty! Unable to do division\n");
    }
    else
    {
        if (work->buffer[work->cnt -1] != NULL)
        {
            double first = popData(work);
            double second = popData(work);
            pushData(work, second/first);
        }
        else
        {
            printf("ERROR! Division by zero! Try again!\n");
            popData(work);
        }
    }

}
//!===========================================
void plusData(stack_t *work)
{
    if (work->cnt == 1 || work->cnt == 0)
    {
        printf("#Stack is empty! Unable to add\n");
    }
    else
    {
        pushData(work, popData(work) + popData(work));
    }
}
//!===========================================
void nsumData(stack_t *work)
{
    if (work->cnt == 1 || work->cnt == 0)
    {
        printf("#Stack is empty! Unable to minus\n");
    }
    else
    {
        assert(stack_ok(work) == NULL);
        //printf("this is nsum!\n");
        //printf("a = <%lg> b = <%lg>\n", work->buffer[work->cnt - 2], work->buffer[work->cnt - 1]);
        double first = popData(work);
        double second = popData(work);

        pushData(work, second - first);
        assert(stack_ok(work) == NULL);
    }
}
//!===========================================
void multData(stack_t *work)
{
    assert(stack_ok(work) == NULL);
    if (work->cnt == 1 || work->cnt == 0)
    {
        printf("#Stack is empty! Unable to multiply\n!");
    }
    else
    {
        pushData(work, popData(work) * popData(work));
        //resData(work);
    }
    assert(stack_ok(work) == NULL);
}
//!===========================================
void showData(stack_t *work)
{
    //printf("this is show data\n");
    SLASHES;
    assert(stack_ok(work) == NULL);
    for (int i = 0; i < work->cnt; i++)
    {
        //assert(0 <= i && i < work->cnt);
        printf("b[%d] = <%lg>\n", i, work->buffer[i]);
    }
    SLASHES;
}
//!===========================================
void sumData(stack_t *work)
{
    double sum = 0;

    assert(stack_ok(work) == NULL);
    for (int i = work->cnt; i > 0; --i)
    {
        assert_ok(0 < i && i <= work->cnt);
        sum += popData(work);
    }
    assert(stack_ok(work) == NULL);

    pushData(work, sum);
    //resData(work);
}
//!===========================================
void sqrData(stack_t *work)
{
    //printf("this is sqrData\n");
    assert(stack_ok(work) == NULL);

    if (work->cnt > 0)
        pushData(work, pow(popData(work), 2));
    else
        printf("Stack is empty! Unable to square the value!\n");
    assert(stack_ok(work) == NULL);
}
//!===========================================
void sqrtData(stack_t *work)
{
    double value = popData(work);
    if (value >= 0)
    {
        assert(stack_ok(work) == NULL);
        pushData(work, sqrt(value));
        assert(stack_ok(work) == NULL);
    }
    else
    {
        printf("Unable to get root!\nValue is negative\n");
        printf("This negative value will be deleted!\n");
    }
}
