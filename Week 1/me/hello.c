#include <stdio.h>
#include <cs50.h>

int main()
{
    string s = get_string("What's your name?\n");
    printf("hello, %s\n", s);
}