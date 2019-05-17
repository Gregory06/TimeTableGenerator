//
// Created by Gregory Postnikov on 2019-05-15.
//

#ifndef TIMETABLE_SOLUTIONCLASS_H
#define TIMETABLE_SOLUTIONCLASS_H

#include "TimeTableClass.h"
#include "SubjectClass.h"
#include "TimeTableClass.h"
#include "GroupClass.h"
#include "CabinetClass.h"
#include "EventClass.h"

class Solution {

    std::map<std::string,Subject> subjects;
    std::map<std::string,Teacher> teachers;
    std::map<std::string,Group> groups;
    std::map<std::string,Cabinet> cabinets;

    TimeTable<Event> solution;

public:

    Solution();
    Solution(std::vector<std::string>& group_names_, size_t days_in_week, size_t pairs_in_day,
             std::map<std::string,Subject>& subjects_, std::map<std::string,Teacher>& teachers_,
             std::map<std::string,Group>& groups_, std::map<std::string,Cabinet>& cabinets_);
    Solution(const Solution& solution_);
    void GetRandomSolution();
    void RandomSwap(std::vector<std::string>& subject_names);
    void RandomMove(std::vector<std::string>& subject_names);
    TimeTable<Event>& GetSolution();
    ~Solution();

};

Solution::Solution() {}

Solution::Solution(std::vector<std::string>& group_names_, size_t days_in_week, size_t pairs_in_day,
                   std::map<std::string,Subject>& subjects_, std::map<std::string,Teacher>& teachers_,
                   std::map<std::string,Group>& groups_, std::map<std::string,Cabinet>& cabinets_)
    : solution(group_names_, days_in_week, pairs_in_day),
      teachers(teachers_),
      groups(groups_),
      subjects(subjects_),
      cabinets(cabinets_)
    {
    for (auto i = subjects_.begin(); i != subjects_.end(); i++)
        subjects.at((*i).first).Reconstruct((*i).second, teachers, groups);
    }

Solution::Solution(const Solution& solution_)
    :
      teachers(solution_.teachers),
      cabinets(solution_.cabinets),
      solution(solution_.solution, teachers, cabinets),
      groups(solution_.groups),
      subjects(solution_.subjects)
    {
        for (auto i = solution_.subjects.begin(); i != solution_.subjects.end(); i++)
            subjects.at((*i).first).Reconstruct((*i).second, teachers, groups);
    }

void Solution::GetRandomSolution() {
    GenerateRandomSchedule(solution, subjects, teachers, groups, cabinets);
}

void Solution::RandomSwap(std::vector<std::string>& subject_names) {
    solution.RandomSwap(subjects, subject_names, cabinets);
}

void Solution::RandomMove(std::vector<std::string>& subject_names) {
    solution.RandomMove(subjects, subject_names, cabinets);
}

TimeTable<Event>& Solution::GetSolution() {
    return solution;
}

Solution::~Solution() {}

class TestSolution {
public:
    int value;

    TestSolution(int& a_);
    TestSolution(int a_);
    TestSolution();
    int& GetSolution();
};

TestSolution::TestSolution(int &a_)
    : value(a_)
{}

TestSolution::TestSolution(int a_)
    : value(a_)
{}

TestSolution::TestSolution()
    : value(0)
{}

int& TestSolution::GetSolution() {
    return value;
}

#endif //TIMETABLE_SOLUTIONCLASS_H
