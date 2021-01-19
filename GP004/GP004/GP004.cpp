#include "stdafx.h"
// 표준 라이브러리의 퀵 정렬을 사용함

typedef struct {
    int id;
    float score;
} student;

int compare_student(const void* p, const void* q);

int main()
{
    int n = 5;
    student* list = (student*)malloc(sizeof(student) * n);
    for (int i = 0; i < n; i++)
    {
        list[i].id = i + 1;
        list[i].score = (float)(rand() % 100);
    }
    
    qsort(list, n, sizeof(student), compare_student);

    for (int i = 0; i < n; i++)
    {
        cout << "점수 : " << list[i].id << " 성적 : " << list[i].score << endl;
    }
}

int compare_student(const void* p, const void* q)
{
    const student* ip = (const student*)p;
    const student* iq = (const student*)q;
    if (ip->score > iq->score)
    {
        return 1;
    }
    else if (ip->score < iq->score)
    {
        return -1;
    }
    else
    {
        return 0;
    }

}