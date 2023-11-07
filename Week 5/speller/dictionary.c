// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <strings.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// TODO: Choose number of buckets in hash table
const unsigned int N = 41999; // odd prime number to avoid collisions
int word_pos = 0;

// Hash table
node *table[N];

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // Hash word
    int hshcd = hash(word);

    // Create cursor that points to the head of the linked list
    node *cursor = table[hshcd];

    // Loop until the end of the list is reached
    while (cursor != NULL)
    {
        // Compare the words
        if (strcasecmp(cursor->word, word) == 0)
        {
            return true;
        }
        // If the words are not the same move the cursor to the next word
        cursor = cursor->next;
    }

    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    unsigned int hash = 401; // odd prime number for avoiding collisions
    for (int i = 0, n = strlen(word); i < n; i++)
    {
        hash = 31 * hash + toupper(word[i]); // 31 for better distribution and avoiding collisions
    }
    return hash % N;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // Opens dictionary
    FILE *file = fopen(dictionary, "r");

    // Check if null
    if (file == NULL)
    {
        return false;
    }
    // Initialise word array
    char word[LENGTH + 1];

    // Read strings from file one at a time
    while (fscanf(file, "%s", word) == 1)
    {
        // Create new node for each word
        node *n = malloc(sizeof(node));
        if (n == NULL)
        {
            return false;
        }

        // Copies word into node ysubg strcioy
        strcpy(n->word, word);

        // Hash word to obtain a hash value
        int h = hash(word);

        // Inserts new nodes at h location
        if (table[h] == NULL)
        {
            n->next = NULL;
        }

        else
        {
            n->next = table[h];
        }
        table[h] = n;
        word_pos++;
    }
    fclose(file);

    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    return word_pos;
}


// Recursive function to free a linked list
void free_list(node *n)
{
    // Base case
    if (n->next == NULL)
    {
        free(n);
    }
    // General case
    else
    {
        free_list(n->next);
        free(n);
    }
}

// Unloads dictionary from memory
bool unload(void)
{
    // Loop every linked list of the hash table
    for (int i = 0; i < N; i++)
    {
        if (table[i] != NULL)
        {
            free_list(table[i]);
        }
    }
    return true;
}