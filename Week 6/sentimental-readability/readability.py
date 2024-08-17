from cs50 import get_string


def main():
    text = get_string("Text: ")
    letters_number = count_letters(text)
    words_number = count_words(text)
    sentences_number = count_sentences(text)

    grade = round(0.0588 * letters_number / words_number * 100 - \
                  0.296 * sentences_number / words_number * 100 - 15.8)

    if grade < 1:
        print("Before Grade 1")
    elif grade >= 16:
        print("Grade 16+")
    else:
        print(f"Grade {grade}")

def count_letters(text):
    sum = 0
    for i in range(len(text)):
        if text[i].isalpha():
            sum += 1
    return sum

def count_words(text):
    sum = 1
    for i in range(len(text)):
        if text[i].isspace():
            sum += 1
    return sum

def count_sentences(text):
    sum = 0
    for i in range(len(text)):
        if text[i] == "." or text[i] == "!" or text[i] == "?":
            sum += 1
    return sum

main()
