#include "Degree.h"
#include <iostream>

Degree::Degree() : years(DEFAULT_YEARS){
}

int Degree::getNumberOfYears() const{
    return (int)years.size();
}

void Degree::setYear(int index, const AcademicYear& year){
    if (index < 0 || index >= MAX_YEARS){
        cout << "Invalid year. Must be between 1 and " << MAX_YEARS << "." << endl;
        return;
    }
    if (index >= (int)years.size()){
        years.resize(index + 1);
    }
    years[index] = year;
}

AcademicYear Degree::getYear(int index) const{
    if (index < 0 || index >= (int)years.size()){
        return AcademicYear();
    }
    return years[index];
}

int Degree::getTotalCreditHours() const{
    int total = 0;
    for (size_t i = 0; i < years.size(); i++){
        total += years[i].getTotalCreditHours();
    }
    return total;
}

double Degree::getTotalPoints() const{
    double total = 0;
    for (size_t i = 0; i < years.size(); i++){
        total += years[i].getTotalPoints();
    }
    return total;
}

double Degree::calculateCumulativeCWA() const{
    int hours = getTotalCreditHours();
    return (hours > 0) ? getTotalPoints() / hours : 0.0;
}
