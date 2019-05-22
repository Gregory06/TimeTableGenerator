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

    size_t days, pair;
    std::map<std::string, Schedule<T>*> table;
    std::map<std::string, std::pair<std::string, Time>> elements;

public:

    TimeTable(std::vector<std::string>& group_names, size_t days_in_week, size_t pairs_in_day);
    TimeTable(const TimeTable<T> &example, std::map<std::string, Teacher>& teachers, std::map<std::string, Cabinet>& cabinets);
    TimeTable();
    Schedule<T>* operator[](std::string group);
    T& GetElemByName(std::string elem) const;
    void InsertEvent(std::string name, size_t duration, size_t type, Teacher *teacher, std::vector<Group*>& groups, Time start_time, Cabinet *cabinet);
    void InsertEvent(T& elem, std::vector<Group*>& groups);
    void DeleteEvent(Time time, std::vector<Group*> &groups);
    void Print();
    bool Swap(T& elem1, T& elem2, std::map<std::string, Subject> &subjects, std::map<std::string, Cabinet> &cabinets);
    bool RandomSwap(std::map<std::string, Subject> &subjects, std::vector<std::string> &subjects_name, std::map<std::string, Cabinet> &cabinets);
    bool Move(T& elem, std::map<std::string, Subject> &subjects, std::map<std::string, Cabinet> &cabinets);
    bool RandomMove(std::map<std::string, Subject> &subjects, std::vector<std::string> &subjects_name, std::map<std::string, Cabinet> &cabinets);
    std::map<std::string, Schedule<T>*>& GetTable();
    void WriteCSV(std::string filename);
    void CompareTables(TimeTable<Event>& b);
    void DumpElems();

    ~TimeTable();
};

#include "GenerateSchedule.h"

template <typename T>
TimeTable<T>::TimeTable(std::vector<std::string>& group_names, size_t days_in_week, size_t pairs_in_day)
    : days(days_in_week),
    pair(pairs_in_day) {

    for (auto i = group_names.begin(); i < group_names.end(); i++) {
        Schedule<T> *schedule = new Schedule<T>(days_in_week, pairs_in_day);
        table.insert(std::pair(*i, schedule));
    }

}

template <typename T>
TimeTable<T>::TimeTable(const TimeTable<T> &example, std::map<std::string, Teacher>& teachers, std::map<std::string, Cabinet>& cabinets)
    : elements(example.elements)
    {
//    std::cout << "Babe im fucked up" << std::endl;
    for (auto i = example.table.begin(); i != example.table.end(); i++) {
        Schedule<T> *new_schedule = new Schedule<T>(*(*i).second, teachers, cabinets);
//        for (size_t k = 0; k < example.days; k++)
//            for (size_t j = 0; j < example.pair; j++)
//                new_schedule->operator[](Time (k,j)) = (*i).second->operator[](Time (k,j));
        table.insert(std::pair((*i).first, new_schedule));
    }
}

template <typename T>
TimeTable<T>::TimeTable() {}

template <typename T>
Schedule<T>* TimeTable<T>::operator[](std::string group) {
    return table.at(group);
}

template <typename T>
T& TimeTable<T>::GetElemByName(std::string elem) const {

}

template <typename T>
void TimeTable<T>::Print() {
    std::cout << "###___________TIME_TABLE__________###" << std::endl;
    for (auto i = table.begin(); i != table.end(); i++) {
        std::cout << "#Group " << (*i).first << std::endl;
        std::cout << std::endl;
        (*i).second->Print();
    }
    std::cout << "###_______________________________###" << std::endl;
}

template <typename T>
void TimeTable<T>::InsertEvent(std::string name, size_t duration, size_t type, Teacher *teacher, std::vector<Group*>& groups, Time start_time, Cabinet *cabinet) {
    for (size_t j = 0; j < duration; j++) {
        for (auto i = groups.begin(); i != groups.end(); i++) {
            table[(*i)->GetName()]->GetElem(start_time + j) = Event(name, duration, type, teacher, start_time, cabinet);
            (*i)->TakeTime(start_time+j);
        }
        teacher->TakeTime(start_time+j);
        cabinet->TakeTime(start_time+j);
    }
    elements.insert(std::pair(name, std::pair((*groups.begin())->GetName(),start_time)));
}

