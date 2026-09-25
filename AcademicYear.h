#ifndef ACADEMICYEAR_H
#define ACADEMICYEAR_H

#include "Semester.h"

class AcademicYear{
    public:
    AcademicYear();

    void setSemester(const int& index, const Semester& s);
    Semester getSemester(const int& index) const;

    bool hasCourses() const;
    int getTotalCreditHours() const;
    double getTotalPoints() const;
    double calculateYearCWA() const;   // weighted by credit hours
    void displayYear() const;

    private:
    Semester semesters[2];
};

#endif
