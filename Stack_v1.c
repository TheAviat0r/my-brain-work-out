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
//!===========================================
#define assert_ok { if(!cond) {\
                                printf("FAIL: %s, in %s,%d, %s \n", #cond, \
                                __FILE__, __LINE__, __PRETTY_FUNCTION__);\
                                dump_ok(mystack);\
                                abort();}
#define SLASHES printf("===============================================\n");
#define BUF work->buffer
#define CNT work->cnt
#define EMPT printf("\n");
//!===========================================
const int YES = 1;
const int NO = 0;
const int STARTMEM = 5;
//!===========================================
const char EXIT [] =    "exit";
const char PUSH [] =    "push";
const char POP [] =     "pop";
const char CLEAR [] =   "clear";
const char DIV [] =     "div";
const char PLUS [] =    "plus";
const char MULT [] =    "mult";
const char NSUM [] =    "nsum";
const char START [] =   "start";
//!===========================================
struct stack_t
{
    int stackSZ;
    int cnt;
    int flag;
    int freemem;
    double *buffer;
};
//!===========================================
void dump_ok(stack_t *work);
//!===========================================
int stack_ok(stack_t *work);
//!===========================================
void addMem (stack_t *work);
//!===========================================
void pushData (stack_t *work, double numb);
//!===========================================
void popData (stack_t *work);
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
void FAQ(void);
//!===========================================
int main()
{
    SLASHES;
    printf("Aviator MIPT, %s, %s\n", __DATE__, __TIME__);
    SLASHES;
    printf("Hello! I am the most perfect stack that you ever seen!\n");
    SLASHES;
    //!===========================================
    stack_t mystack = {};
    mystack.stackSZ = STARTMEM;
    mystack.freemem = STARTMEM; //rightly down we create start memory
    //!===========================================
    mystack.buffer = (double *) calloc(mystack.stackSZ, sizeof(double *));
    //!===========================================
    printf("Lets start our program! \n");
    printf("If you want to see commands list - type 1, if no - type 2\n");
    //!===========================================
    int value = 0;
    scanf("%d", &value);
    //!===========================================
    switch(value)
    {
        case 1:     FAQ();
            break;
        case 2:     printf("OK, here we go!\n");
            break;
        default:    printf("Wrong value! If you want FAQ - type 2, if no - type 1\n");
            scanf("%d", &value);
            if (value ==2)
                FAQ();
            else
                printf("OK! No problem, its not serious, lets go!\n");
            break;
    }
    SLASHES;
    //!===========================================
    printf("buffer[%d] = %d [%d]\n", mystack.cnt, *(mystack.buffer)S, mystack.buffer);
    printf("Type start to begin\n");
    char command[10] = {};
    double numb = 0;
    scanf("%s", command);

    if (!strcmp(command, START))
    {
        printf("\n{\n");
        //!===========================================
        while (strcmp(command, EXIT))
        {
            if (!strcmp(command, PUSH))
            {
                if (scanf("%lf", &numb) == 1)
                    pushData(&mystack, numb);
                else
                    printf("Wrong input! Number type should be double!\n");
                scanf("%s", command);
                continue;
            }
            if (!strcmp(command, POP))
            {
                popData(&mystack);
                scanf("%s", command);
                continue;
            }
            if (!strcmp(command, CLEAR))
            {
                delData(&mystack);
                scanf("%s", command);
                continue;
            }
            if (!strcmp(command, DIV))
            {
                divData(&mystack);
                scanf("%s", command);
                continue;
            }
            if (!strcmp(command, PLUS))
            {
                plusData(&mystack);
                scanf("%s", command);
                continue;
            }
            if (!strcmp(command, NSUM))
            {
                nsumData(&mystack);
                scanf("%s", command);
                continue;
            }
            if (!strcmp(command, MULT))
            {
                multData(&mystack);
                scanf("%s", command);
                continue;
            }



            scanf("%s", command);
        }
    }
    //!===========================================




    //addMem(&mystack);
    //dump_ok(&mystack);

    return 0;
}
//!===========================================
void dump_ok(stack_t *work)
{
    EMPT
    SLASHES;
    printf("I AM THE DUMPER!!!\n");

    //work->cnt = 0;
    work->flag = 0;
    printf("{\n");
    printf("    buffer adress = [0x%p]\n", work->buffer);
    printf("    stackSZ = <%d>\n", work->stackSZ);
    printf("    freemem = <%d>\n", work->freemem);
    printf("    cnt = <%d>\n", work->cnt);

    EMPT;
    for (work->flag = 0; work->flag < (work->stackSZ); ++work->flag)
    {
        printf("    buffer[%d] [%p] = (%lg)\n", work->flag, (work->buffer + work->flag), work->buffer[work->cnt]);
        //printf("    flag = %d\n", work->flag);
    }
    //printf("}\n");
    SLASHES;
    EMPT;
}

//!===========================================
int stack_ok(stack_t *work)
{
    SLASHES;
    printf("{\n");
    printf("    this is stack_ok\n");
    if (BUF == 0)
    {
        printf("ERR#1\n");
        dump_ok(work);
        return 1;
    }
    if (work->freemem < 0)
    {
        printf("ERR#2\n");
        dump_ok(work);
        return 2;
    }
    if (CNT < 0)
    {
        printf("ERR#3\n");
        dump_ok(work);
        return 3;
    }
    dump_ok(work);
    printf("}\n");
    return 0;
}
//!===========================================
void addMem(stack_t *work)
{
    EMPT;
    SLASHES;
    printf("I AM addMem!\n");
    SLASHES;

    printf("old size = %d\n", work -> stackSZ);
    printf("old freemem = %d\n", work -> freemem);

    work->freemem += work->stackSZ;
    work->stackSZ *= 2;

    printf("new size = %d\n", work -> stackSZ);
    printf("new freemem = %d\n", work -> freemem);
    EMPT;

    printf("old buffer = [0x%p]\n", work->buffer);
    work->buffer = (double *) realloc(work->buffer, work->stackSZ);
    printf("new buffer = [0x%p]\n", work->buffer);

    SLASHES;
    EMPT;
}
//!===========================================
void FAQ(void)
{
    EMPT;
    SLASHES;
    printf("        Available commands:\n");
    SLASHES;
    printf("push    - add element to stack\n");
    printf("pop     - delete element from stack\n");
    printf("clear   - clean all stack (all elements will become 0)\n");
    printf("div     - division last element\n");
    printf("mult    - multiply operation\n");
    printf("plus    - sum operation\n");
    printf("nsum    - minus operation\n");
    printf("exit    - end program\n");
    SLASHES;
    EMPT;
}
//!===========================================
void pushData(stack_t *work, double numb)
{
    SLASHES;
    printf("This is pushData\n");
    stack_ok(work);
    printf("I push <%lg> into stack\n", numb);

    if (work->freemem > 0 )
    {
        work->buffer[work->cnt] = numb;
        work->freemem--;
        work->cnt++;
    }

    printf("new binary = <%lg> \n", work->buffer[work->cnt]);

    stack_ok(work);

}
//!===========================================
void popData(stack_t *work)
{
}
//!===========================================
void delData(stack_t *work)
{
}
//!===========================================
void divData(stack_t *work)
{
}
//!===========================================
void plusData(stack_t *work)
{
}
//!===========================================
void nsumData(stack_t *work)
{
}
//!===========================================
void multData(stack_t *work)
{
}


