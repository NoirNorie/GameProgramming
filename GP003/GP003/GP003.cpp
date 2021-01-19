#include "stdafx.h"
#include <stdlib.h>
#include <time.h>

void print_array(const char* str, const int* list, int n); // list print Function
void quickSort(int* list, int ilo, int ihi); // quickSort Function

int main()
{
    int n = 5;
    int* list = (int*)malloc(sizeof(int) * n); // memory allocate
    for (int i = 0; i < n; i++)
    {
        list[i] = rand();
    }
    print_array("before sorting", list, n);

    quickSort(list, 0, n - 1);

    print_array("after sorting", list, n);

}

void print_array(const char* str, const int* list, int n)
{
    cout << str << endl;
    
    for (int i = 0; i < n; i++)
    {
        cout << list[i] << " ";
    }

    cout << endl;
}

void quickSort(int* list, int ilo, int ihi)
{
    if (ilo >= ihi) return;
    int pivot = list[(ilo + ihi) / 2];
    int ieq = ilo;
    int ulo = ilo;
    int uhi = ihi;

    while (ulo <= uhi)
    {
        if (list[uhi] > pivot)
        {
            uhi--;
        }
        else
        {
            int tempEntry = list[ulo];
            list[ulo] = list[uhi];
            list[uhi] = tempEntry;
            if (list[ulo] < tempEntry)
            {
                tempEntry = list[ieq];
                list[ieq] = list[ulo];
                list[ulo] = tempEntry;
                ieq++;
            }
            ulo++;
        }
    }
    quickSort(list, ilo, ieq - 1);
    quickSort(list, uhi + 1, ihi);
}