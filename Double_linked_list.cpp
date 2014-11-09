#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
//!-----------------------------------
#ifndef NDEBUG
#define assert_ok(cond) if(!(cond)){\
                        printf("FAIL: %s in %s, %d %;", \
                        #cond, __FILE__, __LINE__, __PRETTY_FUNCTION__);\
                        EMPT; dumpList(Head); exit(1);}
#else #define assert_ok(cond) if(0);
#endif
//!-----------------------------------
#define SLASHES printf("---------------------------------------------\n");
#define EMPT printf("\n");
//!-----------------------------------
const int LEFT =     1;
const int RIGHT =    2;
const int STARTLEN = 2;
//!-----------------------------------
typedef int dataType;
//!-----------------------------------
struct listElem_t
{
    dataType data;
    listElem_t *prev;
    listElem_t *next;
};
//!-----------------------------------
struct header_t
{
   unsigned int ListLen;
   listElem_t *theFirst;
   listElem_t *theLast;
};
//!-----------------------------------
void greeting(void);
void processList();
//!-----------------------------------
    void initiateList(header_t *Head);
    void removeList(header_t *Head);
    listElem_t *ctor(void);
    void dtor (listElem_t *work, header_t *Head);
    void insert_in(dataType value, int position, int orientation, header_t *Head);
    void remove_in(int position, int orientation, header_t *Head);
    //!-------------------------------
    void dumpList(header_t *Head);
    int  List_ok(header_t *Head);
