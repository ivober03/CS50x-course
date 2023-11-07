// Write a function to replace vowels with numbers
// Get practice with strings
// Get practice with command line
// Get practice with switch

#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

// Functions:
bool its_vowel(char c);
string replace(string s);

int main(int argc, string argv[])
{
    if(argc == 2)
    {
        printf("%s\n", replace(argv[1]));
    } else
    {
        printf("The command-line argument must be exactly one\n");
        return 1;
    }
}

bool its_vowel(char c)
{
    bool iv = false;
    if(c == 'a' || c == 'e' || c == 'i' || c == 'o')
    {
        iv = true;
    }
    return iv;
}

string replace(string s)
{
    int lenght = strlen(s);
    for(int i = 0; i < lenght; i++)
    {
        char c = tolower(s[i]);
        if(its_vowel(c))
        {
            switch(c)
            {
                case 'a':
                    s[i] = '6';
                    break;
                case 'e':
                    s[i] = '3';
                    break;
                case 'i':
                    s[i] = '1';
                    break;
                case 'o':
                    s[i] = '0';
                    break;
            }
        }
    }
    return s;
}