// GP016.cpp : string, wstring

#include <iostream>
#include <string>
using namespace std;

int main()
{
    string s1("abc");
    
    //basic_string<char> s1("abc");
    //char로 인스턴스화

    wstring s2(L"abc");
    
    //basic_string<wchar_t> s2(L"abc");
    //wchar_t로 인스턴스화
    //와이드 문자 타입의 문자열 상수는 접두사 L로 표시해야 한다.

    //한글 처리
    string s3("가나다");
    wstring s4(L"가나다"); //한글 문자열을 다루는 경우 string 대신 wstring을 사용해야 정확하게 나온다.
    cout << s3.length() << ", " << s4.length() << endl;

}