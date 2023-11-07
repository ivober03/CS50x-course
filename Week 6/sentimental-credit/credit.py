# This program verify if a credit card is an American Express, MasterCard, Visa or none of them
from cs50 import get_int


def main():

    # promt the user for the credit card number
    number = credit_number()
    # print the type of the credit card
    check_type(number)


def credit_number():
    n = -1
    while n < 0:
        n = get_int("Number: ")
    return n


# checks if the checksum is valid
def checksum(n):
    return (Luhn_sum(n) % 10) == 0


# checks if the credit card is valid and print his type
def check_type(n):
    type = "INVALID"
    f2d = first2(n)
    f1d = round(f2d/10)

    if checksum(n):
        if digits(n) == 15 and (f2d == 34 or f2d == 37):
            type = "AMEX"
        elif (digits(n) == 13 or digits(n) == 16) and f1d == 4:
            type = "VISA"
        elif digits(n) == 16 and (f2d == 51 or f2d == 52 or f2d == 53 or f2d == 54 or f2d == 55):
            type = "MASTERCARD"

    print(f"{type}")


# return the digits of a number
def digits(n):
    n = str(n)
    return len(n)


# calculate the Luhn's sum
def Luhn_sum(n):
    # convert the credit card number to a list of digits
    def digits_of(e):
        return [int(d) for d in str(e)]
    digits = digits_of(n)

    # generate a list of odd and even digits starting from the rightmost or last digit
    odd_digits = digits[-1::-2]
    even_digits = digits[-2::-2]

    # multiply each value from the even_digits by 2. If any number is greater than 9, sum its digits to convert it into a single-digit number
    checksum = 0
    for d in even_digits:
        checksum += sum(digits_of(d*2))

    # sum all the odd numbers along with this checksum
    checksum += sum(odd_digits)

    return checksum


# return the first two digits of a number
def first2(n):
    n = str(n)
    fd = n[:2]
    return int(fd)


main()