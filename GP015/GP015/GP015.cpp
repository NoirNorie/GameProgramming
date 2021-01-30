// GP015.cpp : string / 문자열

#include <iostream>
#include <string>
#include <algorithm>
using namespace std;

int main()
{
	string s;
	const char* my_str = "My friend";
	s += my_str; //공백 문자열에 += 연산으로 문자열을 추가

	string t("your friend");

	const char* t_cstyle = t.c_str();
	cout << "\t c-style = " << t_cstyle << ", " << strlen(t_cstyle) << endl;

	printf("\t [%s] %s [%s]\n", s.c_str(), (s < t ? "<" : ">="), t.c_str());
	//문자열의 길이 비교 연산

	t = s; //대입 연산

	if (t == "your friend")
	{
		cout << "\t string matched" << endl;
	}
	else
	{
		cout << "\t string mismatched" << endl;
	}

	cout << endl;
	cout << "문자열 찾기" << endl;
	//문자열 찾기
	int num = 0;
	string srch = "ABC ABB BABBA AAAA ABC AABBAA ABBBA";

	for (int i = srch.find("ABC", 0); i != string::npos;i = srch.find("ABC", i))
	{
		num++;
		i++; // 일치하는 경우 찾은 후에 그 경우를 다시 찾지 않기 위해 i++
	}
	//string::npos : 일치하는 문자열이 없는 경우 리턴된다.

	cout << "ABC appearances = " << num << endl;

	cout << endl;
	cout << "부분 문자열" << endl;

	//부분 문자열
	string my_string = "My friend";
	string first_ten_of_alphabet = my_string.substr(3, 4);
	//substr() : 첫 인자에서 시작해서 두번째 인자값 길이 만큼의 부분 문자열을 새로 생성해 리턴
	printf("substr(3, 4) is [%s]\n", first_ten_of_alphabet.c_str());

	cout << endl;
	cout << "문자열과 알고리즘" << endl;

	//문자열과 알고리즘
	reverse(s.begin(), s.end());
	printf("\t reverse = [%s]\n", s.c_str());

	sort(t.begin(), t.end());
	printf("\t sort = [%s]\n", t.c_str());


}