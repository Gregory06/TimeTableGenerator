//
// Created by Gregory Postnikov on 2019-05-03.
//

#ifndef TIMETABLE_CABINETCLASS_H
#define TIMETABLE_CABINETCLASS_H

#include "TimeClass.h"

// ##################
// new class
// ##################
class Cabinet {

    std::string name;
    size_t capacity;
    int64_t available_time;

public:

    Cabinet(std::string name_, size_t capacity_, int64_t availible_time_);
    std::string GetName() const;
    size_t GetCapacity() const;
    bool IsFeasible(Time start_time, size_t duration) const;
    void TakeTime(Time time);
    void ReleaseTime(Time time);
    void Print() const;

};

Cabinet::Cabinet(std::string name_, size_t capacity_, int64_t availible_time_)
        : name(name_),
          capacity(capacity_),
          available_time(availible_time_)
{}

std::string Cabinet::GetName() const {
    return name;
}

size_t Cabinet::GetCapacity() const {
    return capacity;
}

bool Cabinet::IsFeasible(Time start_time, size_t duration) const {
    bool is_feasible = true;
    for (size_t i = start_time.GetIndex(); i < start_time.GetIndex() + duration; i++) {
        is_feasible = (bool) (available_time & (1 << i));
        if (!is_feasible)
            break;
    }

    return is_feasible;
}

void Cabinet::TakeTime(const Time time) {
    size_t index = time.GetIndex();
    available_time &= ~(1 << index);
}

void Cabinet::ReleaseTime(const Time time) {
    size_t index = time.GetIndex();
    available_time |= 1 << index;
}

void Cabinet::Print() const {
    std::cout << "##########" << std::endl;
    std::cout << "### Cabinet " << name << std::endl;
    std::cout << "### of capacity " << capacity << std::endl;
    std::cout << "### and availible time " << std::bitset<sizeof(available_time) * CHAR_BIT>(available_time) << std::endl;
    std::cout << "##########" << std::endl;
}

#endif //TIMETABLE_CABINETCLASS_H
