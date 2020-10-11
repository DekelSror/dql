#include <string.h>
#include <stdio.h>

const int values[] = {1, 4, 5, 0, 4};
const size_t values_length = sizeof(values) / sizeof(values[0]);

int result[2] = {0};

void BestSellPoint(void)
{
    int highest_delta = 0;
    size_t low = 0;
    size_t high = 0;

    while (low < values_length)
    {
        if (low == high) high = low + 1;
        if (high == values_length) high = values_length -1;

        int delta = values[high] - values[low];

        if (delta > highest_delta)
        {
            highest_delta = delta;

            result[0] = values[low];
            result[1] = values[high];

            ++high;
        }
        else ++low;
   }
}

char permutations_test_string[] = "abc";
size_t len_permutations_test_string = 4;

void SwapChars(char* str, size_t i1, size_t i2)
{
    char tmp = str[i1];
    str[i1] = str[i2];
    str[i2] = tmp;
}

void AllPermutations(char* perm)
{
    if (0 == *perm) return;


    for (size_t i = 0; i < strlen(perm) - 1; i++)
    {
        printf("%s\n", perm);
        // AllPermutations(perm + 1);
        SwapChars(perm, i, i + 1);
        printf("%s\n", perm);
        // AllPermutations(perm + 1);
    }
}

int main(void)
{
    // BestSellPoint();

    // printf("got %d - %d = %d\n", result[1], result[0], result[1] - result[0]);

    AllPermutations(permutations_test_string);

    return 0;
}
