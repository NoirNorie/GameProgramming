// GP011.cpp : deque
//

#include <iostream>
#include <deque>
using namespace std;

struct MyStruct
{
    int myInt;
};
deque<MyStruct> dq;

int main()
{
    MyStruct m;
    //deque에서 값 삽입
    m.myInt = 4; dq.push_front(m);
    m.myInt = 3; dq.push_back(m);
    m.myInt = 5; dq.push_front(m);

    deque<MyStruct>::iterator iter; // 출력용 반복자
    for (iter = dq.begin(); iter != dq.end(); iter++)
    {
        cout << iter->myInt << " ";
    }
    cout << endl;

    //deque에서 값 제거
    dq.pop_front();
    dq.pop_back();

    for (iter = dq.begin(); iter != dq.end(); iter++)
    {
        cout << iter->myInt << " ";
    }
    cout << endl;

    return 0;
}