#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

char letters[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
                  'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};

int CheckKey(char k[], int l);
int FindLoc(char letter);

int main(int argc, string argv[])
{
    if (argc == 1 || argc > 2)
    {
        printf("There must be the key or the key must contain 26 characters\n");
        return 1;
    }

    char key[26];

    for (int i = 0, n = strlen(argv[1]); i < n; i++)
    {
        key[i] = tolower(argv[1][i]);
    }

    int check = CheckKey(key, strlen(argv[1]));
    if (check == 1)
    {
        printf("Key must contain at least 26 characters\n");
        return 1;
    }
    else if (check == 2)
    {
        printf("All characters in key must be different\n");
        return 1;
    }
    else
    {
        int key_length = strlen(argv[1]);
        int difference[key_length];

        for (int i = 0; i < key_length; i++)
        {
            difference[i] = key[i] - letters[i];
        }
        string plaintext = get_string("plaintext: ");
        for (int i = 0, n = strlen(plaintext); i < n; i++)
        {
            if (isalpha(plaintext[i]))
            {
                int letter_loc = FindLoc(plaintext[i]);
                if (letter_loc == 404)
                {
                    printf("Error!\n");
                    return 1;
                }
                plaintext[i] = plaintext[i] + difference[letter_loc];
            }
        }
        printf("ciphertext: %s", plaintext);
        printf("\n");
        return 0;
    }
}

int CheckKey(char k[], int len)
{
    if (len != 26)
    {
        return 1;
    }
    for (int i = 0; i < len; i++)
    {
        int result = 0;
        for (int j = 0; j < len; j++)
        {
            if (letters[i] == k[j])
            {
                result++;
            }
        }
        if (result != 1)
        {
            return 2;
        }
    }
    return 0;
}

int FindLoc(char letter)
{
    for (int i = 0; i < 26; i++)
    {
        if (tolower(letter) == letters[i])
        {
            return i;
        }
    }
    return 404;
}
