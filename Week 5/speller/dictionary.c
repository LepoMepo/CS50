// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
} node;

// TODO: Choose number of buckets in hash table
const unsigned int N = 6000;

// Hash table
node *table[N];

int counter = 0;

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    int hash_number = hash(word);
    if (table[hash_number] == NULL)
    {
        return false;
    }
    else
    {
        node *cursor = table[hash_number];
        while (cursor != NULL)
        {
            if (!strcasecmp(cursor->word, word))
            {
                return true;
            }
            else
            {
                cursor = cursor->next;
            }
        }
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // TODO: Improve this hash function
    unsigned int hash = 0;
    for (int i = 0; i < strlen(word); i++)
    {
        hash += tolower(word[i]);
    }
    return hash;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // TODO

    FILE *source = fopen(dictionary, "r");
    if (source == NULL)
    {
        printf("Unable to load the dictionary\n");
        return false;
    }

    char new_word[LENGTH + 1];
    unsigned int number;

    while(fscanf(source, "%s", new_word) != EOF)
    {
        node *buffer = malloc(sizeof(node));
        if (buffer == NULL)
        {
            printf("No space\n");
            return false;
        }
        strcpy(buffer->word, new_word);
        buffer->next = NULL;
        counter++;
        number = hash(new_word);
        if (table[number] == NULL)
        {
            table[number] = buffer;
        }
        else
        {
            buffer->next = table[number];
            table[number] = buffer;
        }
    }

    fclose(source);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    // TODO
    return counter;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // TODO
    node *cursor, *tmp;
    for (int i = 0; i < N; i++)
    {
        cursor = table[i];
        tmp = table[i];
        while (cursor != NULL)
        {
            cursor = cursor->next;
            free(tmp);
            tmp = cursor;
        }
    }
    return true;
}
