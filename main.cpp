#include <stdio.h>
#include <math.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>

#define YES 1
#define NO 0

const int INF_STR = 150;

//!=========================
void task_one(void);
//!=========================
    int measure(int number);
//!=========================
void task_two(void);
//!=========================
    int isprime(int value);   //Like this I show functions that are inside other function
//!=========================
    int search_numb(int numb);
//!=========================
void task_three(void);
//!=========================
void task_four(void);
//!=========================
void task_five(void);
//!=========================
void task_six(void);
//!=========================
void task_seven(void);
//!=========================
void task_eight(void);
//!=========================
void task_nine(void);
//!=========================
void task_ten(void);
//!=========================



int main()
{
    printf("=================================================================\n");
    printf("Here you can see the list of the tasks that you can choose\n");
    printf("Type number from 1 to 10. For exit type 11\n");

    int num_task = 0;

    while (num_task != (11))
    {
        if( scanf("%d", &num_task) == 1)
        {
            assert(num_task > 0);
            switch(num_task)
            {
                case(1): task_one();
                    break;
                case(2): task_two();
                    break;
                case(3): task_three();
                    break;
                case(4): task_four();
                    break;
                case(5): task_five();
                    break;
                case(6): task_six();
                    break;
                case(7): task_seven();
                    break;
                case(8): task_eight();
                    break;
                case(9): task_nine();
                    break;
                case(10): task_ten();
                    break;
                case(11): printf("Here we go! Program is finished.\n");
                    break;
                default: printf("Wrong value! It should be positive int from 1 to 10!\n");
                    break;
            }
        }
        else
        {
            printf("ERROR! Your value is not int! Try again!\n");
            scanf("%d", &num_task);
        }
        printf("\n");
    }

    printf("Nice to see you! Have a nice day :) \n");
    return 0;
}
//!=====================================================================
void task_one(void)
{
    printf("=====================================\n");
    printf("This program is solving task #1\n");
    printf("Type your int number pls\n");

    int number = 0;
    assert(scanf("%d",&number) == 1 && number > 0);

    int result = measure(number);
    assert(result == 1 || result == 0);

    switch(result)
    {
        case(1): printf("YES\n");
            break;
        case(0): printf("NO\n");
            break;
        default: printf("ERROR. Check the sources\n");
            break;
    }
}
//!=====================================================================
int measure(int number)
{
    if (number == 1)
        return 1;
    if (number %4 == 1)
        return 1;
    if (number %4 == 2)
        return 0;
    if (number %4 == 3)
        return 1;
    if (number %4 == 0)
        return 1;
}
//!=====================================================================
void task_two(void)
{
    printf("=====================================\n");
    printf("This is a program that search prime value\n");
    printf("Type the number of your prime value\n");
    printf("=====================================\n");

    int numb = 0;
    assert(scanf("%d", &numb) == 1 && numb > 0);

    printf("Your value = %d", search_numb(numb));
}
//!=====================================================================
int search_numb(int numb)
{
    int cnt = 0;
    unsigned long work = 2;

    while (cnt <= numb)
    {
        assert(work > 0);
        if (isprime(work))
            ++cnt;
        ++work;
    }

    return work;
}
//!=====================================================================
int isprime(int value)
{
    if (value == 1)
        return 0; //! 1 is not a simple falue
    for (int i = 2; i < sqrt(value); ++i)
        if ((value-1) % i == 0)
            return 0;
    return 1;
}
//!=====================================================================
void task_three(void)
{
    long n = 0;
    int x = 0,y = 0,z = 0, t = 0;

    printf("=====================================================================\n");
    printf("Here you can see the program that shows you the sum of 4 squares\n");
    printf("Type your number pls\n");
    printf("=====================================================================\n");

    if (scanf("%ld", &n) == 1)
    {
    assert(n > 0);
    do
    {
        x++;
        y=0;
        do
        {
            y++;
            z=0;
            do
            {
                z++;
                t=-1;
                    do
                    {
                        t++;
                    }
                    while \
                        ( (x*x+y*y+z*z+t*t!=n)&&(t<n));
            }
            while \
                ((x*x+y*y+z*z+t*t!=n)&&(z*z<n));
        }
        while \
            ((x*x+y*y+z*z+t*t!=n)&&(y*y<n));
    }
    while \
        ((x*x+y*y+z*z+t*t!=n)&&(x*x<n));

    printf("See your result: a = %d, b = %d, c = %d, d = %d\n", x, y,z, t);
    }
    else printf("ERROR! WRONG VALUE! Try again and type the number!\n)");
}
//!=====================================================================
void task_four(void)
{
    char my_str[INF_STR];
    printf("\n=====================================================\n");
    printf("Here you can see the program that inverts words in stroke\n");
    printf("Type your stroke\n");

    gets(my_str);

    int len = 0;
    len = strlen(my_str);

    int thebegin = 0, theend = 0, word = NO;

    for(int i = 0; i < len; ++i)
    {
        assert( 0 <= i && i < len);

        char work = ' ';
        if (isalpha(my_str[i]) && word == NO)
        {
            thebegin = i;
            word = YES;
        }

        if ( (isspace(my_str[i]) || my_str[i] == '\0') && word == YES)
        {
            theend = i - 1;
            word = NO;

            my_str[i] = '\0';
            strrev(my_str+thebegin);
            my_str[i] = work;
        }
    }


    /*for (int i = 0; i <= len; ++i)
    {
        assert(0 <= i && i <= len);

        char work = ' ';
        if(my_str[i] !=' ' && (my_str[i+1] = ' '|| my_str[i+1] == '\n' ||my_str[i] == 'NULL'))
        {
            work = my_str[i+1];
            my_str[i+1] = '\0';
            strrev(my_str);
            my_str[i+1] = work;
        }
    }*/
    puts(my_str);
    printf("If you want to continue, type number of the next task\n");
    printf("If you want to exit, type 11\n");
}
//!=====================================================================
void task_five(void)
{

}
//!=====================================================================
void task_six(void)
{
}
//!=====================================================================
void task_seven(void)
{
}
//!=====================================================================
void task_eight(void)
{
}
//!=====================================================================
void task_nine(void)
{
}
//!=====================================================================
void task_ten(void)
{
}
