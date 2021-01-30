// GP019.cpp : 함수 객체

#include <iostream>
using namespace std;

class Matrix
{
    int m_matrix[5][5];
    int m_row;
    int m_col;
public:
    Matrix(int r, int c) {}
    int operator()(int, int) const;
    //함수처럼 동작하는 객체의 생성이 가능해진다.
};

int Matrix::operator()(int r, int c) const
{
    if (r > 0 && r <= m_row && c > 0 && c <= m_col)
    {
        return m_matrix[r][c];
    }
    else
    {
        return 0;
    }
}

int main()
{
    Matrix m(10, 10);

    int element = m(5, 5);
}