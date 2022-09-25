#include "infomanager.h"

InfoManager::InfoManager()
{

}

void InfoManager::addStudentInfo(const StudentInfo & input)
{
    int idx = searchInfo(input.name);
    if(idx<0)
    {
        student_info.push_back(input);
    }
}

bool InfoManager::deleteStudentInfo(const string & search_name)
{
    unsigned int idx = 0;
    for(const auto & student:student_info)
    {
        if(student.name == search_name) idx ++;
    }
    if(idx != student_info.size())
    {
        student_info.erase(student_info.begin()+idx);
        return true;
    }
    return false;
}

int InfoManager::searchInfo(const string & name)
{
    int index = 0;
    for(const auto & student:student_info)
    {
        if(student.name == name)
        return index;
        index ++;
    }
    index = -1;
    return index;
}

const InfoManager::StudentInfo InfoManager::avgInfo()
{
    StudentInfo tmp;
    double avg_math =0.,avg_cn=0.,avg_en=0.;
    for(const auto & student:student_info)
    {
        avg_math += student.math_score;
        avg_cn += student.cn_score;
        avg_en += student.en_score;
    }
    avg_math /= double(student_info.size());
    avg_cn /= double(student_info.size());
    avg_en /= double(student_info.size());

    tmp.name = "Average";
    tmp.cn_score = avg_cn;
    tmp.en_score = avg_en;
    tmp.math_score = avg_math;

    return tmp;
}
