#include "StudentID.h"
#include <tchar.h>

// --------------- 선언부 --------------------
// 전역 변수
std::vector<student> vec; // 학생의 정보를 저장할 vector
std::pair<int, int> pos; // x,y 좌표를 저장할 pair 객체
std::pair<int, int> textpos; // 정렬을 위해 x,y 좌표를 사용할 pair 객체

//정보를 반환하는데 사용하는 변수들
int screenleft, screentop, screenright, screenbottom = 0;
int drawleft, drawtextleft;

PAINTSTRUCT ps;

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

//함수

void init(HWND hwnd); //동작 함수

//vector의 내용을 생성하거나 변환해주는 함수
bool compare(student a, student b); // 커스텀 비교 함수
std::string rtnName(); //이름 생성 함수
int rtnScore(); //점수 생성 함수
std::wstring String_Transfer(const std::string& s); // String을 LPCWSTR로 변환하는 함수

//vector 박스 관련 함수
std::pair<int, int> getlastPosition(); // vector의 마지막 위치를 얻어낼 함수
void setlastPosition(int y); // vector가 쌓일 마지막 위치를 위한 함수
void vectorpop(); // vector의 pop 동작을 위한 함수

// --------------- 선언부 --------------------


// --------------- 구현부 --------------------

void init(HWND hwnd) {
	if (screenbottom != 0) // init함수가 또 한번 불리는 것을 막기 위해서이다.
	{
		return;
	}
	RECT rc;
	GetClientRect(hwnd, &rc);
	screenleft = rc.left;
	screentop = rc.top;
	screenright = rc.right;
	screenbottom = rc.bottom; //screen 사이즈를 갖고와서 각각의 screen 인자에 넣어준다.

	drawleft = (screenright - screenleft) / 2 - 100; // vector가 쌓일 x좌표
	drawtextleft = drawleft + 100 - 30; // text가 기록될 x좌표

	pos.first = drawleft; // vector에 가장 최근 vector의 x,y좌표를 넣어준다.
	pos.second = screenbottom;

	srand(time(NULL));
}

LRESULT drawVector(HWND hwnd) //vector를 출력하는데 사용하는 함수
{
	BeginPaint(hwnd, &ps);
	HPEN hMyPen = CreatePen(PS_SOLID, 3, RGB(rand() % 255, rand() % 255, rand() % 255)); // 펜을 생성함
	HGDIOBJ hOldPen = SelectObject(ps.hdc, hMyPen); // 펜을 선택함	

	if (vec.size() == 8) 	// 허용된 최대 vector의 크기는 8로 이 크기를 넘어서는 경우를 제거하기 위한 부분
	{
		return -1; // -1을 리턴시킨다
	}

	// 정상 동작한 경우 student 객체를 하나 생성하고 vector에 push_back 한다

	student tempObject = { rtnScore(), rtnName() };
	vec.push_back(tempObject);

	std::string score = std::to_string(vec.back().score); // Int형인 score를 string형으로 변환
	std::string nameAndScore = vec.back().name + " " + score; // name과 score를 합쳐서 string으로 합침
	std::wstring tmp = String_Transfer(nameAndScore); //변환 함수를 사용해 string 타입을 LPCWSTR로 변환
	LPCWSTR Disp = tmp.c_str();
	pos.second = pos.second - 55; // vector박스 사이에 간격을 띄움

	HBRUSH MyBrush, OldBrush;
	COLORREF color = RGB(rand() % 255, rand() % 255, rand() % 255);

	MyBrush = CreateHatchBrush(HS_HORIZONTAL, color);
	OldBrush = (HBRUSH)SelectObject(ps.hdc, MyBrush);


	Rectangle(ps.hdc, drawleft, pos.second, drawleft + 200, screentop + pos.second + 50);
	TextOut(ps.hdc, drawtextleft, pos.second + 15, Disp, 6);

	return 0;
}

void RewriteVector(HWND hwnd) // push 동작 이후 vector의 값을 정렬해 출력하는 함수
{
	textpos.second = screenbottom; //가장 밑부분

	for (std::vector<student>::size_type i = 0; i < vec.size(); i++)
	{
		std::sort(vec.begin(), vec.end(), compare);
		std::string tes = std::to_string(i);
		std::string score = std::to_string(vec[i].score);
		std::string nameAndScore = vec[i].name + " " + score + tes;
		std::wstring tmp = String_Transfer(nameAndScore);
		LPCWSTR Disp = tmp.c_str();
		textpos.second = textpos.second - 55;
		TextOut(ps.hdc, drawtextleft, textpos.second + 15, Disp, 6);
	}
}


std::pair<int, int> getlastPosition()
{
	return pos;
}
void setlastPosition(int y)
{
	pos.second = y + 5;
}
void vectorpop()
{
	if (vec.size() >= 1)
	{
		vec.pop_back();
	}
}
bool compare(student a, student b)
{
	return a.score > b.score;
}

// 학생의 정보를 만들기 위한 함수
std::string rtnName() //이름 생성 함수
{
	std::string studentName = familyName[rand() % 12] + firstName[rand() % 12] + secondName[rand() % 12];
	return studentName;
}
int rtnScore() //점수 생성 함수
{
	int score = rand() % 50 + 50;
	return score;
}

std::wstring String_Transfer(const std::string& s) // String을 LPCWSTR로 변환하는 함수
{
	int len;
	int slength = (int)s.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];

	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
	std::wstring r(buf);
	delete[] buf; //메모리 누수는 막아야 한다

	return r;
}

// --------------- 구현부 --------------------