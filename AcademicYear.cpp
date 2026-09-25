#include "AcademicYear.h"
#include <iostream>

AcademicYear::AcademicYear(){
    semesters[0].setName(FIRST);
    semesters[1].setName(SECOND);
}

void AcademicYear::setSemester(const int& index, const Semester& s){
    if (index < 0 || index > 1){
        cout << "Invalid semester index. Must be 0 or 1." << endl;
        return;
    }
    semesters[index] = s;
}

Semester AcademicYear::getSemester(const int& index) const{
    if (index < 0 || index > 1){
        cout << "Invalid semester index. Must be 0 or 1." << endl;
        return Semester();
    }
    return semesters[index];
}

bool AcademicYear::hasCourses() const{
    return semesters[0].getNumberOfOCourses() > 0 ||
           semesters[1].getNumberOfOCourses() > 0;
}

int AcademicYear::getTotalCreditHours() const{
    return semesters[0].getTotalCreditHours() + semesters[1].getTotalCreditHours();
}

double AcademicYear::getTotalPoints() const{
    return semesters[0].getTotalPoints() + semesters[1].getTotalPoints();
}

// Weighted by credit hours. (Averaging the two semester CWAs is only correct
// when both semesters have exactly the same number of credit hours.)
double AcademicYear::calculateYearCWA() const{
    int hours = getTotalCreditHours();
    return (hours > 0) ? getTotalPoints() / hours : 0.0;
}

void AcademicYear::displayYear() const{
    for (int i = 0; i < 2; i++){
        cout << "Semester " << (i + 1) << " CWA: " << semesters[i].getCWA() << endl;
    }
    cout << "Year CWA: " << calculateYearCWA() << endl;
}
