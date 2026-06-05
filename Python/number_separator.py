# Write a function that takes a list of non-negative integers and returns a dictionary with three keys:
# "even_factorials" — a list of factorials of all even numbers in the input list
# "odd_numbers" — a list of all odd numbers from the input list
# "invalid" — a count of any negative numbers found (ignore them in the other lists)

import math
user = [1,2,3,4,5,6,7,8,9,10]

def even(nums):
    evens = []
    for num in nums:
        if num % 2 == 0:
            evens.append(num)
    return evens

def odd(nums):
    odd = []
    for num in nums:
        if num % 2 == 1:
            odd.append(num)
    return odd

def even_factorial(nums):
    evens = even(user)
    even_fact = []
    for i in evens:
        even_fact.append(math.factorial(i))
    return even_fact

def invalid(user):
    invalid = []
    for i in user:
        invalid.append(math.factorial(i))
    return invalid

def main():
    print(f"EVEN : {even(user)}")
    print(f"ODD : {odd(user)}")
    print(f"EVEN FACTORIAL : {even_factorial(user)}")
    print(f"INVALID : {invalid(user)}")
          
if __name__ == '__main__':
    main()
