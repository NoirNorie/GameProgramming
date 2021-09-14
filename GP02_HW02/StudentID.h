#pragma once
#include "stdafx.h"

/*
	학생의 정보와 관련된 Header 파일
*/

// 학생 정보 구조체
struct student
{
	int score;
	std::string name;
};

// vector의 정렬을 위한 커스텀 비교 함수
bool compare(student a, student b);

// 학생의 정보를 만들기 위한 함수
std::string rtnName(); //이름 생성 함수
int rtnScore(); //점수 생성 함수