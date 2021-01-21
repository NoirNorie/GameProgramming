#pragma once
// stack.h
#include <iostream>
using namespace std;

template <typename T>
class Stack
{
public:
	Stack() {}
	~Stack() {}
	void ping()
	{
		cout << "pong" << endl;
	}
	void pong()
	{
		cout << "ping" << endl;
	}
};