#ifndef DEGREE_H
#define DEGREE_H

#include <vector>
#include "AcademicYear.h"

const int DEFAULT_YEARS = 4;   // a normal degree
const int MAX_YEARS = 8;       // room for longer / professional programmes

class Degree{
    public:
    Degree();

    int getNumberOfYears() const;

    // index is 0-based. setYear grows the degree if needed (up to MAX_YEARS).
    void setYear(int index, const AcademicYear& year);
    AcademicYear getYear(int index) const;   // empty year if nothing stored yet

    int getTotalCreditHours() const;
    double getTotalPoints() const;
    double calculateCumulativeCWA() const;   // across every year, weighted by credit hours

    private:
    vector<AcademicYear> years;
};

#endif
