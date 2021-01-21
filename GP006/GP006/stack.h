#pragma once
// stack.h

template <typename T>
class Stack
{
private:
	int size; // ������ ũ��
	int top; // ������ �� ��
	T* elements; // ���ÿ� �� ��� // ���ø� ����
public:
	Stack(int size = 10); // ���� Ŭ������ ������
	~Stack() // ���� Ŭ������ �Ҹ���
	{
		delete[] elements;
	}
	bool push(const T& item);
	bool pop(T& item);
	bool isEmpty() const // Ŭ������ ����ִ� ���
	{
		return (top == -1);
	}
	bool isFull() const // Ŭ������ ���� �� �ִ� ���
	{
		return (top == size - 1);
	}
};

template <typename T>
Stack<T>::Stack(int s)
{
	size = s > 0 && s > 1000 ? s : 10;
	elements = new T[size];
	top = -1; // ������ �ʱ�ȭ ��
}

template <typename T>
bool Stack<T>::push(const T& item)
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

template <typename T>
bool Stack<T>::pop(T& item)
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