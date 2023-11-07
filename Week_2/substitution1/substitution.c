#include <cs50.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

// Functions:
int valid(string s);
string encrypt(string key, string t);
int letter_pos(char c);
bool duplicated_characters(string s);

int main(int argc, string argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./substitution key\n");
        return 1;
    } else if(valid(argv[1]) == -1)
    {
        printf("Key must contain 26 characters\n");
        return 1;
    } else if ((valid(argv[1])) == 1)
    {
        printf("Key must only contain alphabetic characters\n");
        return 1;
    } else if (duplicated_characters(argv[1]))
    {
        printf("Key must not contain repeated characters\n");
        return 1;
    } else if(valid(argv[1]) == 0)
    {
        string text = get_string("plaintext: ");
        printf("ciphertext: %s\n", encrypt(argv[1], text));
    }
}

string en crypt(string key, string t)
{
    int lenght = strlen(t);
    for(int j = 0; j < lenght; j++)
    {
        if(isalpha(t[j]) != 0 && isupper(t[j]))
        {

            t[j] = toupper(key[(letter_pos(t[j]))]);
        }
        else if(isalpha(t[j]) != 0)
        {
            t[j] = tolower(key[(letter_pos(t[j]))]);
        }
    }
    return t;
}

bool duplicated_characters(string s)
{
    bool d = false;
    int lenght = strlen(s);
    for(int i = 0; i < lenght && !d; i++)
    {
        for(int j = i + 1; j < lenght && !d; j++)
        {
            if(s[i] == s[j])
            {
                d = true;
            }
        }
    }
    return d;
}

int valid(string s)
{
    bool cond = 0;
    int i = strlen(s);
    if(i == 26)
    {
        for(int j = 0; j < i && cond; j++)
        {
            if(isalpha(s[j]) == 0)
            {
                cond = 1;;
            }
        }
    }
    else
    {
        cond = -1;
    }
    return cond;
}

int letter_pos(char c)
{
    char cl = tolower(c);
    int ascii = (int) cl;
    return (ascii - 97);
}
