//This program verify if a credit card is an American Express, MasterCard, Visa or none of them

#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

// Functions
long credit_number(void);
bool checksum(long n);
void check_type(long n);
int digits(long n);
int Luhn_sum(long n);
int first_2(long n);

int main(void)
{
    bool check1;
    // Asks for the credit card number
    long number = credit_number();
    // Print the type of the credit card
    check_type(number);

}

//Checks if the credit card is valid and print his type
void check_type(long n)
{
    char type[20] = "INVALID";
    int fd = first_2(n);
    if (checksum(n))
    {
        if (digits(n) == 15 && (fd == 34 || fd == 37))
        {
            strcpy(type, "AMEX");
        }
            else if ((digits(n) == 13 || digits(n) == 16) && (fd/10 == 4))
            {
                strcpy(type, "VISA");
            }
                else if (digits(n) == 16 && (fd == 51 || fd == 52 || fd == 53 || fd == 54 || fd == 55))
                {
                    strcpy(type, "MASTERCARD");
                }
    }

    printf("%s\n", type);
}

// Calculate the first two digits of a credit card
int first_2(long n)
{
    return (n / pow(10, (digits(n) - 2) ));
}

long credit_number()
{
    long n;
    do
    {
        n = get_long("Number: ");

    }
    while (n < 0);
    return n;
}

// Checks if the checksum is valid
bool checksum(long n)
{
    int mod = Luhn_sum(n) % 10;
    return mod == 0;
}

int Luhn_sum(long n)
{
    long aux0 = n;
    int aux;
    int sum = 0;

    // Adds the first part of Luhn's algorithm to the sum
    do
    {
        n = n / 10;
        aux = (n % 10) * 2;
        if (aux > 9)
        {
            aux = (aux / 10) + (aux % 10);
        }
        sum = sum + aux;
        n = n / 10;
    }
    while (n > 0);

    // Adds the second part of Luhn's algorithm to the sum
    do
    {
        sum = sum + (aux0 % 10);
        aux0 = aux0 / 100;
    }
    while (aux0 > 0);
    return sum;
}

// Counts the digits of a number
int digits(long n)
{
    int count = 0;;
    do
    {
        n /= 10;
        ++count;
    }
    while (n != 0);

    return count;
}