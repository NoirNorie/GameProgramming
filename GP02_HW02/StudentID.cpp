#include "StudentID.h"

/*
	학생의 정보와 관련된 CPP 파일
*/

// --------------- 선언부 --------------------
// 전역 변수
//std::vector<student> vec; // 학생의 정보를 저장할 vector

// 학생의 이름을 만들때 사용할 배열
std::string familyName[12] =
{
	"김", "이", "박", "최",
	"남", "정", "한", "류",
	"신", "유", "마", "주"
};
std::string firstName[12] =
{
	"형", "남", "명", "승",
	"학", "의", "찬", "우",
	"홍", "철", "규", "대"
};
std::string secondName[12] =
{
	"현", "진", "준", "성",
	"기", "지", "은", "훈",
	"연", "원", "빈", "선"
};

// --------------- 구현부 --------------------

// 학생의 정보를 만들기 위한 함수
std::string rtnName() //이름 생성 함수
{
	srand((unsigned int)time(NULL));
	std::string studentName = familyName[rand() % 12] + firstName[rand() % 12] + secondName[rand() % 12];
	return studentName;
}
int rtnScore() //점수 생성 함수
{
	srand((unsigned int)time(NULL));
	int score = rand() % 50 + 50;
	return score;
}

// vector를 정렬하기 위한 커스텀 비교함수
bool compare(student a, student b)
{
	return a.score > b.score;
}