// GP007.cpp : 클래스 템플릿 사용 - 비타입 인자, typedef

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

	cout << endl << endl;
	cout << "typedef 사용" << endl;

	// typedef로 가독성을 높일 수 있음
	typedef Stack<int> INTSTACK;
	typedef Stack<float> FLOATSTACK;
	// 이 부분을 템플릿 구현부로 지정한다.
	// 이후 변경 사항이 존재한다면 이 부분만 변경한다면 아래의 코드는 변경할 필요가 없어진다.


	INTSTACK instack;
	cout << endl;
	cout << "istack에 원소들을 넣습니다." << endl;
	while (instack.push(i))
	{
		cout << i << ' ';
		i += 1;
	}
	cout << endl;
	cout << "istack에서 원소들을 뺍니다." << endl;
	while (instack.pop(i))
	{
		cout << i << ' ';
	}


	FLOATSTACK flstack(5);
	cout << "fstack에 원소 삽입" << endl;
	while (flstack.push(f))
	{
		cout << f << ' ';
		f += 1.1f;
	}
	cout << endl;
	cout << "fstack에서 원소 제거" << endl;
	while (flstack.pop(f))
	{
		cout << f << ' ';
	}

	return 0;
}