template <typename T>
void TimeTable<T>::InsertEvent(T& elem,  std::vector<Group*>& groups) {
    for (size_t j = 0; j < elem.GetDuration(); j++) {
        for (auto i = groups.begin(); i != groups.end(); i++) {
            table[(*i)->GetName()]->GetElem(elem.GetStartTime() + j) = Event(elem);
            (*i)->TakeTime(elem.GetStartTime()+j);
        }
        elem.GetTeacher()->TakeTime(elem.GetStartTime()+j);
        elem.GetCabinet()->TakeTime(elem.GetStartTime()+j);
    }
    elements.insert(std::pair(elem.GetName(), std::pair((*groups.begin())->GetName(), elem.GetStartTime())));
}

template <typename T>
void TimeTable<T>::DeleteEvent(Time time, std::vector<Group*> &groups) {
    T &event(table[(*groups.begin())->GetName()]->GetElem(time));
    elements.erase(event.GetName());
    for (size_t i = 0; i < event.GetDuration(); i++) {
        for (auto group = groups.begin(); group != groups.end(); group++) {
            table[(*groups.begin())->GetName()]->GetElem(event.GetStartTime() + i).Deactivate();
            (*group)->ReleaseTime(event.GetStartTime() + i);
        }
        event.GetTeacher()->ReleaseTime(event.GetStartTime()+i);
        event.GetCabinet()->ReleaseTime(event.GetStartTime()+i);
    }
}

template <typename T>
bool TimeTable<T>::Swap(T& elem1, T& elem2, std::map<std::string, Subject> &subjects, std::map<std::string, Cabinet> &cabinets) {
//    std::cout << elements.at("Дифференциальные уравнения").first << std::endl;
    T temp1(elem1), temp2(elem2);
    std::vector<Group*> group1(subjects.at(elem1.GetName()).GetGroups()), group2(subjects.at(elem2.GetName()).GetGroups());
    if (!GroupIntersection(group1, group2))
        return false;
    Subject subject1(subjects.at(elem1.GetName())), subject2(subjects.at(elem2.GetName()));
    DeleteEvent(elem1.GetStartTime(), group1);
    DeleteEvent(elem2.GetStartTime(), group2);
    int64_t feasible_time1(subject1.GetResultingFeasibleTime()), feasible_time2(subject2.GetResultingFeasibleTime()) ;

    for (size_t i = elem1.GetStartTime().GetIndex(); i < elem1.GetStartTime().GetIndex()+elem1.GetDuration(); i++) {
        if (!(feasible_time2 & (1 << i))) {
            InsertEvent(temp1, group1);
            InsertEvent(temp2, group2);
            return false;
        }
    }

    for (size_t i = elem2.GetStartTime().GetIndex(); i < elem2.GetStartTime().GetIndex()+elem2.GetDuration(); i++) {
        if (!(feasible_time1 & (1 << i))) {
            InsertEvent(temp1, group1);
            InsertEvent(temp2, group2);
            return false;
        }
    }

    Cabinet *cabinet1(GetFeasibleCabinet(cabinets, temp2.GetStartTime(), temp1.GetDuration(), subject1.GetParticipantsNumber())),
            *cabinet2(GetFeasibleCabinet(cabinets, temp1.GetStartTime(), temp2.GetDuration(), subject2.GetParticipantsNumber()));

    if (!cabinet1 || !cabinet2) {
        InsertEvent(temp1, group1);
        InsertEvent(temp2, group2);
        return false;
    }

    temp1.AssignCabinet(cabinet1);
    temp2.AssignCabinet(cabinet2);

    Time temp_time;
    temp_time = temp2.GetStartTime();
    temp2.GetStartTime() = temp1.GetStartTime();
    temp1.GetStartTime() = temp_time;

//    std::cout << std::endl;
//    temp1.Print();
//    temp2.Print();
//    std::cout << std::endl;

    InsertEvent(temp1, group1);
    InsertEvent(temp2, group2);

    return true;

}

template <typename T>
bool TimeTable<T>::RandomSwap(std::map<std::string, Subject> &subjects, std::vector<std::string> &subjects_name, std::map<std::string, Cabinet> &cabinets) {
    std::vector<std::string> random_permutation_from(subjects_name), random_permutation_to(subjects_name);
    RandomPermutation(random_permutation_from);
    RandomPermutation(random_permutation_to);
    for (auto i = random_permutation_from.begin(); i != random_permutation_from.end(); i++)
        for (auto j = random_permutation_to.begin(); j != random_permutation_to.end(); j++)
            if (*i != *j) {
//                std::cout << *i << " | " << *j << std::endl;
//                std::cout << elements.at(*i).first << " | ";
//                std::cout << elements.at(*j).first << std::endl;
                if (Swap(table[elements.at(*i).first]->GetElem(elements.at(*i).second),
                         table[elements.at(*j).first]->GetElem(elements.at(*j).second),
                         subjects, cabinets)) {
                    return true;
                }
            }

    return false;
}

