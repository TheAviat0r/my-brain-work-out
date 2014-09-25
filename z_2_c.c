//!{======================================================================
//!     @file       z_2_c.c
//!     @author     Niatshin Bulat
//!     @task       This program gives you the value of the prime number
//!     @par        User types the number, this program shows him
//!                 the value of this prime number;
//!     @warning    In my opinion all is ok
//!}======================================================================
#include <stdio.h>
//!=======================================================================
int isprime(int value);
int search_numb(int numb);
//!=======================================================================
int main()
{
    printf("=====================================\n");
    printf("This is a program that search prime value\n");
    printf("Type the number of your prime value\n");
    printf("=====================================\n");

    int numb = 0;
    if (scanf("%d", &numb) != 1)
    {
        printf("ERROR. You should type only 1 value, not 2 or others.\n");
        printf("Try again!\n");
        return 0;
    }


    printf("Your value = %d", search_numb(numb));

    return 0;
}
//!=======================================================================
int search_numb(int numb)
{
    int cnt = 0;
    unsigned long work = 2;

    while (cnt <= numb)
    {
        if (isprime(work))
            ++cnt;
        ++work;
    }

    return work;
}
//!=======================================================================
int isprime(int value)
{
    if (value == 1)
        return 0; //! 1 is not a simple falue
    for (int i = 2; i < (value-1); ++i)
        if ((value-1) % i == 0)
            return 0;
    return 1;
}
