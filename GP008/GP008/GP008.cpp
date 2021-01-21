// GP008.cpp : 템플릿의 인스턴스화.

#include <iostream>
#include "stack.h"
using namespace std;

int main()
{
    Stack<int> istack;
    istack.ping();
    Stack<float> fstack;
    fstack.pong();

    // 각각 안쓰는 istack.pong()과 fstack.ping()의 정의는 사용하지 않는다.

    return 0;
}