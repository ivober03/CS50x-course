#include <cs50.h>
#include <stdio.h>


bool prime(int number);

int main(void)
{
    int min;
    do
    {
        min = get_int("Minimum: ");
    }
    while (min < 1);

    int max;
    do
    {
        max = get_int("Maximum: ");
    }
    while (min >= max);

    for (int i = min; i <= max; i++)
    {
        if (prime(i))
        {
            printf("%i\n", i);
        }
    }
}

bool prime(int number)
{
    // TODO
    bool prime = true;
    int multiple = 0;
    int cant = 2;
    // see if the number is 1, if that happens the number it's not prime
    if (number == 1)
    {
        prime = false;
    }
    else
    {
        // try dividing every number from 2 up to number-1
        while ((prime) && (cant < number))
        {
            // if the number can be divided by any number between the range (without including the number itself) then it's not prime
            if ((number % cant == 0) && cant != number)
            {
                prime = false;
            }
            cant++;
        }
    }
    return prime;
}
