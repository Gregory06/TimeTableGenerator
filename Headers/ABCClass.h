//
// Created by Gregory Postnikov on 2019-04-13.
//

#ifndef TIMETABLE_ABCCLASS_H
#define TIMETABLE_ABCCLASS_H

#include <list>
#include "EventClass.h"
#include "TimeTableClass.h"
#include "CostFunctionClass.h"

template <typename T, typename F>
class ArtificialBeeColony {

    std::list<std::pair<T, size_t>> solutions {};
    std::pair<T, size_t> current_best_solution;

    size_t population_size;
    size_t maximum_cycle_number;
    size_t single_source_limit;

    F cost_function;

    virtual T& GetSource(T &exemplar) = 0;
    virtual T ModifySolution(T& solution) = 0;
    void SendScouts();
    void SendEmploedBees();
    void SendOnlookerBees();
    double ValuesSum();
    void ManageSource(typename std::list<std::pair<T,size_t>>::iterator source);

public:

    ArtificialBeeColony(size_t population_size, size_t maximum_cycle_number, size_t single_source_limit);

    T& FindOptimal(T &element);

};

template <typename T, typename F>
ArtificialBeeColony<T,F>::ArtificialBeeColony(size_t population_size_, size_t maximum_cycle_number_,
                                            size_t single_source_limit_)
                                            : population_size(population_size_),
                                            maximum_cycle_number(maximum_cycle_number_),
                                            single_source_limit(single_source_limit_) {
                                                current_best_solution.second = 100000;
                                            }

template <typename T, typename F>
void ArtificialBeeColony<T,F>::SendScouts() {
    if (solutions.size() < population_size)
        while (solutions.size() != population_size) {
            T *new_elem = new T;
            T &new_elem_ref = *new_elem;
            solutions.push_back(std::pair(GetSource(new_elem_ref), 0));
        }
}

template <typename T, typename F>
void ArtificialBeeColony<T,F>::SendEmploedBees() {
    for (auto i = solutions.begin(); i != solutions.end(); i++)
        ManageSource(i);
}

template <typename T, typename F>
void ArtificialBeeColony<T,F>::SendOnlookerBees() {
    double sum = ValuesSum();
    for (size_t i = 0; i < population_size; i++) {
        for (auto j = solutions.begin(); j != solutions.end(); j++) {
            size_t rand = (size_t) random() % 1000;
            if (rand < (cost_function.Count((*j).first)/sum*1000) ) {
                ManageSource(j);
                break;
            }
//            if (j == solutions.begin()+solutions.size())
//                ManageSource(j);
        }
    }
}

template <typename T, typename F>
T& ArtificialBeeColony<T,F>::FindOptimal(T &element) {

    for (size_t j = 0; j < maximum_cycle_number; j++) {

        std::cout << "HI1" << std::endl;
        SendScouts();
        std::cout << "HI2" << std::endl;
        SendEmploedBees();
        std::cout << "HI3" << std::endl;
        SendOnlookerBees();

    }

    element = current_best_solution.first;
    return element;
}

template <typename T, typename F>
double ArtificialBeeColony<T,F>::ValuesSum() {
    double sum = 0;
    for (auto i = solutions.begin(); i != solutions.end(); i++)
        sum += cost_function.Count((*i).first);

    return sum;
}

template <typename T, typename F>
void ArtificialBeeColony<T,F>::ManageSource(typename std::list<std::pair<T, size_t>>::iterator source) {
    T new_solution = ModifySolution((*source).first);
    size_t current_score = cost_function.Count(new_solution);
    if (current_score < cost_function.Count((*source).first)) {
        (*source).first = new_solution;
        (*source).second = 0;
    } else
        (*source).second++;
    if ((*source).second > single_source_limit) {
//        delete source.first;
        solutions.erase(source);
    }
    if (cost_function.Count((*source).first) < current_best_solution.second) {
        current_best_solution.first = (*source).first;
        current_best_solution.second = cost_function.Count((*source).first);
    }
}

class TestABC : public ArtificialBeeColony<int, TestCostFunction> {

    int& GetSource(int& a) override;
    int ModifySolution(int& a) override;

public:

    TestABC(size_t population_size, size_t maximum_cycle_number, size_t single_source_limit);

};

TestABC::TestABC(size_t population_size, size_t maximum_cycle_number, size_t single_source_limit)
    : ArtificialBeeColony<int, TestCostFunction>( population_size, maximum_cycle_number,
                                                  single_source_limit)
{}

int& TestABC::GetSource(int& a) {
    a = random() % 200 - 100;
    return a;
}

int TestABC::ModifySolution(int& a) {
    int b(0);
    if (random() % 2)
        b = 1;
    else
        b = -1;
    return a+b;
}

class TimeTableCreator : public ArtificialBeeColony<TimeTable<Event>,CostFunction> {

    std::map<std::string,Subject>& subjects;
    std::map<std::string,Teacher>& teachers;
    std::map<std::string,Group>& groups;
    std::map<std::string,Cabinet>& cabinets;

    std::vector<std::string> subject_names;

    TimeTable<Event>& GetSource(TimeTable<Event>& timetable) override;
    TimeTable<Event> ModifySolution(TimeTable<Event>& solution) override;

public:

    TimeTableCreator(size_t population_size, size_t maximum_cycle_number, size_t single_source_limit,
                     std::map<std::string,Subject>& subjects_, std::map<std::string,Teacher>& teachers_,
                     std::map<std::string,Group>& groups_, std::map<std::string,Cabinet>& cabinets_);

};

TimeTableCreator::TimeTableCreator(size_t population_size, size_t maximum_cycle_number, size_t single_source_limit,
                                   std::map<std::string,Subject>& subjects_, std::map<std::string,Teacher>& teachers_,
                                   std::map<std::string,Group>& groups_, std::map<std::string,Cabinet>& cabinets_)
    : ArtificialBeeColony<TimeTable<Event>,CostFunction>(population_size, maximum_cycle_number, single_source_limit),
    subjects(subjects_),
    teachers(teachers_),
    groups(groups_),
    cabinets(cabinets_)
{
    for (auto subject = subjects.begin(); subject != subjects.end(); subject++)
        subject_names.push_back((*subject).first);
}

TimeTable<Event>& TimeTableCreator::GetSource(TimeTable<Event>& timetable) {
    return GenerateRandomSchedule(timetable, subjects, teachers, groups, cabinets);
}

TimeTable<Event> TimeTableCreator::ModifySolution(TimeTable<Event>& solution) {
    size_t rand = random() % 100;
    if (rand > 70) {
        solution.RandomSwap(subjects, subject_names, cabinets);
    }
    else {

    }

    return solution;
}

#endif //TIMETABLE_ABCCLASS_H
