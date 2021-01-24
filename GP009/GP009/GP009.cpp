// GP009.cpp : STL01
//

#include "stdafx.h"
#define SIZE 10 // 정적 배열
int S_arr[SIZE];

int main()
{
    int size = 10;
    vector<int> arr(size); // 크기 10의 벡터 선언
    int* d_arr = new int[size]; // 동적 배열

    for (int i = 0; i < size; i++) // 벡터
    {
        arr[i] = i;
    }

    for (int i = 0; i < size; i++) // 정적 배열
    {
        S_arr[i] = i;
    }

    for (int i = 0; i < size; i++) // 동적 배열
    {
        d_arr[i] = i;
    }

    cout << "벡터 출력" << endl;
    for (int i = 0; i < size; i++) // 벡터 출력
    {
        cout << arr[i] << " ";
    }
    cout << endl;

    cout << "정적 배열 출력" << endl;

    for (int i = 0; i < size; i++) // 정적 배열 출력
    {
        cout << S_arr[i] << " ";
    }
    cout << endl;

    cout << "동적 배열 출력" << endl;

    for (int i = 0; i < size; i++) // 동적 배열 출력
    {
        cout << d_arr[i] << " ";
    }

    delete[] d_arr;

    return 0;
}