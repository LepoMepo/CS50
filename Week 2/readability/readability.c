#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

int CountLetters(string text);
int CountWords(string text);
int CountSentences(string text);

int main(void)
{

    string text = get_string("Text: ");
    int letters_number = CountLetters(text);
    int words_number = CountWords(text);
    int sentences_number = CountSentences(text);

    float grade = 0.0588 * letters_number / (float)words_number * 100 - 0.296 * sentences_number / (float)words_number * 100 - 15.8;

    int i_grade = round(grade);

    if (i_grade < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (i_grade >= 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %i\n", i_grade);
    }

}

int CountLetters(string t)
{
    int sum = 0;

    for (int i = 0, n = strlen(t); i < n; i++)
    {
        if (isalpha(t[i]))
        {
            sum++;
        }
    }
    return sum;
}

int CountWords(string t)
{
    int sum = 1;

    for (int i = 0, n = strlen(t); i < n; i++)
    {
        if (isblank(t[i]))
        {
            sum++;
        }
    }
    return sum;
}

int CountSentences(string t)
{
    int sum = 0;

    for (int i = 0, n = strlen(t); i < n; i++)
    {
        if (t[i] == '.' || t[i] == '?' || t[i] == '!')
        {
            sum++;
        }
    }
    return sum;
}
