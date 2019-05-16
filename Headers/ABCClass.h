//
// Created by Gregory Postnikov on 2019-04-13.
//

#ifndef TIMETABLE_ABCCLASS_H
#define TIMETABLE_ABCCLASS_H

#include <list>
#include "EventClass.h"
#include "TimeTableClass.h"
#include "CostFunctionClass.h"
#include "SolutionClass.h"

template <typename T, typename F>
class ArtificialBeeColony {
protected:
    std::list<std::pair<T*, size_t>> solutions {};
    std::pair<T, size_t> current_best_solution;

    size_t population_size;
    size_t maximum_cycle_number;
    size_t single_source_limit;

    F cost_function;

    virtual T* GetSource() = 0;
    virtual void ModifySolution(T& solution) = 0;
    virtual T* Construct(T& example) = 0;

    void SendScouts();
    void SendEmploedBees();
    void SendOnlookerBees();
    double ValuesSum();
    void ManageSource(typename std::list<std::pair<T*,size_t>>::iterator source);

public:

    ArtificialBeeColony(size_t population_size, size_t maximum_cycle_number, size_t single_source_limit);

    T FindOptimal();

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
    while (solutions.size() < population_size) {
        std::cout << "Generating" << std::endl;
        solutions.push_back(std::pair(GetSource(), 0));
        std::cout << "Generated" << std::endl;

    }
}

template <typename T, typename F>
void ArtificialBeeColony<T,F>::SendEmploedBees() {
    for (auto i = solutions.begin(); i != solutions.end(); i++) {
        ManageSource(i);
        std::cout << "DONE " << (*i).second << std::endl;

    }
}

template <typename T, typename F>
void ArtificialBeeColony<T,F>::SendOnlookerBees() {
    double sum = ValuesSum();
    for (size_t i = 0; i < population_size; i++) {
        for (auto j = solutions.begin(); j != solutions.end(); j++) {
            size_t rand = (size_t) random() % 1000;
            if (rand < (cost_function.Count((*j).first->GetSolution())/sum*1000) ) {
                ManageSource(j);
                break;
            }
//            if (j == solutions.begin()+solutions.size())
//                ManageSource(j);
        }
    }
}

template <typename T, typename F>
T ArtificialBeeColony<T,F>::FindOptimal() {

    for (size_t j = 0; j < maximum_cycle_number; j++) {

        std::cout << "HI1" << std::endl;
        SendScouts();
        std::cout << "HI2" << std::endl;
        SendEmploedBees();
        std::cout << "HI3" << std::endl;
        SendOnlookerBees();

    }

//    for (auto i = solutions.begin(); i != solutions.end(); i++) {
//        std::cout << " ____________________" << std::endl;
//        std::cout << " ____________________" << std::endl;
//        std::cout << " ____________________" << std::endl;
//        (*i).first->GetSolution().Print();
//        std::cout << " ____________________" << std::endl;
//        std::cout << " ____________________" << std::endl;
//        std::cout << " ____________________" << std::endl;
//    }

    std::cout << "GET IT!!" <<std::endl;
    std::cout << "THe BEsT ScORE is " << current_best_solution.second << std::endl;
    current_best_solution.first->GetSolution().Print();
    return current_best_solution.first;
}

template <typename T, typename F>
double ArtificialBeeColony<T,F>::ValuesSum() {
    double sum = 0;
    for (auto i = solutions.begin(); i != solutions.end(); i++)
        sum += cost_function.Count((*i).first->GetSolution());

    return sum;
}

