// GP010.cpp : vector

#include "stdafx.h"

class compObj
{
public:
    bool operator()(int x, int y)
    {
        return x > y;
    }
};

bool comp(int x, int y);

int main()
{
    vector<int> v0; // 빈 vector 생성
    vector<int> v1(3); // 크기가 3인 vector 생성, 내용은 디폴트 값인 0으로 초기화
    vector<int> v2(5, 2); // 크기가 5인 vector 생성, 내용은 2로 초기화
    vector<int> v4(v2); // v2의 복제본 vector를 생성
    vector<int> v5(v4.begin() + 1, v4.begin() + 3); // v4의 부분 복제 vector를 생성

    vector<int> a1;
    //vector<int, allocator<int>> a1;
    // 같은 선언 문장이다.
    // allocator는 vector의 메모리 할당과 반환에 대한 구체적인 사항들이 담겨있다.
    // vector<TYPE, allocator<TYPE>> 형태


    // iterator를 통한 vector의 출력
    vector<int>::iterator iter;
    cout << "v2 = ";
    for (iter = v2.begin(); iter != v2.end(); iter++)
    {
        cout << " " << *iter;
    }
    cout << endl;

    // push_back 함수
    a1.push_back(1);
    int i = (int)a1.size();
    cout << "벡터의 길이 = " << i << endl;
    a1.push_back(2);
    i = (int)a1.size();
    cout << "벡터의 길이 = " << i << endl;

    a1.push_back(8);
    cout << "마지막 원소 : " << a1.back() << endl;
    cout << "첫번째 원소 : " << a1.front() << endl;

    a1.push_back(9);
    int& i2 = a1[1];
    cout << "두번째 원소 : " << i2 << endl;
    int& i3 = a1.at(2);
    cout << "세번째 원소 : " << i3 << endl;
    int& i4 = a1.at(3);
    cout << "마지막 원소 : " << i4 << endl;

    //pop_back 함수
    a1.pop_back();
    cout << "마지막 원소 : " << a1.back() << endl;

    //pointer 함수
    vector<int> v;
    v.push_back(1);
    v.push_back(2);
    vector<int>::pointer ptr = &v[0];
    cout << *ptr << endl;
    ptr++; // 포인터 이동
    cout << *ptr << endl;

    *ptr = 4; // 포인터로 값 수정
    cout << *ptr << endl;

    vector<int> sort_vector;
    for (int i = 0; i < 8; i++)
    {
        sort_vector.push_back(rand()); // 랜덤 값을 vector에 8개 삽입
    }

    sort(sort_vector.begin(), sort_vector.end());
    // #include<algorithm>을 통해 STL의 알고리즘을 읽어옴

    for (vector<int>::iterator iter = sort_vector.begin(); iter != sort_vector.end(); iter++)
    {
        cout << *iter << " ";
    }
    cout << endl;


    // 사용자 정의 비교 함수 사용
    sort(sort_vector.begin(), sort_vector.end(), comp);
    // 정의에서 오름차순으로 정렬되도록 만들었으므로 오름차순으로 정렬됨

    // 사용자 정의 클래스 사용
    sort(sort_vector.begin(), sort_vector.end(), compObj());
    // 이건 내림차순

}

bool comp(int x, int y)
{
    return x < y;
}