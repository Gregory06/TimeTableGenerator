//
// Created by Gregory Postnikov on 2019-04-24.
//

#ifndef TIMETABLE_TEACHERCLASS_H
#define TIMETABLE_TEACHERCLASS_H

#include <fstream>
#include "Constants.h"
#include "TimeClass.h"

// ##################
// new class
// ##################
class Teacher {

    std::string name;
    int64_t feasible_time;
    int64_t current_available_time;

public:

    Teacher(std::string name_, int64_t feasible_time_);

    size_t GetAvailibleTimeLen() const;
    int64_t GetFeasibleTime() const;
    void Print() const;
    void TakeTime(Time time);
    void ReleaseTime(Time time);

};

Teacher::Teacher(std::string name_, int64_t feasible_time_)
        : name(name_),
          feasible_time(feasible_time_),
          current_available_time(feasible_time_)
{}

size_t Teacher::GetAvailibleTimeLen() const {
    size_t a = current_available_time;
    size_t ones_number = 0;
    while (a != 0) {
        if (a % 2)
            ones_number += 1;
        a /= 2;
    }
    return ones_number;
}

int64_t Teacher::GetFeasibleTime() const {
    return feasible_time;
}

void Teacher::Print() const {
    std::cout << "##########" << std::endl;
    std::cout << "### Teacher " << name << std::endl;
    std::cout << "### and feasible time " << std::bitset<sizeof(feasible_time) * CHAR_BIT>(feasible_time) << std::endl;
    std::cout << "##########" << std::endl;

}

void Teacher::TakeTime(const Time time) {
    size_t index = time.GetIndex();
    feasible_time &= ~(1 << index);
}

void Teacher::ReleaseTime(const Time time) {
    size_t index = time.GetIndex();
    feasible_time |= 1 << index;
}

#endif //TIMETABLE_TEACHERCLASS_H
