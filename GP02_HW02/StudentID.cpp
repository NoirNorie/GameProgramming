#include "StudentID.h"

/*
	�л��� ������ ���õ� CPP ����
*/

// --------------- ����� --------------------
// ���� ����
//std::vector<student> vec; // �л��� ������ ������ vector

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

// --------------- ������ --------------------

// �л��� ������ ����� ���� �Լ�
std::string rtnName() //�̸� ���� �Լ�
{
	srand((unsigned int)time(NULL));
	std::string studentName = familyName[rand() % 12] + firstName[rand() % 12] + secondName[rand() % 12];
	return studentName;
}
int rtnScore() //���� ���� �Լ�
{
	srand((unsigned int)time(NULL));
	int score = rand() % 50 + 50;
	return score;
}

// vector�� �����ϱ� ���� Ŀ���� ���Լ�
bool compare(student a, student b)
{
	return a.score > b.score;
}