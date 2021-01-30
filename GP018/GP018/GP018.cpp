// GP018.cpp : 컨테이너 어댑터

#include <iostream>
#include <vector>
#include <deque>
#include <stack>
#include <queue>

using namespace std;

int main()
{
    //stack
    stack<int, deque<int>> stack;
    for (int i = 0; i < 10; i++)
    { 
        stack.push(i); //stack의 push : 스택의 맨 위에 원소 추가
    }

    while (stack.size())
    {
        cout << stack.top() << " "; //stack의 top : 스택의 맨 위 원소 리턴
        stack.pop(); //stack의 pop : 스택의 맨 위 원소 삭제
    }
    cout << endl << endl;

    //queue
    queue<int, deque<int>> queue;
    for (int i = 0; i < 10; i++)
    {
        queue.push(i); //queue의 push : 큐의 맨 뒤에 원소 추가
    }

    while (queue.size())
    {
        cout << queue.front() << " ";   // queue의 front : 큐의 맨 앞 원소 리턴
                                        // queue의 back : 큐의 맨 뒤 원소 리턴
        queue.pop(); // queue의 pop : 큐의 맨 앞 원소 제거
    }
    cout << endl << endl;

    //priority_queue
    //기본적으로 queue와 비슷함
    priority_queue<int, vector<int>, less<int>> pqueue;
    for (int i = 0; i < 10; i++)
    {
        pqueue.push(rand());
    }

    while (pqueue.size())
    {
        cout << pqueue.top() << " ";
        pqueue.pop();
    }
    cout << endl << endl;
}