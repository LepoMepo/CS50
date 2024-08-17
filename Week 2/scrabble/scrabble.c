#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

int CountPoints(string word);
void DefineWinner(int points_1, int points_2);

int POINTS[] = {1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3, 1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10};

int main()
{

    string word_1 = get_string("Player 1: ");
    string word_2 = get_string("Player 2: ");

    int sum_1 = 0;
    int sum_2 = 0;

    sum_1 = CountPoints(word_1);
    sum_2 = CountPoints(word_2);

    DefineWinner(sum_1, sum_2);
}

int CountPoints(string word)
{
    int sum = 0;

    for (int i = 0, n = strlen(word); i < n; i++)
    {
        word[i] = tolower(word[i]);
        if (word[i] >= 'a' && word[i] <= 'z')
        {
            sum += POINTS[word[i] - 'a'];
        }
    }
    return sum;
}

void DefineWinner(int points_1, int points_2)
{
    if (points_1 > points_2)
    {
        printf("Player 1 wins!\n");
    }
    else if (points_1 < points_2)
    {
        printf("Player 2 wins!\n");
    }
    else
    {
        printf("Tie!\n");
    }
}
