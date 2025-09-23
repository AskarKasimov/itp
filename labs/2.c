#include <stdio.h>

struct Exam_day {
    int day;
    int year;
    char month[20];
};


struct Student
{
    char name[50];
    char surname[50];
    int group_number;
    struct Exam_day ed;
};

int main() {
    struct Student student;
    printf("Enter the student's name: ");
    scanf("%s", student.name);

    
    printf("Enter the student's surname: ");
    scanf("%s", student.surname);

    printf("Enter the student's group number: ");
    scanf("%d", &student.group_number);

    struct Exam_day exam_day;

    printf("Enter the exam's day: ");
    scanf("%d", &exam_day.day);
    
    printf("Enter the exam's year: ");
    scanf("%d", &exam_day.year);

    printf("Enter the exam's month: ");
    scanf("%s", exam_day.month);

    student.ed = exam_day;

    printf("Student %s %s from the group number %d has the exam on %s %d %d\n",
        student.name, student.surname, student.group_number, student.ed.month,
        student.ed.day, student.ed.year);
}
