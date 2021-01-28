// GP014.cpp : set과 multiset

#include <iostream>
#include <algorithm>
#include <set>
using namespace std;

int main()
{
    set<int> s1;
    //디폴트로 less<TYPE>이 적용되어 오름차순으로 정렬된다.
    int A[8] = { 4,2,1,2,1,2,4,1 };
    
    for (int i = 0; i < 8; i++)
    {
        s1.insert(A[i]);
        //set에 중복된 값을 삽입하는 경우 무시된다.
    }

    set<int, greater<int>> s2;
    //greater<TYPE> 적용 시 내림차순으로 정렬된다.
    int B[5] = { 5,2,3,3,1 };
    
    for (int i = 0; i < 5; i++)
    {
        s2.insert(B[i]);
    }

    multiset<int, less<int>> ms1;
    for (int i = 0; i < 8; i++)
    {
        ms1.insert(A[i]);
    }
    ms1.insert(ms1.begin(), 3);//값을 추가, 자동으로 정렬된다.
    ms1.erase(2); //해당되는 값 모두 제거

    //multiset은 중복 객체들을 허용한다는 점 외에 set과 동일하다.

    //set<int> s3;
    //set_union(s1.begin(), s1.end(), s2.begin(), s2.end(), inserter(s3, s3.begin()) );
    ////set으로 합집합 연산을 수행한다.

    //set<int> s4;
    //set_intersection(s1.begin(), s1.end(), s2.begin(), s2.end(), inserter(s4, s4.begin()) );
    ////set으로 교집합 연산을 수행한다.

}