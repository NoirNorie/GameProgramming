#pragma once
#include "stdafx.h"

/*
	�л��� ������ ���õ� Header ����
*/

// �л� ���� ����ü
struct student
{
	int score;
	std::string name;
};

// vector�� ������ ���� Ŀ���� �� �Լ�
bool compare(student a, student b);

// �л��� ������ ����� ���� �Լ�
std::string rtnName(); //�̸� ���� �Լ�
int rtnScore(); //���� ���� �Լ