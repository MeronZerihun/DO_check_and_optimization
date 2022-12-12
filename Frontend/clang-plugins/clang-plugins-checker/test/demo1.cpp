#include <stdio.h>

// A function to implement Bubble Sort
void bubbleSort(int arr1[], int n)
{
    int i, j;
    for (i = 0; i < n - 1; i++)
        for (j = 0; j < n - i - 1; j++)
            if (arr1[j] > arr1[j + 1])
            {
                int tmp = arr1[j];
                arr1[j] = arr1[j + 1];
                arr1[j + 1] = tmp;
            }
}

// Function to print an array
void printArray(int arr[], int size)
{
    int i;
    for (i = 0; i < size; i++)
        printf("%d ", arr[i]);
    printf("\n");
}

int main()
{
    int my_array[] = {5, 1, 4, 2, 8};
    int n = sizeof(my_array) / sizeof(my_array[0]);
    bubbleSort(my_array, n);
    printf("Sorted array: \n");
    printArray(my_array, n);
    return 0;
}