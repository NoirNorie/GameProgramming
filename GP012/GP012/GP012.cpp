// GP012.cpp : list
//

#include <iostream>
#include <list>
using namespace std;

int main()
{
    list<int> c1; // 리스트 생성
    c1.push_back(2);
    c1.push_back(1);
    c1.push_back(3);

    list<int>::iterator iter; // 리스트에 접근할 반복자
    iter = c1.begin();
    iter++;
    c1.insert(iter, 8); // 두번째에 값 8 삽입

    list<int> c2;
    c2.push_back(4);
    c2.push_back(5);
    c2.push_back(6);
    
    c1.insert(++c1.begin(), c2.begin(), --c2.end());

    // insert 함수는 3가지 형태를 가지고 있다.
    //insert(iterator position, const value_type & val);
    //일반적으로 특정 위치에 값을 삽입하는 형태

    //insert(iterator position, size_type n, const value_type & val);
    //특정 위치에 값을 n번 반복해 삽입하는 형태
    
    //insert(iterator position, iterator_type first, iterator_type last);
    //특정 위치에 first ~ last - 1 범위의 값을 삽입하는 경우
    //리스트와 리스트를 합치는 경우 유용할 것 같음
}