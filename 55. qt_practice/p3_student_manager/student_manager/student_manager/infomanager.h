#ifndef INFOMANAGER_H
#define INFOMANAGER_H
#include <iostream>
#include <string>
#include <vector>

using namespace  std;

class InfoManager
{
public:
    InfoManager();

    struct StudentInfo
    {
        string name;
        double cn_score;
        double math_score;
        double en_score;
    };

    void addStudentInfo(const StudentInfo & input);
    bool deleteStudentInfo(const string & search_name);
    int searchInfo(const string & name);
    const StudentInfo avgInfo();

    std::vector<StudentInfo> student_info;

};

#endif // INFOMANAGER_H
