#include <stdlib.h>
#include <stdio.h>
#include <math.h>


int sumArr(int* arr, int arrSize) 
{
    if (NULL == arr)  return 0;
    
    int sum = 0;
    for (int* elem = arr; elem < arr + arrSize; ++elem)
    {
        sum += *elem;
    }
    
    return sum;
    
}

double findMedianSortedArrays(int* nums1, int nums1Size, int* nums2, int nums2Size)
{
    if (NULL == nums1 || NULL == nums2) return NAN;
    if (nums1Size == 0 && nums2Size == 0) return 0;
    
    
    double both =  (double)(sumArr(nums1, nums1Size) + sumArr(nums2, nums2Size));
    return both / (double)(nums1Size + nums2Size);
}

int main(int argc, char const *argv[])
{
    int ns1[] = {1};
    int ns2[] = {3, 4};


    printf("it is %lf\n", findMedianSortedArrays(ns1, 1, ns2, 2));

    
    return 0;
}
