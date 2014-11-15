#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
//!-----------------------------------
#ifndef NDEBUG
#define assert_ok(cond) if(!(cond)){\
                        printf("FAIL: %s in %s, %d %;", \
                        #cond, __FILE__, __LINE__, __PRETTY_FUNCTION__);\
                        EMPT; dumpList(Head); exit(10);}
#else #define assert_ok(cond) if(0);
#endif
//!-----------------------------------
#define SLASHES printf("---------------------------------------------\n");
#define EMPT printf("\n");
#define ASSERT_OK assert_ok(List_ok(Head) == NULL);
//!-----------------------------------
const int LEFT =     1;
const int RIGHT =    2;
const int STARTLEN = 2;
const int HEADLIM  = 97;
const int STRLIM = 40;
const int NOTEXIST = -1;
//!-----------------------------------
const char STUPID_HASH []=    "stupid Hash";
const char LEN_HASH []=       "len hash";
const char SMART_HASH []=     "smart hash";
const char ASCI_HASH []=      "asci hash";
const char SUM_HASH []=       "sum hash";
const char NO_STR[] =         "NO";
const char YES_STR[] =        "YES";
const char EXIT[] =           "EXIT";
//!-----------------------------------
typedef char * dataType;
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
typedef unsigned int hashFunc_t (const char str[]);
//!-----------------------------------
void greeting(void);
void processData(hashFunc_t *hasher, const char func_name[]);
//!-----------------------------------
    //!     standart functions
    //!-------------------------------
    listElem_t *ctor(void);
    void dtor (listElem_t *work, header_t *Head);
    //!-------------------------------
    //!     hash-table functions
    //!-------------------------------
    void initiateTable(header_t head_arr[]);
    FILE* openFile();
    void outputData(header_t head_arr[], const char func_name[]);
    void destroyTable(header_t head_arr[]);
    void processSearch(hashFunc_t *hasher, header_t head_arr[]);
    int searchData(hashFunc_t *hasher, header_t head_arr[], const char search_word[]);
    //!-------------------------------
    void processHashing(FILE *input, header_t head_arr[], hashFunc_t * hasher, const char func_name[]);
    hashFunc_t stupidHash;
    hashFunc_t asciHash;
    hashFunc_t sumAsciHash;
    hashFunc_t lenHash;
    hashFunc_t smartHash;
    //!-------------------------------
    //!     list functions
    //!-------------------------------
    void initiateList(header_t *Head);
    void removeList(header_t *Head);
    void insert_in(dataType value, listElem_t *work, int orientation, header_t *Head);
    void remove_in(listElem_t * work, header_t *Head);
    void printList(header_t *Head);
    listElem_t * seekElem(int position, header_t *Head);
    //!-------------------------------
    //!     debugging functions
    //!-------------------------------
    void dumpHead(header_t head_arr[]);
    void dumpList(header_t *Head);
    int  List_ok(header_t *Head);
