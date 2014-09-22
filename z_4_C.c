#include <stdio.h>
#include <string.h>

#define YES 1
#define NO 0

const int INF_STROKE = 150;

int main()
{
    printf("This is a simple program that changes symbols in an opposite way\n");
    char stroke[INF_STROKE];
    int word = NO, symb = 0;
    gets(stroke);

    if (strlen(stroke) == 1) //! Проверка на пустую строку
    {
        printf("ERROR. Your stroke is empty, try again!\n");
        return 0;
    }

    #ifdef for (int cnt = 0; cnt < strlen(stroke); cnt++)
    {
        ++symb;
        printf("\nsee cnt = %d", cnt);

        if (*(stroke+cnt) != '\n' && *(stroke+cnt) != ' ' && word == NO && *(stroke+cnt) != '\0')
            word = YES;

        if (word == NO && symb != 0)   // второе - проверяем на начало строки
                for (int i = (cnt-symb-1); i <= cnt-1 ; ++i)
                        *(stroke+(cnt-symb-1)) = *(stroke+cnt-1);

        if (*(stroke+cnt) == ' ' || *(stroke+cnt) == '\n' && word == YES )
        {
            word = NO;
            printf("\nsee the symbol, %d\n", symb-1);
            symb = 0;
        }

    }
    #endif

    for (int cnt = 0; cnt < strlen(stroke); cnt++)
    {
        if (*(stroke+cnt) != ' ' && *(stroke+cnt) != '\n' && word == NO)
            word = YES;

        if (word == YES && *(stroke+cnt) != ' ' && *(stroke+cnt) != '\n')
            ++symb;

        if (word == YES && *(stroke+cnt) == ' ' || *(stroke+cnt) == '\n')
        {
            word = NO;

            for (int i = 0; i < symb/2; ++i)
            {
                *t = *(stroke+cnt+i);
                *(stroke+cnt+i) = *(stroke+symb-i)
                *(stroke+cnt+symb-i) = *t;
            }
        }
    }

    printf("SEE THE POWER OF A NEW STROKE!!!\n");
    puts(stroke);
    return 0;

}
