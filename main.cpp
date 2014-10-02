//!====================================================================================
//!     @file       main.cpp
//!     @author     Niatshin Bulat
//!     @condition  Here you can see the program that solves 10 of tasks
//!     @task       I am too lazy to write something rightly here
//!     @warning    I tryed to debug task 9 and others and in some cases I failed
//!                 and now I am not able to find mistakes that I did
//!====================================================================================

#include <stdio.h>
#include <math.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
//!====================================================================================
#define YES 1
#define NO 0
//!====================================================================================
const int INF_STR = 150;
const int INF_NUM = 10000;
//!====================================================================================
struct answer_t
{
        int point_one;
        int point_two;
        double rebro;
};

struct point_t
{
        double coord_x;
        double coord_y;
};

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
    answer_t len_max(point_t buf[], int number);
//!=========================
void task_six(void);
//!=========================
void task_seven(void);
//!=========================
void task_eight(void);
//!=========================
void task_nine(void);
//!=========================
    void get_binsum(char number_one[], char number_two[]);
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
    for (int i = 2; i < (value); ++i)
        if ((value) % i == 0)
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
    scanf(" ");

    gets(my_str);

    int len = 0;
    len = strlen(my_str);

    int thebegin = 0, theend = 0, word = NO;

    for(int i = 0; i < len; ++i)
    {
        assert( 0 <= i && i < len);

        char work = ' ';
        char safe = 0;
        if (isalpha(my_str[i]) && word == NO)
        {
            thebegin = i;
            word = YES;
        }

        if ( (isspace(my_str[i]) || my_str[i+1] == '\0') && word == YES)
        {
            theend = i - 1;
            word = NO;

            safe = my_str[i];
            my_str[i] = '\0';
            if (my_str[i+1] == 0)
                my_str[i] = safe;
            strrev(my_str+thebegin);
            my_str[i] = work;
        }
    }


    printf ("you got: <%s>\n", my_str);
    puts(my_str);
    printf("If you want to continue, type number of the next task\n");
    printf("If you want to exit, type 11\n");
}
//!=====================================================================
void task_five(void)
{
    point_t buf [1000] = {};
    answer_t result = {};


    printf("Type the number of points");

    int number = 0;
    scanf("%d", &number);

    printf("Now type coordinates of your points");

    for (int i = 0; i < number; ++i)
        scanf("%lf %lf", &buf[i].coord_x, &buf[i].coord_y);

    result = len_max(buf, number);

    printf("!*!*!*!*!*!*!*!*!*!*!*!*!*!\n");
    printf(" N1 = %d N2 = %d Length = %10.6lf\n", result.point_one, result.point_two, \
                                                    result.rebro);


}
//!=====================================================================
double get_len (point_t a, point_t b)
{
    double len = sqrt(pow(a.coord_x - b.coord_x,2) + pow(a.coord_y - b.coord_y,2));
    return len;
}

answer_t len_max(point_t buf[], int number)
{
    answer_t result = {};
    result.point_one = 1;
    result.point_two = 2;

    double maxlen = get_len(buf[0], buf[1]);

    for (int i = 0; i < number; ++i)
        for (int j = 1; j < number; ++j)
            if (get_len(buf[i], buf[j]) > maxlen)
            {
                maxlen = get_len(buf[i], buf[j]);
                result.point_one = i;
                result.point_two = j;
            }

    result.rebro = maxlen;

    return result;
}
//!=====================================================================
//!=====================================================================
void task_six(void)
{
    printf("\n========================================================\n");
    printf("Type your number\n");

    int number = 0, sum = 0;
    scanf("%d", &number);

    for (int i = 1; i <= number; ++i)
    {
        if (number % i == 0)
            sum +=i;
    }

    printf(" sum = %d\n", sum);

}
//!=====================================================================
void task_seven(void)
{
    printf("||-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-||\n");
    printf("WELCOME! Type how many prime numbers you want to get\n");

    int number = 0;
    int values[INF_NUM] = {};
    scanf("%d", &number);
    assert(number > 0);

    printf("Now type your numbers\n");

    for (int i = 0; i < number; ++i)
        scanf("%d", &values[i]);

    for (int i = 0; i < number; ++i)
        printf("value %d = %d ", (i+1) ,search_numb(values[i]));

    printf("NICE TO SEE YOU!\n");

}
//!=====================================================================
void task_eight(void)
{
    task_three();
}
//!=====================================================================
void task_nine(void)
{
    printf("^_^_^_^_^_^_^_^_^_^_^_^_^_^_^_^_^_^_^_^_^_^_^_^_^_^_^_^_^_^\n");
    printf("This is nyan binary sum\n");
    printf("Type your two strokes\n");

    char number_one [INF_STR] = {};
    char number_two [INF_STR] = {};
    //char result [INF_STR] = {};

    scanf(" ");
    gets(number_one);
    scanf(" ");
    gets(number_two);

    printf("N1 = <%s>, N2 = <%s>\n", number_one, number_two);

    strrev(number_one);
    strrev(number_two);
    printf("REV N1 = <%s>, N2 = <%s>\n", number_one, number_two);

    get_binsum(number_one, number_two);

}

void get_binsum(char number_one[], char number_two[])
{
    int len_one = strlen(number_one);
    int len_two = strlen(number_two);

    assert(len_one >= 0 && len_two >= 0);

    if (len_one > len_two)
    {
        for (int i = 0; i < len_two; ++i)
        {
            assert(0 <= i && i < len_two);

            if (number_one[i] == '0')
                number_one[i] = number_two[i];
            else
            {
                if (number_two[i] == '1')
                {
                    number_one[i] == '0';

                    int cnt_one = 1;
                    int cnt_beg = i, cnt_end = 0;;

                    int cnt_symb = 0;

                    for (int j = i+1; j <= len_two; ++j)
                        if (number_one[j] == '1' && (number_one[j+1] == '1' || number_one[j+1] == 0))
                        {
                            ++cnt_symb;
                            cnt_end =j;
                            number_one[j] = 0;
                            if (number_one[j+1] == '0' || number_one[j+1] == 0)
                            {
                                    i = j - 1;
                                    break;
                            }
                        }

                    for (int k = cnt_end; k <= (cnt_symb+cnt_end); ++k)
                        number_one[k] = '1';
                }
            }
        }

        printf("see your sum = ");
        strrev(number_one);
        puts(number_one);
        printf("\nthis is the end of the task. Type new number pls\n");
    }
    else
    {
        for (int i = 0; i < len_one; ++i)
        {
            assert(0 <= i && i < len_one);

            if (number_two[i] == '0')
                number_two[i] = number_one[i];
            else
            {
                if (number_one[i] == '1')
                {
                    number_two[i] == '0';

                    /*int cnt_one = 1;
                    int cnt_beg = i;
                    int cnt_end = 0;*/

                    for (int j = i+1; j <= (len_one); ++j)
                        if (number_two[j] == '1' && (number_two[j+1] == '1' || number_two[j+1] == 0))
                        {
                            number_two[j] = '0';
                            number_two[j+1] = '1';
                                if (number_two[j+1] == '0' || number_two[j+1] == 0);
                                    break;
                        }

                    //for (int i = cnt_beg; i <= cnt_end; ++i);
                        //number_one[i] = '1';
                }
            }
        }

        printf("see your sum = ");
        strrev(number_two);
        puts(number_two);
        printf("\nthis is the end of the task. Type new number pls\n");
    }

}


//!=====================================================================
void task_ten(void)
{
}
