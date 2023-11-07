import sys
import random

from pyfiglet import Figlet
from cs50 import get_string

def main():

    arguments = len(sys.argv)
    figlet = Figlet()

    if arguments == 1:
        random_font = random.choice(figlet.getFonts())
        figlet.setFont(font = random_font)

    elif arguments == 3 and (sys.argv[1] == "-f" or sys.argv[1] == "--font"):
        try:
            figlet.setFont(font = sys.argv[2])
        except:
            print("Invalid usage")
            sys.exit(1)

    else:
        print("Invalid usage")
        sys.exit(1)

    text = get_string("Input: ")
    print("Output: ")
    print(figlet.renderText(text))

if __name__ == "__main__":
    main()
