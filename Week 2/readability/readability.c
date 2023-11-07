#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

// Functions:
int count_letters(string s);
int count_words(string s);
int count_sentences(string s);
void print_grade(float i);

// This program takes a text and determinates its reading level.
int main(void)
{
    string text = get_string("Text: ");

    int letters = count_letters(text);
    int words = count_words(text);
    int sentences = count_sentences(text);

    float L = ((float) letters / (float) words) * 100;
    float S = ((float) sentences / (float) words) * 100;
    float subindex = 0.0588 * L - 0.296 * S - 15.8;
    int index = round(subindex);

    print_grade(index);
}

int count_letters(string s)
{
    int lenght = strlen(s);
    int ls;
    int count = 0;
    for(int i = 0; i < lenght; i++)
    {
        ls = (int) tolower(s[i]);
        if(ls >= 97 && ls <= 122)
        {
            count++;
        }
    }
    return count;
}

int count_words(string s)
{
    int lenght = strlen(s);
    int count = 1;
    for(int i = 0; i < lenght; i++)
    {
        if(isspace(s[i]))
        {
            count++;
        }
    }
    return count;
}

int count_sentences(string s)
{
    int lenght = strlen(s);
    int count = 0;
    for(int i = 0; i < lenght; i++)
    {
        if((s[i] == '.') || (s[i] == '!') || (s[i] == '?'))
        {
            count++;
        }
    }
    return count;
}

void print_grade(float i)
{
    if(i >= 16)
    {
        printf("Grade 16+\n");
    }
    else if(i < 1)
    {
        printf("Before Grade 1\n");
    }
    else
    {
        printf("Grade %i\n", (int) round(i));
    }
}