#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <io.h>
#include <assert.h>

void greeting(void);
int get_num(char buf[], int flen);
void get_strokes (char buf[], char *data[], int flen);
int ComparePls (const void **a, const void ** b);
int compar( char a[],  char b[]);
void lowsort (char ** data, int nStroke);


int main()
{
    greeting();
    printf("Here you can see the program that inverts Evgeny Onegin \n");

    FILE *input;
    if( (input = fopen("input.txt", "r")) != 0 )
    {
        int flen = filelength(fileno(input));
        assert(flen >= 0);

        char *buffer = (char *) calloc(flen + 1, sizeof(*buffer));
        fread(buffer, flen, sizeof(char), input);
        fclose(input);

        puts(buffer);
        printf("======================================\n");
        int nStroke = get_num(buffer, flen);
        printf("nStroke = %d\n", nStroke);
        assert(nStroke > 0);

        char **data = (char **) calloc(nStroke+1, sizeof(char **));

        get_strokes (buffer, data, flen);
        lowsort(data, nStroke);

        for (int i = 0; i <= nStroke; +i)
            puts(data[i]);
        //qsort(data, flen, sizeof(char**), &ComparePls);

        return 0;

    }
    else
    {
        printf("ERROR INPUT! Try again! \n");
        return 0;
    }
}

void lowsort (char ** data, int nStroke)
{
    for (int i = 0; i <= nStroke; ++i)
    {
        int maxnumber = i;
        char *work = 0;
        assert ( 0 <= i && i<= nStroke);
        for (int j = 0; j <= nStroke; ++j)
        {
            assert ( 1 <= j && j <= nStroke);
            if (compar(data[i], data[j]) == 1)
            {
               work = data[j];
               data[j] = data[i];
               data[i] = work;
            }
        }
    }
}

int compar(char *a[], char *b[])
{
    int i=0;
    while (*a[i] != '\0' || *b[i]!='\0')
    {
        if(*a[i] < *b[i]) return -1;
        if(*a[i] > *b[i]) return 1;
        i++;
    }
    return 0; //если ни первое и не второе то они равны
}

int get_num(char buf[], int flen)
{
    int strokes = 1;
    for (int i = 0; i < flen; i++)
    {
        assert ( 0 <= i && i < flen);
        if (buf[i] == '\n')
            ++strokes;
    }

    return strokes;
}

void get_strokes (char buf[], char *data[], int flen)
{
    data[0] = buf;
    for (int i = 1; i < flen; i++)
    {
        assert ( 0 <= i && i < flen);
        if (buf[i] == '\n')
        {
            buf[i] = 0;
            *(data+i) = buf+i;
            puts(buf+i+1);
        }
    }
}


void greeting(void)
{
    printf("$=$=$=$=$=$=$=$=$=$=$=$=$=$=$=$=$=$=$=$=$=\n");
    printf("Aviator MIPT (c), copyright reserved\n");
    printf("%s", __DATE__ , " %s", __TIME__ " ");
    printf("\n$=$=$=$=$=$=$=$=$=$=$=$=$=$=$=$=$=$=$=$=$=\n");
}