template <typename T>
bool TimeTable<T>::Move(T& elem, std::map<std::string, Subject> &subjects, std::map<std::string, Cabinet> &cabinets) {
    T temp(elem);
    Subject &subject(subjects.at(elem.GetName()));
    std::vector<Group*> groups(subject.GetGroups());
    DeleteEvent(elem.GetStartTime(), groups);
    int64_t feasible_time (subject.GetResultingFeasibleTime());
    std::vector<Time> shufled_fesible_time {};
    Int2Vector(shufled_fesible_time, feasible_time);
    RandomPermutation(shufled_fesible_time);

    for (auto time = shufled_fesible_time.begin(); time != shufled_fesible_time.end(); time++) {
        Cabinet *cabinet = GetFeasibleCabinet(cabinets, *time, subject.GetDuration(), subject.GetParticipantsNumber());
        if (!cabinet)
            continue;
        InsertEvent(subject.GetName(), subject.GetDuration(),
                    subject.GetType(), subject.GetTeacher(),
                    groups, *time, cabinet);
        return true;
    }

    InsertEvent(temp, groups);
    return false;
}

template <typename T>
bool TimeTable<T>::RandomMove(std::map<std::string, Subject> &subjects, std::vector<std::string> &subjects_name, std::map<std::string, Cabinet> &cabinets) {
    std::vector<std::string> random_permutation(subjects_name);
    RandomPermutation(random_permutation);
    for (auto i = random_permutation.begin(); i != random_permutation.end(); i++)
        if (Move(table[elements.at(*i).first]->GetElem(elements.at(*i).second),
                 subjects, cabinets)) {
            return true;
        }

    return false;
}

template <typename T>
std::map<std::string, Schedule<T>*>& TimeTable<T>::GetTable() {
    return table;
}

template <typename T>
void TimeTable<T>::WriteCSV(std::string filename) {
     std::ofstream file;
     file.open(filename);
     if (!file.is_open()) {
         std::cout << "FILE " << filename << " OPENING ERROR" << std::endl;
         return;
     }
     file.clear();
     file << "D;P";
     for (auto group = table.begin(); group != table.end(); group++)
//         for (size_t i = 0; i < 3; i++)
             file << ";" << (*group).first;
     file << std::endl;

     for (size_t day = 0; day < DAYS_IN_WEEK; day++)
         for (size_t pair = 0; pair < PAIRS_IN_DAY; pair++) {
             file << day << ";" << pair;
             for (auto group = table.begin(); group != table.end(); group++) {
                 file << ";" << (*group).second->GetElem(Time(day, pair)).GetName();
                 if ((*group).second->GetElem(Time(day, pair)).GetTeacher()) {
                     file << "#" << (*group).second->GetElem(Time(day, pair)).GetTeacher()->GetName();
                     file << "#" << (*group).second->GetElem(Time(day, pair)).GetCabinet()->GetName();
//                     file << ";";
                 } else {
                     file << "##";
                 }
             }
             file << std::endl;
         }


     file.close();
}

template <typename T>
void TimeTable<T>::DumpElems() {
    for (auto i = elements.begin(); i != elements.end(); i++) {
        std::cout << "___" << std::endl;
        std::cout << (*i).first << std::endl;
        std::cout << (*i).second.first << std::endl;
        (*i).second.second.Print();
    }
}


template <>
void TimeTable<Event>::CompareTables(TimeTable<Event>& b) {
    auto j = b.table.begin();
    for (auto i = table.begin(); i != table.end(); i++) {
        for (size_t a = 0; a < DAYS_IN_WEEK; a++)
            for (size_t b = 0; b < PAIRS_IN_DAY; b++) {
                (*i).second->GetElem(Time(a,b)).Compare((*j).second->GetElem(Time(a,b)));
            }
        j++;
        if (j == b.table.end())
            break;
    }
}


template <typename T>
TimeTable<T>::~TimeTable() {
    for (auto i = table.begin(); i != table.end(); i++) {
        delete (*i).second;
    }
}


#endif //TIMETABLE_TIMETABLECLASS_H
