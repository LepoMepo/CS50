#include <stdio.h>
#include <cs50.h>


int GetHeight();
void PrintSpaces(int n);
void PrintHashes(int n);
void PrintGap();

int main()
{
    int height = GetHeight();
    for (int i = 1; i <= height; i++)
    {
        PrintSpaces(height - i);
        PrintHashes(i);
        PrintGap();
        PrintHashes(i);
        printf("\n");
    }
}

int GetHeight(void)
{
    int n;
    do
    {
        n = get_int("Height:\n");
    }
    while ((n < 1) || (n > 8));

    return n;
}

void PrintSpaces(int n)
{
    for (int i = n; i > 0; i--)
    {
        printf(" ");
    }
}

void PrintHashes(int n)
{
    for (int i = 1; i <= n; i++)
    {
        printf("#");
    }
}

void PrintGap(void)
{
    printf("  ");
}