//!-----------------------------------
int main()
{
    greeting();

    processData(stupidHash, STUPID_HASH);
    processData(asciHash, ASCI_HASH);
    processData(lenHash, LEN_HASH);
    processData(sumAsciHash, SUM_HASH);
    processData(smartHash, SMART_HASH);

    return 0;
}
//!-----------------------------------
void greeting(void)
{
    SLASHES;
    printf("Aviator MIPT, %s, %s\n", __DATE__, __TIME__);
    SLASHES;
    printf("Hello! I am the Hash-Table!\n");
    SLASHES;
}
//!-----------------------------------
void processData(hashFunc_t *hasher, const char func_name[])
{
    EMPT SLASHES printf("\t TYPE OF HASHING <%s>", func_name);
    header_t head_arr[HEADLIM] = {};
    initiateTable(head_arr);

    FILE *input = openFile();
    processHashing(input, head_arr, hasher, func_name);
    outputData(head_arr, func_name);
    processSearch(hasher, head_arr);

    fclose(input);
    destroyTable(head_arr);
}
//!-----------------------------------
void processSearch(hashFunc_t *hasher, header_t head_arr[])
{
    assert(hasher != NULL && head_arr != NULL);

    char command[STRLIM] = {};

    EMPT SLASHES printf("If you want to search - type YES, no - type NO\n");
    scanf("%s", command);
    if (!strcmp(command, NO_STR))
        return;
    if (!strcmp(command, YES_STR))
    {
        printf("if you want to stop searching - type EXIT\n");
        scanf("%s", command);

        while (strcmp(command, EXIT))
        {
            int index = searchData(hasher, head_arr, command);
            if (index != NOTEXIST)
                printList(head_arr + index);
            else
                printf("Element <%s> doesn't exist!\n", command);

            scanf("%s", command);
        }
        return ;
    }
}
void initiateList(header_t *Head)
{
    assert_ok(Head != NULL);

    Head->ListLen = NULL;
    ASSERT_OK;
}
//!-----------------------------------
void initiateTable(header_t head_arr[])
{
    assert(head_arr != NULL);
    header_t HashTable[HEADLIM] = {};

    for (int i = 0; i < HEADLIM; i++)
    {
        assert(0 <= i && i < HEADLIM);
        initiateList(&HashTable[i]);
    }

    EMPT SLASHES printf("Hash is initiated!\n");
}
//!-----------------------------------
void destroyTable(header_t head_arr[])
{
    assert(head_arr != NULL);

    for (int i = 0; i < HEADLIM; i++)
    {
        assert(0 <= i && i < HEADLIM);
        removeList(&head_arr[i]);
    }

    free(head_arr);
    head_arr = NULL;

    SLASHES printf("Hash-table is DESTROYED!\n");
}
//!-----------------------------------
FILE* openFile()
{
    FILE *input = fopen("source.txt", "r");
    if (input == NULL)
    {
        printf("ERROR! Unable to open source.txt!\n");
        return 0;
    }

    SLASHES printf("source.txt is successfully opened!\n");
    return input;
}
//!-----------------------------------
void outputData (header_t head_arr[], const char func_name[])
{
    assert(head_arr != NULL && func_name != NULL);
    FILE * output = fopen("output.csv", "a");
    if (output == NULL)
    {
        SLASHES printf("Unable to create or open output.csv! Try again!\n");
        exit(1);
    }

    fprintf(output, "%s;", func_name);

    for (int i = 0; i < HEADLIM; i++)
    {
        assert(0 <= i && i < HEADLIM);
        fprintf(output, "%d;", (head_arr + i)->ListLen);
    }

    fprintf(output, "\n");
    fclose(output);
}
//!-----------------------------------
void processHashing(FILE *input, header_t head_arr[], hashFunc_t *hasher, const char func_name[])
{
    assert(input != NULL && head_arr != NULL && hasher != NULL);

    char command[STRLIM] = {};
    int index = 0;

    while (fscanf(input, "%s", command) == 1)
    {
        index = hasher(command);
        insert_in(command, (head_arr + index)->theLast, RIGHT, head_arr + index);
    }

    SLASHES printf("input.txt is successfully hashed!\n");
}
//!-----------------------------------
int searchData(hashFunc_t *hasher, header_t head_arr[], const char search_word[])
{
    assert(hasher != NULL && head_arr != NULL && search_word != NULL);
    int index = hasher(search_word);

    if ((head_arr + index)->ListLen > 0)
        return index;
    else
        return NOTEXIST;
}
//!-----------------------------------
unsigned int stupidHash (const char str[])
{
    assert(str != NULL);
    return 1;
}
//!-----------------------------------
unsigned int asciHash (const char str[])
{
    assert(str != NULL);
    return (str[0] - '\0') % HEADLIM;
}
//!-----------------------------------
unsigned int sumAsciHash (const char str[])
{
    assert(str != NULL);
    int len = strlen(str);
    int sum = 0;
    for (int i = 0; i < len; i++)
    {
        assert(0 <= i && i < len);
        sum += str[i];
    }

    return sum % HEADLIM;
}
//!-----------------------------------
unsigned int lenHash (const char str[])
{
    assert(str != NULL);
    return (strlen(str) % HEADLIM);
}
//!-----------------------------------
unsigned int smartHash(const char str[])
{
    assert(str != NULL);

    unsigned int len = strlen(str);
    int value = 0;

    for (int i = 0; i < len; i++)
    {
        assert(0 <= i && i < len);
        value = (value << 1)^str[i];
    }


    return value % HEADLIM;
}
//!-----------------------------------
void removeList(header_t *Head)
{
    ASSERT_OK;
    //listElem_t * work = Head->theFirst;
    listElem_t *work = Head->theFirst;

    for (int i = 1; i < Head->ListLen; i++)
    {
        assert(1 <= i && i < Head->ListLen);
        listElem_t *save = work->next;
        dtor(work, Head);
        work = save;
        //printf("i = %d; work = %p; head = %p\n", i, work, Head);
    }

    free(Head);
    Head->theFirst = NULL;
    Head->theLast = NULL;
    Head->ListLen = NULL;
    Head = NULL;
}
//!-----------------------------------
listElem_t * ctor()
{
    listElem_t * work = {};
    work = (listElem_t *) calloc(1, sizeof(listElem_t));
    work->data = (char *) calloc(STRLIM, sizeof(char));

    return work;
}
//!-----------------------------------
void dtor(listElem_t *work, header_t *Head)
{
    assert_ok(work != NULL);
    if (work->prev == NULL)
    {
        work->next = NULL;
        free(work);
        return;
    }
    if (work->next == NULL)
    {
        work->prev = NULL;
        return;
    }

    work->next = NULL;
    work->prev = NULL;
    free(work);
    work = NULL;
}
//!-----------------------------------
void insert_in(dataType value, listElem_t *work, int orientation, header_t *Head)
{
    ASSERT_OK;
    assert(orientation == LEFT || orientation == RIGHT);

    listElem_t *target = ctor();
    strcpy(target->data, value);

    if (orientation  == LEFT)
    {
        if (work == NULL || Head->ListLen == NULL)
        {
            Head->theFirst = target;
            Head->theLast = target;
            Head->ListLen += 1;

            ASSERT_OK;
            //dumpList(Head);
            return;
        }

        listElem_t *previous = work->prev;
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
        if (work == NULL || Head->ListLen == NULL)
        {
            Head->theFirst = target;
            Head->theLast = target;
            Head->ListLen += 1;

            ASSERT_OK;
            //dumpList(Head);
            return;
        }

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

    ASSERT_OK;
    //dumpList(Head);
}
//!-----------------------------------
void remove_in(listElem_t * work, header_t *Head)
{
    ASSERT_OK;
    if (Head->ListLen == 0)
    {
        printf("Unable to delete!\n");
        return;
    }

    if (work == Head->theFirst)
    {
        Head->theFirst = work->next;
        Head->theFirst->prev = NULL;
        dtor(work, Head);
        Head->ListLen -= 1;

        ASSERT_OK;
        dumpList(Head);
        return;
    }

    if (work == Head->theLast)
    {
        Head->theLast = work->prev;
        Head->theLast->next = NULL;
        dtor(work, Head);
        Head->ListLen -= 1;

        ASSERT_OK;
        dumpList(Head);
        return;
    }

    listElem_t * thePrevious = work->prev;
    listElem_t * theNext = work->next;

    thePrevious->next = work->next;
    theNext->prev = work->prev;
    dtor(work, Head);

    Head->ListLen -= 1;
    dumpList(Head);

    ASSERT_OK;
}
//!-----------------------------------
void printList(header_t *Head)
{
    assert(Head != NULL);
    EMPT    SLASHES;

    int len = Head->ListLen;
    listElem_t * work = NULL;

    if (len > 0)
        work = Head->theFirst;

    for (int i = 1; i <= len; i++)
    {
        printf("    %d) <%s>\n", i, work->data);
        work = work->next;
    }
}
//!-----------------------------------
listElem_t * seekElem(int position, header_t *Head)
{
    ASSERT_OK;
    assert(position >= NULL);
    assert_ok(Head != NULL);

    if (Head->ListLen == NULL)
        return 0;
    if (position > Head->ListLen)
        return 0;

    listElem_t * work = Head->theFirst;
    for (int i = 1; i < position; i++)
    {
        assert(1 <= i && i < position);
        work = work->next;
    }

    ASSERT_OK;

    return work;
}
//!-----------------------------------
void dumpHead(header_t head_arr[])
{
    EMPT SLASHES printf("\tDUMP OF THE HEAD\n"); SLASHES

    for (int i = 0; i < HEADLIM; i++)
    {
        assert(0 <= i && i < HEADLIM);
        dumpList(&head_arr[i]);
    }
}
//!-----------------------------------
void dumpList(header_t *Head)
{
    SLASHES
    printf("   Head     [0x%p]", Head);
    printf("   theFirst [0x%p]", Head->theFirst);
    printf("   theLast  [0x%p]\n", Head->theLast);
    printf("   ListLen  (%d)\n",   Head->ListLen);
    SLASHES;

    listElem_t * work = Head->theFirst;
    for (int i = 1; i <= Head->ListLen; i++)
    {
        printf("   %d) [0x%p] <%s>\n   prev [0x%p]\n   next [0x%p]\n", i, work,\
                                                                        work->data, work->prev, work->next);
        SLASHES
        work = work->next;
    }
}
//!-----------------------------------
int List_ok(header_t *Head)
{
    assert_ok(Head != NULL);
    if (Head->ListLen == NULL)
        return 0;

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
