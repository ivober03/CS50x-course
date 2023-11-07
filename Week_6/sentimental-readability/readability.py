from cs50 import get_string


# This program takes a text and determinates its reading level.
def main():

    # promt the user for a Text
    text = get_string("Text: ")

    letters = count_letters(text)
    words = count_words(text)
    sentences = count_sentences(text)

    # calculate the reading level
    l = letters / words * 100
    s = sentences / words * 100
    index = 0.0588 * l - 0.29 * s - 15.8
    index = round(index)

    # print the reading level
    print_grade(index)


def count_letters(s):
    count = 0

    # loop through the string and count the letters
    for char in s:
        if char.isalpha():
            count += 1
    return count

def count_words(s):
    count = 1

    # loop through the string and count the words
    for char in s:
        if char == " ":
            count += 1
    return count


def count_sentences(s):
    count = 0

    # loop through the string and count the sentences
    for char in s:
        if char == "." or char == "!" or char == "?":
            count += 1
    return count


def print_grade(i):

    if i >= 16:
        print(f"Grade 16+")
    elif i < 1:
        print(f"Before Grade 1")
    else:
        print(f"Grade {i}")


main()