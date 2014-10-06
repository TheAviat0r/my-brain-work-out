//!{========================================================================================
//!     @file       refactored_onegin.c
//!     @author     Niatshin Bulat
//!     @task       The task is to sort Evgeny Onegin, written by A.S. Pushkin
//!     @condition  Program inputs the text from input.txt in reading mode
//!                 After that it sorts it using qsort algorithm
//!                 There are two versions of sorting: from the beginning and
//!                 from the end.
//!     @warning    I suppose there are bunch of bugs in my source code so try to test it pls
//!}========================================================================================
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <io.h>
#include <string.h>
#include <assert.h>
#include <locale.h>
//!=========================================================================================

void greeting(void);
//!=========================================================================================
int countStr (unsigned char buffer[], int flen);
//!=========================================================================================
void placeStr (unsigned char **data, unsigned char buffer[], int flen, int nStroke);
//!=========================================================================================
int ComparePls (const void * a, const void * b);
//!=========================================================================================
void prepareBuff (unsigned char *buffer, int flen);

int main()
{
    greeting();
    setlocale(LC_ALL, "rus");
    //printf("ya = %s\n", 239);
    //!================================
    FILE *input;
    if ( (input = fopen("input.txt", "r")) != 0)
    {
        int flen = filelength(fileno(input));
        printf("File Length = %d\n", flen);
        //!================================
        unsigned char * buffer = (unsigned char *) calloc(flen+1, sizeof(*buffer));
        fread(buffer, flen, sizeof(unsigned char*), input);
        //!================================
        //buffer[flen+1] = 0;
        //!================================
        printf("====================================================================\n");
        printf("This is a default buffer\n");
        //!================================
        printf("====================================================================\n");
        puts((char*)buffer);
        prepareBuff(buffer, flen);
        printf("====================================================================\n");
        puts((char*)buffer);

        printf("====================================================================\n");
        //!================================
        int nStroke = countStr(buffer, flen);
        printf("nStroke = %d\n", nStroke);
        //!================================
        unsigned char **data = (unsigned char **) calloc(nStroke, sizeof(* data));
        placeStr(data, buffer, flen, nStroke);
        //!================================
        qsort(data, nStroke, sizeof(unsigned char *), &ComparePls);
        //!================================
        printf("====================================================================\n");
        for (int i = 0; i < nStroke; ++i)
            puts((char *)data[i]);
        //!================================
        for (int i = 0; i < nStroke; ++i)
            strrev((char*)data[i]);

        qsort(data, nStroke, sizeof(unsigned char *), &ComparePls);
        //!================================
        for (int i = 0; i < nStroke; ++i)
            strrev((char*)data[i]);
        //!================================
        printf("=====================================================================\n");
        for (int i = 0; i < nStroke; ++i)
            puts((char*)data[i]);
        //!================================
        free(buffer);
        free(data);
        //!================================

        return 0;
    }
    else
    {
        printf("ERROR! Unable to read txt file. Try again\n");
        return 0;
    }
    //!================================
}

void prepareBuff (unsigned char *buffer, int flen)
{
    for (int i = 0; i < flen; ++i)
    {
        assert (0 <= i && i < flen);
        if (!isalpha(buffer[i]) && !isspace(buffer[i]) && (buffer[i] != '\n') && (buffer[i] != NULL) &&\
                                                                                buffer[i] != 239)
        {
            printf("buffer[i] =  %c ASCI <%d> [0x%x]\n", buffer[i], buffer[i], buffer+i);
            buffer[i] = ' ';
        }
    }
}
//!================================
int ComparePls (const void * a, const void * b)
{
    return strcmp (*(char **)a, *(char **)b);
}
//!================================
int countStr (unsigned char buffer[], int flen)
{
    assert(buffer != NULL && flen != NULL);
    int nStroke = 1;

    for (int i = 0; i <= flen; ++i)
    {
        assert (0 <= i && i <= flen);
        if (buffer[i] == '\n')
        {
            buffer[i] = '\0';
            ++nStroke;
        }
    }

    return nStroke;
}
//!================================
void placeStr (unsigned char **data, unsigned char buffer[], int flen, int nStroke)
{
    assert( data != NULL && buffer != NULL);
    //!================================
    data[0] = buffer;
    printf("=============\nthis is placeStr\n");
    printf("====================================================================\n");
    puts((char *)data[0]);
    int cnt = 1;
    //!================================
    printf("data %d buffer %d \n", *data, buffer); //checking that adress of data[0] and buffer are the same
    for (int i = 1; i < flen; ++i)
    {
        assert(1 <= i && i < flen);
        if (buffer[i] == '\0')
        {
                if (cnt >= nStroke)
                    break;
                data[cnt] = buffer +i + 1;
                // this if is to keep borders of data
                buffer[i] = 0;
                ++cnt;
                printf("cnt = %d\n", cnt);
                printf("this is data[%d]\n", cnt);
                //puts(data[cnt]);
        }
    }
}
//!================================
void greeting(void)
{
    printf("$=$=$=$=$=$=$=$=$=$=$=$=$=$=$=$=$=$=$=$=$=\n");
    printf("Aviator MIPT (c), copyright reserved\n");
    printf("%s", __DATE__ , " %s", __TIME__ " ");
    printf("\n$=$=$=$=$=$=$=$=$=$=$=$=$=$=$=$=$=$=$=$=$=\n\n");
}


