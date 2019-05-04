#include <iostream>
#include "SubjectClass.h"
#include "GroupClass.h"
#include "TeacherClass.h"
#include "TimeTableClass.h"
#include "EventClass.h"
#include "DataLoader.h"
#include "GenerateSchedule.h"

int main() {

    std::map<std::string, Teacher> teachers = DataLoader::GetTeachers("../DataToLoad/teachers.txt");
    std::map<std::string, Group> groups = DataLoader::GetGroups("../DataToLoad/groups.txt");
    std::map<std::string, Subject> subjects = DataLoader::GetSubjects("../DataToLoad/subjects.txt", teachers, groups);
    std::map<std::string, Cabinet> cabinets = DataLoader::GetCabinets("../DataToLoad/cabinets.txt");

    TimeTable<Event> table("../DataToLoad/groups.txt", DAYS_IN_WEEK, PAIRS_IN_DAY);

    GenerateRandomSchedule(table, subjects, teachers, groups, cabinets);
    table.Print();

    return 0;
}