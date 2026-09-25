#include "Semester.h"

Semester::Semester(){
    name = NONE;
}

void Semester::setName(const semester& s){
    name = s;
}

semester Semester::getName() const{
    return name;
}

void Semester::createourse(const Courses& c){
    courses.push_back(c);
}

void Semester::clearCourses(){
    courses.clear();
}

void Semester::reserveCourseNumber(int x){
    courses.reserve(x);
}

const vector<Courses>& Semester::getCourses() const{
    return courses;
}

int Semester::getNumberOfOCourses() const{
    return (int)courses.size();
}

int Semester::getTotalCreditHours() const{
    int total = 0;
    for (size_t i = 0; i < courses.size(); i++){
        total += courses[i].creditHours;
    }
    return total;
}

double Semester::getTotalPoints() const{
    double total = 0;
    for (size_t i = 0; i < courses.size(); i++){
        total += courses[i].score * courses[i].creditHours;
    }
    return total;
}

double Semester::getCWA() const{
    int hours = getTotalCreditHours();
    return (hours > 0) ? getTotalPoints() / hours : 0.0;
}
