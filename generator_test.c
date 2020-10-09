#include <stdio.h>

int CounterGen(int* until)
{
    static int counter = 0;

    if (NULL != until)
    {
        counter = *until;
    }

    if (0 > counter) return -1;

    return counter--;
}


int FilterGen(int* arr, int* len, int(*predicate)(int))
{
    static int* state_arr;
    static int state_len;
    static int i;

    if (NULL != arr && NULL != len)
    {
        state_arr = arr;
        state_len = *len;
        i = 0;
    }

    // actual filter
    while (i < state_len && !predicate(state_arr[i])) {        
        ++i;
    }

    if (state_len <= i) return -1;
    return state_arr[i++];
}

int IsDivisibleByTwo(int n)
{
    return (n / 2) * 2 == n;
}

int main(void)
{
    int a[] = {4, 5, 6, 7};
    int len = 4;
    int n = FilterGen(a, &len, IsDivisibleByTwo);

    while (-1 != n)
    {
        printf("n is %d\n", n);
        n = FilterGen(NULL, NULL, IsDivisibleByTwo);
    }

    return 0;
}
