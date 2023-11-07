def main():

    greeting = input("Greeting: ")
    output = 0

    greeting = greeting.lower()
    if "hello" in greeting.strip():
        output = 0
    elif "h" == greeting[0]:
        output = 20
    else:
        output = 100

    print(f"${output}")

if __name__ == "__main__":
    main()