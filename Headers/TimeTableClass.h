//
// Created by Gregory Postnikov on 2019-04-15.
//

#ifndef TIMETABLE_TIMETABLECLASS_H
#define TIMETABLE_TIMETABLECLASS_H

#include "Constants.h"
#include "EventClass.h"
#include "TeacherClass.h"
#include "ScheduleClass.h"

// ##################
// new class
// ##################
template <typename T>
class TimeTable {

    std::map<std::string, Schedule<T>*> table;

public:

    TimeTable(std::string filename, size_t days_in_week, size_t pairs_in_day);
    Schedule<T>* operator[](std::string group);
    void InsertEvent(std::string name, size_t duration, Teacher *teacher, std::vector<Group*>& groups, Time start_time, Cabinet *cabinet);
    void Print();

};

template <typename T>
TimeTable<T>::TimeTable(std::string filename, size_t days_in_week, size_t pairs_in_day) {
    size_t groups_number = 0;
    std::vector<std::string> group_names {};
    std::ifstream file(filename);
    if (!file) {
        std::cout << "ERROR" << std::endl;
        return;
    }
    std::string line;
    size_t pos;
    while (std::getline(file, line)) {
        pos = line.find(DELIMETER);
        group_names.push_back(line.substr(0, pos));
        groups_number++;
    }
    for (auto i = group_names.begin(); i < group_names.end(); i++) {
        Schedule<T> *schedule = new Schedule<T>(days_in_week, pairs_in_day);
        table.insert(std::pair(*i, schedule));
    }

}

template <typename T>
Schedule<T>* TimeTable<T>::operator[](std::string group) {
    return table.at(group);
}

template <typename T>
void TimeTable<T>::Print() {
    for (auto i = table.begin(); i != table.end(); i++) {
        std::cout << "Group " << (*i).first << std::endl;
        (*i).second->Print();
    }
}

template <typename T>
void TimeTable<T>::InsertEvent(std::string name, size_t duration, Teacher *teacher, std::vector<Group*>& groups, Time start_time, Cabinet *cabinet) {
    for (size_t j = 0; j < duration; j++) {
        for (auto i = groups.begin(); i != groups.end(); i++) {
            table[(*i)->GetName()]->GetElem(start_time + j) = Event(name, duration, teacher, start_time, cabinet);
            (*i)->TakeTime(start_time+j);
        }
        teacher->TakeTime(start_time+j);
        cabinet->TakeTime(start_time+j);
    }
}

#endif //TIMETABLE_TIMETABLECLASS_H
