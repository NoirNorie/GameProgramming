#pragma once
#include "stdafx.h"

// �л� ����ü �迭
struct student
{
	int score;
	std::string name;
};
LRESULT drawVector(HWND hwnd);
void init(HWND hwnd);

std::pair<int, int> getlastPosition();

void setlastPosition(int y);
void vectorpop();

void RewriteVector(HWND hwnd);