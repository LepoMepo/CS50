#include <cs50.h>
#include <stdio.h>

int CheckLuhn(long Number);
void GetCreditCardName(long Number, int f);

int main(void)
{
    long CreditNumber = get_long("Number: \n");
    int f = CheckLuhn(CreditNumber);
    GetCreditCardName(CreditNumber, f);
}

int CheckLuhn(long Number)
{
    int f = 1;
    int odd_sum = 0;
    int even_sum = 0;
    int sum = 0;
    do
    {
        if (f == 1)
        {
            odd_sum += Number % 10;
            f = 0;
        }
        else
        {
            int even_number = Number % 10 * 2;
            if (even_number > 9)
            {
                even_sum += even_number % 10 + even_number / 10;
            }
            else
            {
                even_sum += even_number;
            }
            f = 1;
        }
        Number /= 10;
    }
    while (Number > 0);
    sum = odd_sum + even_sum;
    if (sum % 10 == 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

void GetCreditCardName(long Number, int f)
{
    if (f == 0)
    {
        printf("INVALID\n");
    }
    else
    {
        int first_number;
        if (Number > 999999999999999)
        {
            first_number = Number / 1000000000000000;
            if (first_number == 4)
            {
                printf("VISA\n");
            }
            else
            {
                first_number = Number / 100000000000000;
                if (first_number == 51 || first_number == 52 || first_number == 53 ||
                    first_number == 54 || first_number == 55)
                {
                    printf("MASTERCARD\n");
                }
                else
                {
                    printf("INVALID\n");
                }
            }
        }
        else if (Number > 99999999999999 && Number < 1000000000000000)
        {
            first_number = Number / 10000000000000;
            if (first_number == 34 || first_number == 37)
            {
                printf("AMEX\n");
            }
            else
            {
                printf("INVALID\n");
            }
        }
        else if (Number > 999999999999 && Number < 10000000000000)
        {
            first_number = Number / 1000000000000;
            if (first_number == 4)
            {
                printf("VISA\n");
            }
            else
            {
                printf("INVALID\n");
            }
        }
        else
        {
            printf("INVALID\n");
        }
    }
}
