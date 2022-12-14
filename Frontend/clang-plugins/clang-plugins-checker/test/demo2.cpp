#include <stdio.h>
#include "lib/enc_lib.h"

using namespace enc_lib;

// A function to implement Bubble Sort
void bubbleSort(enc_int arr1[], int n)
{
    enc_int i, j;
    for (i = 0; i < n - 1; i++)
    {
        for (j = 0; j < n - i - 1; j++)
        {
            if (arr1[j] > arr1[j + 1])
            {
                int tmp = arr1[j];
                arr1[j] = arr1[j + 1];
                arr1[j + 1] = tmp;
            }
        }
    }
}

// Function to print an array
void printArray(int arr[], int size)
{
    int x;
    for (x = 0; x < size; x++)
        printf("%d ", arr[x]);
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