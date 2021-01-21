// GP006.cpp : 클래스 템플릿 사용

#include "stdafx.h"
#include "stack.h"

int main()
{
	Stack<float> fstack(5); // 실수형 크기 5짜리 스택 생성
	float f = 1.1F;
	
	cout << "fstack에 원소 삽입" << endl;
	while (fstack.push(f))
	{
		cout << f << ' ';
		f += 1.1f;
	}
	cout << endl;
	cout << "fstack에서 원소 제거" << endl;
	while (fstack.pop(f))
	{
		cout << f << ' ';
	}

	Stack<int> istack; // 정수형 크기 10짜리 (디폴트) 스택 생성
	int i = 1;
	cout << endl;
	cout << "istack에 원소들을 넣습니다." << endl;
	while (istack.push(i))
	{
		cout << i << ' ';
		i += 1;
	}
	cout << endl;
	cout << "istack에서 원소들을 뺍니다." << endl;
	while (istack.pop(i))
	{
		cout << i << ' ';
	}
}