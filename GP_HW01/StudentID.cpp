#include "StudentID.h"
#include <tchar.h>

// --------------- ����� --------------------
// ���� ����
std::vector<student> vec; // �л��� ������ ������ vector
std::pair<int, int> pos; // x,y ��ǥ�� ������ pair ��ü
std::pair<int, int> textpos; // ������ ���� x,y ��ǥ�� ����� pair ��ü

//������ ��ȯ�ϴµ� ����ϴ� ������
int screenleft, screentop, screenright, screenbottom = 0;
int drawleft, drawtextleft;

PAINTSTRUCT ps;

// �л��� �̸��� ���鶧 ����� �迭
std::string familyName[12] =
{
	"��", "��", "��", "��",
	"��", "��", "��", "��",
	"��", "��", "��", "��"
};
std::string firstName[12] =
{
	"��", "��", "��", "��",
	"��", "��", "��", "��",
	"ȫ", "ö", "��", "��"
};
std::string secondName[12] =
{
	"��", "��", "��", "��",
	"��", "��", "��", "��",
	"��", "��", "��", "��"
};

//�Լ�

void init(HWND hwnd); //���� �Լ�

//vector�� ������ �����ϰų� ��ȯ���ִ� �Լ�
bool compare(student a, student b); // Ŀ���� �� �Լ�
std::string rtnName(); //�̸� ���� �Լ�
int rtnScore(); //���� ���� �Լ�
std::wstring String_Transfer(const std::string& s); // String�� LPCWSTR�� ��ȯ�ϴ� �Լ�

//vector �ڽ� ���� �Լ�
std::pair<int, int> getlastPosition(); // vector�� ������ ��ġ�� �� �Լ�
void setlastPosition(int y); // vector�� ���� ������ ��ġ�� ���� �Լ�
void vectorpop(); // vector�� pop ������ ���� �Լ�

// --------------- ����� --------------------


// --------------- ������ --------------------

void init(HWND hwnd) {
	if (screenbottom != 0) // init�Լ��� �� �ѹ� �Ҹ��� ���� ���� ���ؼ��̴�.
	{
		return;
	}
	RECT rc;
	GetClientRect(hwnd, &rc);
	screenleft = rc.left;
	screentop = rc.top;
	screenright = rc.right;
	screenbottom = rc.bottom; //screen ����� ����ͼ� ������ screen ���ڿ� �־��ش�.

	drawleft = (screenright - screenleft) / 2 - 100; // vector�� ���� x��ǥ
	drawtextleft = drawleft + 100 - 30; // text�� ��ϵ� x��ǥ

	pos.first = drawleft; // vector�� ���� �ֱ� vector�� x,y��ǥ�� �־��ش�.
	pos.second = screenbottom;

	srand(time(NULL));
}

LRESULT drawVector(HWND hwnd) //vector�� ����ϴµ� ����ϴ� �Լ�
{
	BeginPaint(hwnd, &ps);
	HPEN hMyPen = CreatePen(PS_SOLID, 3, RGB(rand() % 255, rand() % 255, rand() % 255)); // ���� ������
	HGDIOBJ hOldPen = SelectObject(ps.hdc, hMyPen); // ���� ������	

	if (vec.size() == 8) 	// ���� �ִ� vector�� ũ��� 8�� �� ũ�⸦ �Ѿ�� ��츦 �����ϱ� ���� �κ�
	{
		return -1; // -1�� ���Ͻ�Ų��
	}

	// ���� ������ ��� student ��ü�� �ϳ� �����ϰ� vector�� push_back �Ѵ�

	student tempObject = { rtnScore(), rtnName() };
	vec.push_back(tempObject);

	std::string score = std::to_string(vec.back().score); // Int���� score�� string������ ��ȯ
	std::string nameAndScore = vec.back().name + " " + score; // name�� score�� ���ļ� string���� ��ħ
	std::wstring tmp = String_Transfer(nameAndScore); //��ȯ �Լ��� ����� string Ÿ���� LPCWSTR�� ��ȯ
	LPCWSTR Disp = tmp.c_str();
	pos.second = pos.second - 55; // vector�ڽ� ���̿� ������ ���

	HBRUSH MyBrush, OldBrush;
	COLORREF color = RGB(rand() % 255, rand() % 255, rand() % 255);

	MyBrush = CreateHatchBrush(HS_HORIZONTAL, color);
	OldBrush = (HBRUSH)SelectObject(ps.hdc, MyBrush);


	Rectangle(ps.hdc, drawleft, pos.second, drawleft + 200, screentop + pos.second + 50);
	TextOut(ps.hdc, drawtextleft, pos.second + 15, Disp, 6);

	return 0;
}

void RewriteVector(HWND hwnd) // push ���� ���� vector�� ���� ������ ����ϴ� �Լ�
{
	textpos.second = screenbottom; //���� �غκ�

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

// �л��� ������ ����� ���� �Լ�
std::string rtnName() //�̸� ���� �Լ�
{
	std::string studentName = familyName[rand() % 12] + firstName[rand() % 12] + secondName[rand() % 12];
	return studentName;
}
int rtnScore() //���� ���� �Լ�
{
	int score = rand() % 50 + 50;
	return score;
}

std::wstring String_Transfer(const std::string& s) // String�� LPCWSTR�� ��ȯ�ϴ� �Լ�
{
	int len;
	int slength = (int)s.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];

	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
	std::wstring r(buf);
	delete[] buf; //�޸� ������ ���ƾ� �Ѵ�

	return r;
}

// --------------- ������ --------------------