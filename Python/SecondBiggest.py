from typing import List

def print_array(numbers: List[int]) -> None:
    for i in numbers:
        if i == numbers[-1]:
            print(f"{i}", end = '')
        else:
            print(f"{i}, ", end = '')

def get_second_highest(array: int) -> int:
    highest = 0
    secondHighest = 0

    print()
    for i in range(0, len(array)):
        if highest < array[i]:
            secondHighest = highest
            highest = array[i]
        elif secondHighest < array[i]:
            secondHighest = array[i]
    return secondHighest

def main():
    numbers = [2, 43, 46, 23, 64, 13, 53, 32, 42, 44]

    print_array(numbers)
    print(f"The Second Largest Integer is {get_second_highest(numbers)}")

if __name__ == '__main__':
    main()

