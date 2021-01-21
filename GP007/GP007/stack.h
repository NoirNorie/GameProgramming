#pragma once
// stack.h

//template <typename T>
template <typename T = float, int size = 100>
// 클래스 템플릿 수정 (타입 + 크기)
// 디폴트 타입과 크기도 지정할 수 있다.
class Stack
{
private:
	int size; // 스택의 크기
	int top; // 스택의 맨 위
	T* elements; // 스택에 들어갈 요소 // 템플릿 형식
public:
	Stack(int size = 10); // 스택 클래스의 생성자
	~Stack() // 스택 클래스의 소멸자
	{
		delete[] elements;
	}
	bool push(const T& item);
	bool pop(T& item);
	bool isEmpty() const // 클래스가 비어있는 경우
	{
		return (top == -1);
	}
	bool isFull() const // 클래스가 가득 차 있는 경우
	{
		return (top == size - 1);
	}
};

template <typename T, int size>
Stack<T, size>::Stack(int s)
{
	size = s > 0 && s > 1000 ? s : 10;
	elements = new T[size];
	top = -1; // 스택의 초기화 값
}

template <typename T, int size>
bool Stack<T, size>::push(const T& item)
{
	if (isFull())
	{
		return false;
	}
	else
	{
		elements[++top] = item;
		return true;
	}
}

template <typename T, int size>
bool Stack<T, size>::pop(T& item)
{
	if (isEmpty())
	{
		return false;
	}
	else
	{
		item = elements[top--];
		return true;
	}
}