// GP017.cpp : 반복자와 반복자 어댑터

#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main()
{
    vector<int> v1(10); //크기 10짜리 vector 생성
    for (int i = 0; i < v1.size(); i++)
    {
        v1[i] = i;
    }

    vector<int>::iterator iter; //vector에 사용할 반복자
    for (iter = v1.begin(); iter != v1.end(); iter++)
    {
        cout << *iter << " ";
    }
    cout << endl;

    vector<int> v2(10);
    for (int i = 0; i < v1.size(); i++)
    {
        v2[i] = i;
    }

    vector<int>::reverse_iterator riter; //vector를 역방향으로 순회할 반복자
    for (riter = v1.rbegin(); riter != v1.rend(); riter++)
    {
        cout << *riter << " ";
    }
    // rbegin은 첫번째 원소의 앞 위치를 가리킨다 (실제로 존재하지 않는 위치)
    // rend는 마지막 원소를 가리킨다.
    cout << endl;

    sort(v1.rbegin(), v1.rend()); //역방향 반복자이므로 내림차순으로 정렬된다.

    for (iter = v1.begin(); iter != v1.end(); iter++)
    {
        cout << *iter << " ";
    }
    cout << endl;

}