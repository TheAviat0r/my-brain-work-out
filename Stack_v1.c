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
//#define assert_ok { if(!cond) {\
                                printf("FAIL: %s, in %s,%d, %s \n", #cond, \
                                __FILE__, __LINE__, __PRETTY_FUNCTION__);\
                                dump_ok(&mystack);\
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
const char LIST [] =    "list";
const char DUMP [] =    "dump";
const char SHOW [] =    "show";
const char SUM []  =    "sumup";
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
void showData (stack_t *work);
//!===========================================
void FAQ(void);
//!===========================================
void resData (stack_t *work);
//!===========================================
void sumData (stack_t *work);
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
    //!printf("stack size = <%d>\n", mystack.stackSZ);
    mystack.buffer = (double *) calloc(5, sizeof(double));
    //dump_ok(&mystack);
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
            if (!strcmp(command, LIST))
            {
                FAQ();
                scanf("%s", command);
                continue;
            }
            if (!strcmp(command, DUMP))
            {
                dump_ok(&mystack);
                scanf("%s", command);
                continue;
            }
            if (!strcmp(command, SHOW))
            {
                showData(&mystack);
                scanf("%s", command);
                continue;
            }
            if (!strcmp(command, SUM))
            {
                sumData(&mystack);
                scanf("%s", command);
                continue;
            }

            if (strcmp(command, START))
                printf("#Incorrect command! Try again! If you need help - type list\n");
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
    printf("      buffer adress = [0x%p]\n", work->buffer);
    printf("      stackSZ = <%d>\n", work->stackSZ);
    printf("      freemem = <%d>\n", work->freemem);
    printf("      cnt = <%d>\n", work->cnt);

    EMPT;
    for (work->flag = 0; work->flag < (work->cnt); ++work->flag)
    {
        //assert_ok(0 <= work.flag && work->flag < work->stackSZ);
        printf("    ->buffer[%d] [0x%p] = (%lg)\n", work->flag, work->buffer + work->flag, *(work->buffer + work->flag));
        //printf("    flag = %d\n", work->flag);
    }
    for (work->flag = work->cnt; work->flag < work->stackSZ; ++work->flag)
    {
        printf("      buffer[%d] [0x%p] = (%lg)\n", work->flag, work->buffer + work->flag, *(work->buffer + work->flag));
    }
    printf("}\n");
    work->flag = 0;
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
        dump_ok(work);
        abort();
    }
    if (work->freemem < 0)
    {
        SLASHES;
        printf("ERR#2\n");
        dump_ok(work);
        abort();
        //return 2;
    }
    if (CNT < -1)
    {
        SLASHES;
        printf("ERR#3\n");
        dump_ok(work);
        abort();
        //return 3;
    }
    //dump_ok(work);
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
    for (int i = work->stackSZ/2; i <= work->stackSZ; ++i)
        *(work->buffer + i) = 0;

    stack_ok(work);

    //SLASHES;
    //EMPT;
}
//!===========================================
void FAQ(void)
{
    EMPT;
    SLASHES;
    printf("        Available commands:\n");
    SLASHES;
    printf("list    - show the list of commands\n");
    printf("push    - add element to stack\n");
    printf("pop     - delete element from stack\n");
    printf("clear   - clean all stack (all elements will become 0)\n");
    printf("div     - division last element\n");
    printf("mult    - multiply operation\n");
    printf("plus    - sum operation\n");
    printf("nsum    - minus operation\n");
    printf("exit    - end program\n");
    printf("dump    - see the dump\n");
    SLASHES;
    EMPT;
}
//!===========================================
void pushData(stack_t *work, double numb)
{

    if (!stack_ok(work))
    {

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

        //printf("new binary = <%lg> \n", *(work->buffer + work->cnt-1));
    }

    stack_ok(work);

}
//!===========================================
void popData(stack_t *work)
{
    stack_ok(work);
    if (work->cnt >= 0)
    {
        *(work->buffer + work->cnt-1) = 0;
        work->cnt--;
        work->freemem++;
    }

    if (work->cnt == -1) work->cnt = 0;
    stack_ok(work);
}
//!===========================================
void delData(stack_t *work)
{
    stack_ok(work);

    work->freemem = STARTMEM;
    work->stackSZ = STARTMEM;
    work->cnt = 0;
    work->buffer = (double *)realloc(work->buffer, STARTMEM*sizeof(double));

    for (int i = 0; i < work->stackSZ; ++i)
        *(work->buffer + i) = 0;
    stack_ok(work);
}
//!===========================================
void divData(stack_t *work)
{
    stack_ok(work);
    /*if ( work->cnt == 0)
    {
        printf("#Stack is empty. Unable to do division\n");
        int num1 = 0;
        scanf("%lf", &num1);
        printf("\n num1 = %lf\n", num1);
        pushData(work, num1);
        int num2 = 0;
        scanf("%lf", &num2);
        printf("\n num2 = %lf\n", num2);
        pushData(work, num2);

        if (num2)
            pushData(work, num1/num2);
        else
        {
            printf("ERROR! Division by zero! Try again and type new value\n");
            popData(work);
            scanf("%lf", &num2);
            pushData(work, num2);
        }

    }*/
    if (work->cnt == 1 || work->cnt == 0)
    {
        printf("#Stack is empty! Unable to do division\n");
    }
    else
    {
        if (*(work->buffer + work->cnt -1))
        {
            pushData(work, *(work->buffer + work->cnt - 2)/(*(work->buffer + work->cnt - 1)));
            resData(work);
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
        printf("#Stack is empty! Unable to sum up\n");
    }
    else
    {
        pushData(work, *(work->buffer + work->cnt - 2) + *(work->buffer + work->cnt - 1));
        resData(work);
    }
}
//!===========================================
void nsumData(stack_t *work)
{
    if (work->cnt == 1 || work->cnt == 0)
    {
        printf("#Stack is empty! Unable to sum up\n");
    }
    else
    {
        pushData(work, *(work->buffer + work->cnt - 2) - *(work->buffer + work->cnt - 1));
        resData(work);
    }
}
//!===========================================
void multData(stack_t *work)
{
    if (work->cnt == 1 || work->cnt == 0)
    {
        printf("#Stack is empty! Unable to multiply\n!");
    }
    else
    {
        pushData(work, *(work->buffer + work->cnt - 2) * *(work->buffer + work->cnt - 1));
        resData(work);
    }
}
//!===========================================
void showData(stack_t *work)
{
    //printf("this is show data\n");
    SLASHES;
    for (int i = 0; i < work->cnt; i++)
    {
        //assert(0 <= i && i < work->cnt);
        printf("b[%d] = <%lg>\n", i, *(work->buffer + i));
    }
    SLASHES;
}
//!===========================================
void resData(stack_t *work)
{
    printf("\tresult = <%lg>\n", *(work->buffer + work->cnt - 1));
}
//!===========================================
void sumData(stack_t *work)
{
    double sum = 0;
    for (int i = 0; i < work->cnt; ++i)
    {
        assert(0 <= i && i < work->cnt);
        sum += *(work->buffer + i);
    }
    pushData(work, sum);
    resData(work);
}
