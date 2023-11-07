// This program calculate the years required for a population of llamas to grow from the start size to the end size

#include <cs50.h>
#include <stdio.h>

// Functions:
int initial_Population(void);
int final_Population(int s);
int calculate_Years(int s, int e);

int main(void)
{
    // TODO: Prompt for start size
    int start_size = initial_Population();

    // TODO: Prompt for end size
    int end_size = final_Population(start_size);

    // TODO: Calculate number of years until we reach threshold
    int years = calculate_Years(start_size, end_size);

    // TODO: Print number of years
    printf("Years: %i\n", years);
}

int initial_Population(void)
{
    int s;
    do
    {
        s = get_int("Start size (greater than 9): ");
    }
    while (s < 9);
    return s;
}

int final_Population(int s)
{
    int e;
    do
    {
        e = get_int("End size (greater than the initial): ");
    }
    while (e < s);
    return e;
}

int calculate_Years(int s, int e)
{
    int y;
    for (y = 0; s < e; y++)
    {
        s = s + (s / 3) - (s / 4);
    }
    return y;
}