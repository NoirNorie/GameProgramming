// GP005.cpp : 함수 템플릿

#include "stdafx.h"

template <typename T>
T maximum(T a, T b)
{
    return (a > b ? a : b);
}

int main()
{
    cout << "maximum(5, 3) = " << maximum(5, 3) << endl;
    cout << "maximum('x', 'y') = " << maximum('x', 'y') << endl;
    cout << "maximum(3.3, 5.5) = " << maximum(3.3, 5.5) << endl;

    return 0;
}