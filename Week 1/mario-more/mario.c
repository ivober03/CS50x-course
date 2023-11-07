#include <cs50.h>
#include <stdio.h>

// Functions:
int request_height(void);
void draw_Pyramid(int h);
void print_spaces(int h, int i);
void print_hashes(int i);

int main(void)
{
    // Request height
    int height = request_height();

    // Draw pyramid
    draw_Pyramid(height);
}

int request_height()
{
    int h;
    do
    {
        h = get_int("Height: ");
    }
    while (h < 1 || h > 8);
    return h;
}

void draw_Pyramid(int h)
{
    for (int i = 1; i <= h; i++)
    {
        print_spaces(h, i);
        print_hashes(i);
        print_spaces(3, 1);
        print_hashes(i);
        printf("\n");
    }
}

void print_spaces(int h, int i)
{
    for (int j = 0; j < (h - i); j++)
    {
        printf(" ");
    }
}

void print_hashes(int i)
{
    for (int k = 0; k < i; k++)
    {
        printf("#");
    }
}
