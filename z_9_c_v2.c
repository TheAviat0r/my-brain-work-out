//!=======================================================================================
//!     @file           z_9_c_v2.c
//!     @author         Niatshin Bulat
//!     @task           Ðåøåíèå çàäà÷è ïî ïîëó÷åíèþ ñóììû äâóõ äâîè÷íûõ ÷èñåë
//!                     Îñíîâíîé ïðèíöèï - ñ÷èòûâàåì 2 ñòðîêè, ïåðåäàåì èõ â âèäå ïàðàìåòðîâ
//!                     îñíîâíîé èñïîëíèòåëüíîé ôóíêöèè
//!                     Îíà âûçûâàåò äðóãóþ ôóíêöèþ, êîòîðàÿ ðàçâîðà÷èâàåò çàäàííûå ÷èñëà,
//!                     çàòåì ñïðàâà íàëåâî ñêëàäûâàåò, çàïèñûâàÿ âñå èçìåíåíèÿ â áîëüøåå ÷èñëî.
//!                     Çàòåì ñíîâà âûçûâàåòñÿ ðàçâîðà÷èâàþùàÿ ôóíêöèÿ è ðåçóëüòàò ïðèíèìàåò
//!                     íîðìàëüíûé âèä.
//!                     Ïðîáëåìà ñîñòîèò íà äàííûé ìîìåíò â òîì, ÷òî ó ìåíÿ íå êîìïèëèòñÿ êîä.
//!     @warning        This code assumes that user is typing correct numbers!
//!=======================================================================================

#include <stdio.h>
#include <string.h>

//!=======================================================================================
const int INF_NUM = 150;
void get_sum(char *numb_1, char *numb_2, char *res);
void copy_s(char *numb_1, char *res_c);
void reverse_str(char *numb);
//!=======================================================================================
int main()
{
    printf("========================================================\n");
    printf("This program is making sums of binary numbers\n");
    printf("Type 2 binary numbers\n");
    printf("========================================================\n");

    char numb_1[INF_NUM];
    char numb_2[INF_NUM];
    char res[INF_NUM];

    gets(numb_1);
    gets(numb_2);

    get_sum(numb_1, numb_2, res);
    printf("See your result =");
    puts(res);
    printf("\n");

    return 0;
}
//!=======================================================================================
void reverse_str(char numb[])
{
    int len_numb = strlen(numb);
    for (int i = 0; i < len_numb/2; ++i)
    {
        char work = numb[i];
        numb[i] = numb[len_numb - i];
        numb[len_numb - i] = work;
    }
}
//!=======================================================================================
void copy_s(char numb_1[], char res_c[])
{
    int len = strlen(numb_1);
    for (int i = 0; i < len; ++i)
        res_c[i] = numb_1[i];
}
//!=======================================================================================
void get_sum(char numb_1[INF_NUM], char numb_2[INF_NUM], char res[INF_NUM])
{
    reverse_str(numb_1);
    reverse_str(numb_2);

    int len_st = 0, len_big = 0, which_big = 0;

    if (strlen(numb_1) < strlen(numb_2))
    {
        which_big = 2;
        len_st = strlen(numb_2);
    }
    else
    {
        len_st = strlen(numb_2);
        which_big = 1;
    }

    for (int i = 0; i < len_st; ++i)
    {
        if ( strlen(numb_2) < strlen(numb_1) )
        {
            if (numb_1[i] == '0')
                numb_1[i] = numb_2[i];
            else
            {
                if (numb_2[i] == '1')
                {
                    numb_1[i] = '0';
                    numb_1[i+1] = '1';
                }
            }
        }
        else
        {
            if (numb_2[i] == '0')
                numb_2[i] = numb_1[i];
            else
            {
                if (numb_1[i] == '1')
                {
                    numb_2[i+1] = '1';
                    numb_2[i]  = '0';
                }
            }
        }
    }
if (which_big == 2)
    copy_s(numb_2, res);
else
    copy_s(numb_1, res);

    reverse_str(res);
}
