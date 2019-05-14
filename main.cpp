#include <iostream>
#include "Headers/SubjectClass.h"
#include "Headers/GroupClass.h"
#include "Headers/TeacherClass.h"
#include "Headers/TimeTableClass.h"
#include "Headers/EventClass.h"
#include "Headers/DataLoader.h"
#include "Headers/GenerateSchedule.h"
#include "Headers/ABCClass.h"

int main() {

    std::map<std::string, Teacher> teachers;
    DataLoader::GetTeachers(teachers, "../DataToLoad/teachers.txt");

    std::map<std::string, Group> groups;
    DataLoader::GetGroups(groups, "../DataToLoad/groups.txt");

    std::map<std::string, Subject> subjects;
    DataLoader::GetSubjects(subjects, "../DataToLoad/subjects.txt", teachers, groups);

    std::map<std::string, Cabinet> cabinets;
    DataLoader::GetCabinets(cabinets, "../DataToLoad/cabinets.txt");
    TimeTable<Event> table("../DataToLoad/groups.txt", DAYS_IN_WEEK, PAIRS_IN_DAY);

    GenerateRandomSchedule(table, subjects, teachers, groups, cabinets);
    table.Print();
//    table.DumpElems();
    TimeTable<Event> table1(table);
    std::vector<std::string> sbjs;
    for (auto i = subjects.begin(); i != subjects.end(); i++)
        sbjs.push_back((*i).first);
    table.RandomMove(subjects, sbjs, cabinets);
    table.Print();
    table.CompareTables(table1);

//    TimeTableCreator optim(20, 100, 30, subjects, teachers, groups, cabinets);

//    optim.FindOptimal(table);
//
//    table.Print();

    return 0;
}