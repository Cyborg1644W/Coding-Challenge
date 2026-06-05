import math
import random

# Palindrome Checker
def palindrome_checker(user):
    text = user
    reversed_text = user[::-1]
    if text == reversed_text:
        print(f"{text} is a palindrome")
    else:
        print(f"{text} is not a palindrome")

# Fibonacci Sequence Generator
def Fibo_generator(len):
    a, b = 0, 1
    for i in range(len):
        print(a, end = ' ')
        a , b = b, b + a
    print()

# Bank Account Class
class bank:
    number_of_users = 0
    def __init__(self,owner,balance=0): 
        self.name =  owner
        self.balance = balance
        bank.number_of_users += 1
        
    def withdraw(self, amount):
        if amount < self.balance:
            self.balance -= amount
            print(f"{self.name} Withdrew Php {amount:,.2f}. The New Balance is Php {self.balance:,.2f}")
        else:
            print("Ivalid Amount")
            
    def deposit(self, amount):
        if amount > 0:
            self.balance += amount
            print(f"{self.name} Deposited Php {amount:,.2f}. The New Balance is Php {self.balance:.2f}")
        else:
            print("Ivalid Amount")
            
    def get_balance(self):
        return self.balance 
    
# Word Frequency Counter
def word_counter(text):
   words = text.split()
   return len(words)

# Roman Numeral Converter
def roman_convert(number):
    values = [1000, 900, 500, 400,
             100, 90, 50, 40,
             10, 9, 5, 4, 1]
    symbols = ['M', 'CM', 'D', 'CD',
               'C', 'XC', 'L', 'XL',
               'X', 'IX', 'V', 'IV', 'I']
    roman = ""
    for i in range(len(values)):
        while number >= values[i]:
            roman += symbols[i]
            number -= values[i]
    return roman

# Prime Number Checker
def prime_checker(number):
    whole_sqrt = int(math.sqrt(number))
    for i in range(2,whole_sqrt):
        if number % i == 0:
            return False
    return True
        
# Count Vowels in a String
def count_vowel(text):
    vowels = ['a', 'A' , 'e' , 'E' ,'i' , 'I' , 'o' , 'O' , 'u' , 'U']
    counter = 0
    for letter in text:
        if letter in vowels:
            counter +=1 
    return counter

def  riddle():
    rand_num = random.randint(5,26)
    second_random = random.randint(11,49)
    way_1 = ((rand_num)*2)-second_random
    way_2 = ((rand_num)-second_random)* 2
    way_3 = ((rand_num)-second_random)* 3
    way = random.choices([way_1,way_2,way_3])
    if way == way_1:
        user = int(input(f"What number, when doubled and then subtracted by {second_random}, gives {way_1}"))
        if user == rand_num:
            print("CORRECT")
        else:
            print(f"{rand_num} is the correct answer")
    elif way == way_2:
        user = int(input(f"What number when {second_random} is subtracted and the result is doubled, gives {way_2}"))
        if user == rand_num:
            print("CORRECT")
        else:
            print(f"{rand_num} is the correct answer")
    elif way == way_3:
        user = int(input(f"What number when {second_random} is subtracted and the result is tripled, gives {way_3}"))
        if user == rand_num:
            print("CORRECT")
        else:
            print(f"{rand_num} is the correct answer")
            
def main():
    palindrome_checker("Check") #Palindrome
    Fibo_generator(10)  #Fibonacci 
    
    #Bank Functions
    reindel = bank("Reindel",10000)
    james = bank("James",50000)
    godfrey = bank("Godfrey",64000)
    reindel.withdraw(200)
    print(word_counter("The cow jump over the moon")) #word counter
    print(roman_convert(516)) #roman numeral converter
    print(prime_checker(1125))
    print(count_vowel("The cow jumpo over the moon"))
    print(riddle())
    
if __name__ == '__main__':
    main()
