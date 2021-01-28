// GP013.cpp : map

#include <iostream>
#include <map>
using namespace std;

struct MyStringCompare
{
	bool operator()(const char* s1, const char* s2) const
	{
		return strcmp(s1, s2) < 0;
	}
};

int main()
{
	map<const char*, int> m1; // map 생성
	m1["january"] = 31;
	m1["february"] = 28;
	m1["april"] = 30;

	cout << "february" << " = " << m1["february"] << endl;

	cout << "march" << " = " << m1["march"] << endl;
	// 연관된 값이 없는 경우 0이 출력됨

	map<const char*, int>::iterator iter = m1.find("january");
	// map을 따라갈 반복자
	cout << iter->first << " = " << iter->second << endl;

	map<const char*, int, MyStringCompare> m2; // 오름차순으로 정렬할 map 생성
	m2["january"] = 31;
	m2["february"] = 28;
	m2["april"] = 30;


	cout << endl;
	cout << '\t' << "multimap 사용" << endl;
	cout << endl;

	multimap<int, const char*> mm; // multimap
	mm.insert(pair<int, const char* const>(2, "two"));
	mm.insert(pair<int, const char* const>(3, "three"));
	mm.insert(pair<int, const char* const>(2, "zwei"));

	cout << '\t' << "#elements with key 2 = " << mm.count(2) << endl;
	// count() : 해당 값이 map에 몇개 있는지 카운트하는 함수

	multimap<int, const char*>::iterator mm_iter = mm.find(2);
	cout << '\t' << "the first element with key 2 = "
		<< mm_iter->second << endl;
	// zwei를 찾을 경우 mm_iter++를 해주면 된다.

}