template <typename T, typename F>
void ArtificialBeeColony<T,F>::ManageSource(typename std::list<std::pair<T*, size_t>>::iterator source) {
    auto new_solution = new Solution(*(*source).first);
//    auto new_solution = Construct(*(*source).first);
//    new_solution->GetSolution().Print();

//    new_solution->GetSolution().Print();
//    delete new_solution;
    std::cout << "FIND NEW SOL" << std::endl;
    ModifySolution(*new_solution);

    size_t current_score = cost_function.Count(new_solution->GetSolution());
    if (current_score < cost_function.Count((*source).first->GetSolution())) {
        std::cout << "1" << std::endl;

//        delete (*source).first;
        (*source).first = new_solution;
        (*source).second = 0;
    } else {
        std::cout << "2" << std::endl;

        delete new_solution;
        (*source).second++;
    }
    if (cost_function.Count((*source).first->GetSolution()) < current_best_solution.second) {
        current_best_solution.first = *(*source).first;
        current_best_solution.second = cost_function.Count((*source).first->GetSolution());
    }
    if ((*source).second > single_source_limit) {
        std::cout << "3" << std::endl;

        delete (*source).first;
        solutions.erase(source);
    }

}

class TestABC : public ArtificialBeeColony<TestSolution, TestCostFunction> {

    TestSolution* GetSource() override;
    void ModifySolution(TestSolution& a) override;
    TestSolution* Construct(TestSolution& a) override;

public:

    TestABC(size_t population_size, size_t maximum_cycle_number, size_t single_source_limit);

};

TestABC::TestABC(size_t population_size, size_t maximum_cycle_number, size_t single_source_limit)
    : ArtificialBeeColony<TestSolution, TestCostFunction>( population_size, maximum_cycle_number,
                                                  single_source_limit)
{}

TestSolution* TestABC::GetSource() {
    auto a = new TestSolution(random() % 200 - 100);
    return a;
}

void TestABC::ModifySolution(TestSolution& a) {
    a.value += random() % 2 - 1;
}

TestSolution* TestABC::Construct(TestSolution &a) {
    return new TestSolution(a);
}

class TimeTableCreator : public ArtificialBeeColony<Solution,CostFunction> {

    std::map<std::string,Subject>& subjects;
    std::map<std::string,Teacher>& teachers;
    std::map<std::string,Group>& groups;
    std::map<std::string,Cabinet>& cabinets;

    std::vector<std::string> group_names;
    std::vector<std::string> subject_names;

    Solution* GetSource() override;
    void ModifySolution(Solution& solution) override;
    Solution* Construct(Solution& example) override;

public:

    TimeTableCreator(size_t population_size, size_t maximum_cycle_number, size_t single_source_limit,
                     std::map<std::string,Subject>& subjects_, std::map<std::string,Teacher>& teachers_,
                     std::map<std::string,Group>& groups_, std::map<std::string,Cabinet>& cabinets_);

};

TimeTableCreator::TimeTableCreator(size_t population_size, size_t maximum_cycle_number, size_t single_source_limit,
                                   std::map<std::string,Subject>& subjects_, std::map<std::string,Teacher>& teachers_,
                                   std::map<std::string,Group>& groups_, std::map<std::string,Cabinet>& cabinets_)
    : ArtificialBeeColony<Solution,CostFunction>(population_size, maximum_cycle_number, single_source_limit),
    subjects(subjects_),
    teachers(teachers_),
    groups(groups_),
    cabinets(cabinets_)
{
    for (auto group = groups.begin(); group != groups.end(); group++)
        group_names.push_back((*group).first);
    for (auto subject = subjects.begin(); subject != subjects.end(); subject++)
        subject_names.push_back((*subject).first);
}

Solution* TimeTableCreator::GetSource() {
    Solution* solution = new Solution(group_names, DAYS_IN_WEEK, PAIRS_IN_DAY,
                                      subjects, teachers, groups, cabinets);
    solution->GetRandomSolution();
    return solution;
}

void TimeTableCreator::ModifySolution(Solution& solution) {
    size_t rand = random() % 100;
    if (rand > 70) {
        solution.RandomSwap(subject_names);
    }
    else {
        solution.RandomMove(subject_names);
    }
}

Solution* TimeTableCreator::Construct(Solution& example) {
    return new Solution(example);
}

#endif //TIMETABLE_ABCCLASS_H
