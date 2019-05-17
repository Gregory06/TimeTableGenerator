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

//    std::vector<int> a{1,2,3,4,5};
//
//    std::vector<int> &c = a;
//    std::vector<int> b(c);
//
//    b[0]=666;
//
//    for (auto i = a.begin(); i != a.end(); i ++)
//        std::cout << *i << std::endl;
//    for (auto i = b.begin(); i != b.end(); i ++)
//        std::cout << *i << std::endl;
//
//    return 0;

    std::map<std::string, Teacher> teachers;
    DataLoader::GetTeachers(teachers, "../DataToLoad/teachers.txt");

    std::map<std::string, Group> groups;
    DataLoader::GetGroups(groups, "../DataToLoad/groups.txt");

    std::map<std::string, Subject> subjects;
    DataLoader::GetSubjects(subjects, "../DataToLoad/subjects.txt", teachers, groups);

    std::map<std::string, Cabinet> cabinets;
    DataLoader::GetCabinets(cabinets, "../DataToLoad/cabinets.txt");

    TimeTableCreator optim(10, 100, 15, subjects, teachers, groups, cabinets);

    TimeTable<Event> best(optim.FindOptimal()->GetSolution(), teachers, cabinets);
    best.WriteCSV("../Output/result.txt");

    std::cout << "BEST SOLUTION" << std::endl;
    best.Print();

    return 0;
}