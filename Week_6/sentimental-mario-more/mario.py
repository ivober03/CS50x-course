from cs50 import get_int


def main():
    height = 0

    while height > 8 or height < 1:
        height = get_int("Height: ")

    # loop to print pyramid
    for x in range(height):
        # loop to print spaces
        for y in range(height - (x + 1)):
            print(" ", end='')
        # loop to print hashes
        for z in range(x + 1):
            print("#", end='')
        # print two spaces
        print("  ", end='')
        # loop to print other side of pyramid
        for w in range(x + 1):
            print("#", end='')
        print("")

main()