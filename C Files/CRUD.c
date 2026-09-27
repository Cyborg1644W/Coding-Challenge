//CRUD - Create, Read, Update, Delete

#include <stdio.h>
#include <stdlib.h>
int array[100];
int i, l, position, value, arraySize, temp;

void create();
void display();
void insert();
void del();
void asc();
void srch();

int main()
{
    int choice;
    system("cls");
    printf("\n\t MENU \n");
    printf("\nPress 1: Create an Array");
    printf("\nPress 2: Display Elements");
    printf("\nPress 3: Insert an Element");
    printf("\nPress 4: Delete an Element");
    printf("\nPress 5: Sort Array Elements");
    printf("\nPress 6: Search Array Element");
    printf("\nPress 7: Exit");
    printf("\n\tPlease Enter your choice: ");
    scanf("%i", &choice);
    switch(choice)
    {
        case 1: create(); break;
        case 2: display(); break;
        case 3: insert(); break;
        case 4: del(); break;
        case 5: asc(); break;
        case 6: srch(); break;
        case 7: exit(0); break;
        default: printf("\nInvalid choice, please try again!"); break;
    }
}

void create()
{
    system("cls");
    printf("\nInput an integer to define array size: ");
    scanf("%i", &arraySize);

    printf("\nInput %i integers in the array: ", arraySize);
    for(i=0; i<arraySize; i++)
    {
        scanf("%i", &array[i]);
    }
    display();
    main();
}

void display()
{
    system("cls");
    if(arraySize <= 0)
    {
        printf("\nThe array is Empty!\n\n");
        system("pause");
        create();
    }
    else
    {
        printf("\n The elements are: \n");
        for(i=0; i<arraySize; i++)
        {
            printf("[%i] ", array[i]);
        }
        printf("\n");
    }
    system("pause");
    main();
}

void insert()
{
    system("cls");
    if(arraySize <= 0)
    {
        printf("\nThe array is Empty!\n\n");
        system("pause");
        create();
    }
    else
    {
        printf("\nEnter the position (index) of the element to be inserted: ");
        scanf("%i", &position);

        if(position < 0 || position > arraySize)
        {
            printf("\nInvalid position! You can only insert between index 0 and %i.\n", arraySize);
            system("pause");
            insert();
        }

        printf("\nEnter the element/value to be inserted: ");
        scanf("%i", &value);

        for(i=arraySize-1; i>=position; i--)
        {
            array[i + 1] = array[i];
        }
        array[position] = value;
        arraySize += 1; // arraySize = arraySize + 1;
    }
    display();
    system("pause");
    main();
}

void del()
{
    system("cls");
    if(arraySize <= 0)
    {
        printf("\nThe array is Empty!\n\n");
        system("pause");
        create();
    }

    else
    {
        int inputValue, foundIndeces[100], foundCount = 0, valid = 0;

        printf("\nEnter the element/value to be deleted: ");
        scanf("%i", &inputValue);

        for(i=0; i<arraySize; i++)
        {
            if(array[i] == inputValue)
            {
                foundIndeces[foundCount] = i;
                foundCount++;
            }
        }

        if(foundCount == 0)
        {   
            printf("\nThe element %i was not found in the array.\n", inputValue);
        }
        else if(foundCount == 1)
        {
            position = foundIndeces[0];
            value = array[position];
            for(i = position; i<arraySize - 1; i++)
            {
                array[i] = array[i + 1];
            }
            arraySize -= 1;
            printf("\nThe deleted element is: %i \n", value);
        }
        else
        {
            printf("\nThe element %i is found at the following indeces: ", inputValue);
            for(i=0; i < foundCount; i++)
            {
                printf("[%i] ", foundIndeces[i]);
            }

            printf("\n\nEnter the index of the element to be deleted: ");
            scanf("%i", &position);

            for(i = 0; i<foundCount; i++)
            {
                if(position == foundIndeces[i])
                {
                    valid = 1;
                    break;
                }
            }

            if(valid)
            {
                value = array[position];
                for(i=position; i<arraySize - 1; i++)
                {
                    array[i] = array[i + 1];
                }
                arraySize -= 1;
                printf("\nThe deleted element is %i. \n", value);
            }
            else
            {
                printf("\nInvalid index selection. No element deleted.\n");
            }
        }
        system("pause");
        printf("\nThe new elements are: \n");
        display();
    }
    system("pause");
    main();
}

void asc()
{
    system("cls");
    if(arraySize <= 0)
    {
        printf("\nThe array is Empty!\n\n");
        system("pause");
        create();
    }
    else
    {
        for(i=0; i<=arraySize; i++)
        {
            for(l=0; l<arraySize-1; l++)
            {
                if(array[l] > array[l+1])
                {
                    temp = array[l];
                    array[l] = array[l + 1];
                    array[l + 1] = temp;
                }
            }
        }
        printf("\nSorted Array Elements in Ascending Order: \n");
        display();
    }
    system("pause");
    main();
}

void srch()
{
    int input, elementFound = 0;
    system("cls");
    if(arraySize <= 0)
    {
        printf("\nThe array is Empty!\n\n");
        system("pause");
        create();
    }

    else
    {
        printf("\nPlease input an integer to be searched: ");
        scanf("%i", &input);
        for(i=0; i<arraySize; i++)
        {
            if(input == array[i])
            {
                printf("\nThe integer %i is found at index %i", input, i);
                elementFound = 1;
            }
        }

        if(elementFound)
        {
            printf("\nInteger Successfully Found!\n");
        }
        else
        {
            printf("\nThe integer %i is not in the array. \n", input);
            srch();
        }
    }
    system("pause");
    main();
}
































































