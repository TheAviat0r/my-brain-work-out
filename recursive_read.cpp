#include <stdio.h>
#include <io.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <ctype.h>
//!------------------------------------------
#define SLASHES printf("--------------------------------------\n");
#define EMPT printf("\n");
//!------------------------------------------
unsigned int cnt = 0;
unsigned int flen = 0;
char * buffer = 0;
//!------------------------------------------
void greeting();
void processTask();
    //!--------------------------------------
    void formBuffer(FILE *input);
    void outputFile();
    //!--------------------------------------
    int getNum();
    int getAdd();
    int getTerm();
    int getPeek();
//!------------------------------------------
int main()
{
    greeting();
    processTask();
    return 0;
}
//!------------------------------------------
void greeting()
{
    SLASHES
    printf("Aviator MIPT, %s, %s\n", __DATE__, __TIME__);
    SLASHES
    printf("Hello! I am the Recursive Read!\n");
    SLASHES
}
//!------------------------------------------
void processTask()
{
    FILE *input = NULL;
    formBuffer(input);

    EMPT SLASHES puts(buffer); SLASHES

    int value = getAdd();

    if (buffer[cnt] != '\0')
    {
        printf("SYNTAX ERROR <buffer[%d] [%c]>! RECOMPILE YOUR PROG!\n", cnt, buffer[cnt]);
        exit(2);
    }

    fclose(input);
    free(buffer);
    buffer = NULL;

    EMPT SLASHES printf("RESULT = %d\n", value);
}
//!------------------------------------------
void formBuffer(FILE *input)
{
    input = fopen("input.txt", "r");
    if (input == NULL)
    {
        SLASHES printf("Unable to open input.txt! Try again!\n");
        exit(1);
    }

    flen = filelength(fileno(input));

    buffer = (char *) calloc(flen+1, sizeof(*buffer));
    fread(buffer, sizeof(*buffer), flen, input);
}
//!------------------------------------------
void outputFile()
{
}
//!------------------------------------------
int getNum()
{
    assert(buffer != NULL);
    assert(cnt <= flen);

    int val = 0;

    while ('0' <= buffer[cnt] && buffer[cnt] <= '9')
    {
        val = val*10 + buffer[cnt] - '0';
        cnt++;
    }

    return val;
}
//!------------------------------------------
int getAdd()
{
    assert(buffer != NULL);
    assert(cnt <= flen);

    //SLASHES printf("getElem!\n");
    int val = getTerm();

    while (buffer[cnt] == '+' || buffer[cnt] == '-')
    {
        //printf("cnt = %d value = %c\n", cnt, buffer[cnt]);
        cnt++;

        if (buffer[cnt - 1] == '+')
            val += getTerm();
        if (buffer[cnt - 1] == '-')
            val -= getTerm();

    }

    return val;
}
//!-------------------------------------------
int getTerm()
{
    //SLASHES printf("getTerm!\n");
    int val = getPeek();

    while (buffer[cnt] == '*' || buffer[cnt] == '/')
    {
        //printf("cnt = %d buf = %c\n", cnt, buffer[cnt]);
        cnt++;

        if (buffer[cnt - 1] == '*')
            val *= getPeek();
        if (buffer[cnt - 1] == '/')
        {
            int saveNum = getPeek();

            if (saveNum == NULL)
            {
                SLASHES printf("ERROR! Division by zero!\n"); SLASHES
                exit(4);
            }

            val /= saveNum;
        }
    }

    return val;
}
//!-------------------------------------------
int getPeek()
{
    if (buffer[cnt] == '(')
    {
        cnt++;
        int val = getAdd();

        if (buffer[cnt] == ')')
        {
            cnt++;
            return val;
        }
        else
        {
            printf("Wrong input! ')' is missing!\n");
            exit(3);
        }

    }
    else
        return getNum();
}
