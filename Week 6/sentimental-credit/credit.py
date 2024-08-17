from cs50 import get_string, get_int
import re


visa = re.compile(r'4')
amex = re.compile(r'(37|34)')
mastercard = re.compile(r'(51|52|53|54|55)')


def main():
    number = get_int("Number: ")
    if (check_luhn(number)):
        get_credit_card_name(number)
    else:
        print("INVALID")

def check_luhn(n):
    f = 1
    odd_sum = 0
    even_sum = 0
    sum = 0
    while True:
        if f == 1:
            odd_sum += n % 10
            f = 0
        else:
            even_number = n % 10 * 2
            if (even_number > 9):
                even_sum += even_number % 10 + even_number // 10
            else:
                even_sum += even_number
            f = 1
        n //= 10
        if (not n):
            break

    sum = odd_sum + even_sum

    if (sum % 10 == 0):
        return True
    else:
        return False

def get_credit_card_name(n):
    card_number_str = str(n)
    length_of_card = len(card_number_str)
    if (visa.match(card_number_str) != None and (length_of_card == 13 or length_of_card == 16)):
        print("VISA")
    elif (amex.match(card_number_str) != None and length_of_card == 15):
        print("AMEX")
    elif (mastercard.match(card_number_str) != None and length_of_card == 16):
        print("MASTERCARD")
    else:
        print("INVALID")

main()
