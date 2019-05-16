//
// Created by Gregory Postnikov on 2019-04-24.
//

#ifndef TIMETABLE_GROUPCLASS_H
#define TIMETABLE_GROUPCLASS_H

#include <fstream>
#include "EventClass.h"
#include "Constants.h"
#include "ScheduleClass.h"

// ##################
// new class
// ##################
class Group {

    std::string name;
    size_t student_number;
//    Schedule<Event> *schedule;
    int64_t available_time;

public:

    Group(std::string name_, size_t student_number_, int64_t current_available_time_, size_t days_in_week, size_t pairs_in_day);
    size_t GetStudentNumber() const;
    int64_t GetFeasibleTime() const;
    std::string GetName() const;
    void Print() const;
    void TakeTime(Time time);
    void ReleaseTime(Time time);

};

Group::Group(std::string name_, size_t student_number_, int64_t available_time_, size_t days_in_week, size_t pairs_in_day)
        : name(name_),
          student_number(student_number_),
          available_time(available_time_)
{}

size_t Group::GetStudentNumber() const {
    return student_number;
}

int64_t Group::GetFeasibleTime() const {
    return available_time;
}

std::string Group::GetName() const {
    return name;
}

void Group::Print() const {
    std::cout << "##########" << std::endl;
    std::cout << "### Group " << name << std::endl;
    std::cout << "### with " << student_number << " students" << std::endl;
    std::cout << "### and feasible time " << std::bitset<sizeof(available_time) * CHAR_BIT>(available_time) << std::endl;
    std::cout << "##########" << std::endl;
}

void Group::TakeTime(const Time time){
    size_t index = time.GetIndex();
    available_time &= ~(1 << index);
}

void Group::ReleaseTime(const Time time) {
    size_t index = time.GetIndex();
    available_time |= 1 << index;
}

#endif //TIMETABLE_GROUPCLASS_H