//!-----------------------------------
int main()
{
    greeting();
    processList();
    return 0;
}
//!-----------------------------------
void greeting(void)
{
    SLASHES;
    printf("Aviator MIPT, %s, %s\n", __DATE__, __TIME__);
    SLASHES;
    printf("Hello! I am the model of CPU!\n");
    SLASHES;
}
//!-----------------------------------
void processList()
{
    header_t Head = {};
    initiateList(&Head);

    insert_in(1, 2, RIGHT, &Head);
    insert_in(2, 3, RIGHT, &Head);
    insert_in(3, 4, RIGHT, &Head);
    insert_in(4, 5, RIGHT, &Head);
    insert_in(4, 5, LEFT, &Head);
    remove_in(2, LEFT, &Head);
    remove_in(2, RIGHT, &Head);
    removeList(&Head);
}
//!-----------------------------------
void initiateList(header_t *Head)
{
    assert_ok(Head != NULL);

    Head->theFirst = (listElem_t *) calloc(1, sizeof(listElem_t));
    Head->theLast =  (listElem_t *) calloc(1, sizeof(listElem_t));

    Head->theFirst->data = NULL;
    Head->theFirst->next = Head->theLast;
    Head->theFirst->prev = NULL;

    Head->theLast->data = NULL;
    Head->theLast->next = NULL;
    Head->theLast->prev = Head->theFirst;

    Head->ListLen = STARTLEN;
    assert(List_ok(Head) == NULL);
}
//!-----------------------------------
void removeList(header_t *Head)
{
    assert_ok(Head != NULL);
    listElem_t * work = Head->theFirst;

    for (int i = 2; i < Head->ListLen; i++)
    {
        assert(2 <= i && i <= Head->ListLen);
        dtor(work->prev, Head);
        work = work->next;
    }
    work = NULL;

    free(Head);
    dumpList(Head);
}
//!-----------------------------------
listElem_t * ctor()
{
    listElem_t * work = (listElem_t *) calloc(1, sizeof(listElem_t));
    work->next = NULL;
    work->prev = NULL;
    work->data = NULL;

    return work;
}
//!-----------------------------------
void dtor(listElem_t *work, header_t *Head)
{
    assert_ok(work != NULL);
    if (work->prev == NULL)
    {
        work->next = NULL;
        return;
    }
    if (work->next == NULL)
    {
        work->prev = NULL;
        return;
    }

    work->next = NULL;
    work->prev = NULL;
    work = NULL;
}
//!-----------------------------------
void insert_in(dataType value, int position, int orientation, header_t *Head)
{
    assert_ok(List_ok(Head) == NULL);
    assert_ok(position <= Head->ListLen);

    listElem_t *work = Head->theFirst;
    for (int i = 1; i < position; i++)
    {
        assert(1 <= i && i < position);
        work = work->next;
    }

    listElem_t *target = ctor();
    target->data = value;

    if (orientation  == LEFT)
    {
        listElem_t *previous = work->prev;
        //printf("previous [0x%p]\n", previous);
        if (previous != NULL)
            previous->next = target;
        if (previous == NULL)
            Head->theFirst = target;
        target->prev = previous;

        target->next = work;
        work->prev = target;
        Head->ListLen += 1;
    }

    if (orientation == RIGHT)
    {
        listElem_t * theNext = work->next;
        work->next = target;
        target->prev = work;

        target->next = theNext;
        if (theNext != NULL)
            theNext->prev = target;
        if (theNext == NULL)
            Head->theLast = target;
        Head->ListLen += 1;
    }

    dumpList(Head);
    assert_ok(List_ok(Head) == NULL);
}
//!-----------------------------------
void remove_in(int position, int orientation, header_t *Head)
{
    assert_ok(List_ok(Head) == NULL);
    assert_ok(position <= Head->ListLen);
    assert_ok(orientation == LEFT || orientation == RIGHT);

    listElem_t *work = Head->theFirst;
    for (int i = 1; i < position; i++)
    {
        assert(1 <= i && i < position);
        work = work->next;
    }

    if (Head->ListLen == 1)
    {
        printf("Unable to delete!\n");
        exit(3);
    }

    if (orientation  == LEFT)
    {
        if (work->prev == NULL)
        {
            printf("FAIL! You are trying to delete unexisted element\n");
            return;
        }

        listElem_t *previous = work->prev;

        if (previous == Head->theFirst)
        {
            Head->theFirst = work;
            work->prev = NULL;
            dtor(previous, Head);

            Head->ListLen -= 1;
            dumpList(Head);
            return;
        }

        work->prev = previous->prev;
        previous->prev->next = work;
        dtor(previous, Head);
        Head->ListLen -= 1;
    }

    if (orientation == RIGHT)
    {
        if (work->next == NULL)
        {
            printf("ERROR! You are trying to delete unexisted element!\n");
            exit(1);
        }

        listElem_t * theNext = work->next;

        if (theNext == Head->theLast)
        {
            Head->theLast = work;
            work->next = NULL;
            dtor(theNext, Head);

            Head->ListLen -= 1;
            dumpList(Head);
            return;
        }

        work->next = theNext->next;
        theNext->next->prev = work;
        dtor(theNext, Head);
        Head->ListLen -= 1;
    }
    else
    {
        printf("ERROR! WRONG ORIENTATION!\n");
        exit(2);
    }

    assert_ok(List_ok(Head) == NULL);
    dumpList(Head);
}
//!-----------------------------------
void dumpList(header_t *Head)
{
    EMPT SLASHES printf("\tDUMP OF THE LIST\n"); SLASHES
    printf("\tHEADER\n"); SLASHES
    printf("   Head     [0x%p]\n", Head);
    printf("   theFirst [0x%p]\n", Head->theFirst);
    printf("   theLast  [0x%p]\n", Head->theLast);
    printf("   ListLen  (%d)\n",   Head->ListLen);
    SLASHES;

    listElem_t * work = Head->theFirst;
    for (int i = 1; i <= Head->ListLen; i++)
    {
        printf("   %d) ELem [0x%p] <%d>\n   prev [0x%p]\n   next [0x%p]\n", i, work,\
                                                                        work->data, work->prev, work->next);
        SLASHES
        work = work->next;
    }
}
//!-----------------------------------
int List_ok(header_t *Head)
{
    assert_ok(Head != NULL);
    listElem_t *current = Head->theFirst;
    listElem_t *theNext = Head->theFirst->next;
    listElem_t *thePrev = 0;

    for (int i = 1; i < Head->ListLen; i++)
    {
        assert(1 <= 1 && i < Head->ListLen);
        if (current->prev == NULL)
        {
            theNext = current->next;
            if (theNext->prev != current)
                return 1;
            current = current->next;
            continue;
        }

        if (current->next == NULL)
        {
            thePrev = current->prev;
            if (thePrev->next != current)
                return 1;
        }

        thePrev = current->prev;
        if (thePrev->next != current)
            return 1;
        theNext = current->next;
        if (theNext->prev != current)
            return 1;
        current = current->next;
    }
    return 0;
}


