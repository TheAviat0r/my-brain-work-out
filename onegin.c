#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <io.h>
//#include <setlocale>

void greeting(void);
int compareStr(const void *a, const void *b);
int find_strnum (char buffer[], int f_len);
void place_str (char *buffer, char **data, int f_len);

int main()
{
    greeting();
    printf("Here you can see the prog that reads Evgeny Onegin from txt \nfile and sorts it\n");

    FILE *input;
    //FILE *output;

    input = fopen("input", "r");
    {
        //output = fopen("output", "w");
        int f_len = filelength(fileno(input));
        assert (f_len >= 0);

        char* buffer = (char *) calloc(f_len+1, sizeof(*buffer));
        assert(buffer > 0);

        fread(buffer, f_len, sizeof(char), input);
        fclose(input);

        int howmuch_str = find_strnum(buffer, f_len);
        assert (howmuch_str >= 0);

        char **data = (char **) calloc(howmuch_str+1, sizeof(**data));

        place_str (buffer, data, f_len);

        qsort (data, howmuch_str, sizeof(char*), &compareStr);

        for (int i = 0; i < howmuch_str; ++i)
        {
            assert (0 <= i && i < howmuch_str);
            puts(data[i]);
        }

    }
    /*else
    {
        printf("ERROR_TXT! Unable to create or open new txt file. Contact developer.\n");
        return 0;
    }*/

    return 0;
}

int compareStr(const void *a, const void *b)
{
    return strcmp (*(const char**)a, *(const char**)b);
}

int find_strnum (char *buffer, int f_len)
{
    int strnum = 1;
    for (int i = 0; i < f_len; ++i)
    {
        assert ( 0 <= i && i < f_len);
        if (buffer[i] == '\n' /*buffer[i] == EOF*/)
        {
            buffer[i] = '\0';
            ++strnum;
        }
    }
    return strnum;
}

void place_str (char *buffer, char **data, int f_len)
{
    *data = buffer;
    for (int i = 1; i < f_len; ++i)
    {
        assert ( 1 <= i && i < f_len);
        if (buffer[i] == '\n')
        {
            buffer[i] = '\0';
            data[i] = buffer+i;
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

