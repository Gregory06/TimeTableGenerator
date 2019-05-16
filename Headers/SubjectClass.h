//
// Created by Gregory Postnikov on 2019-04-16.
//

#ifndef TIMETABLE_SUBJECTCLASS_H
#define TIMETABLE_SUBJECTCLASS_H

#include <vector>
#include <set>
#include <map>
#include "TeacherClass.h"
#include "GroupClass.h"

// ##################
// new class
// ##################
class Subject {

    std::string name;
    size_t duration;
    size_t type; // 0 -- семинар, 1 -- лекция
    Teacher* teacher;
    std::vector<Group*> groups;
    size_t total_participants;

public:

    Subject(std::string name_, size_t duration_, size_t type_, Teacher* teacher_, std::vector<Group*> &groups_);

    size_t GetTeacherTimeLen() const;
    int64_t GetResultingFeasibleTime() const;
    void Print() const;
    size_t GetDuration() const;
    size_t GetParticipantsNumber() const;
    std::string GetName() const;
    Teacher* GetTeacher() const;
    std::vector<Group*> GetGroups() const;
    size_t GetType() const;
    void Reconstruct(const Subject& example, std::map<std::string,Teacher>& teachers_, std::map<std::string,Group>& groups_);

};

Subject::Subject(std::string name_, size_t duration_, size_t type_, Teacher* teacher_, std::vector<Group*> &groups_)
        : name(name_),
          type(type_),
          teacher(teacher_),
          duration(duration_) {

    total_participants = 0;
    for (auto i = groups_.begin(); i != groups_.end(); i++) {
        groups.push_back(*i);
        total_participants += (*i)->GetStudentNumber();
    }

}

size_t Subject::GetTeacherTimeLen() const {
    return teacher->GetAvailibleTimeLen();
}

int64_t Subject::GetResultingFeasibleTime() const { // Возвражает допустимое время НАЧАЛА события, с учетом продолжительности
    int64_t result_feasible_time = teacher->GetFeasibleTime();
    for (auto i = groups.begin(); i != groups.end(); i++)
        result_feasible_time &= (*i)->GetFeasibleTime();

    for (size_t i = 0; i < INT64_SIZE; i++) {
        if (result_feasible_time & (1 << i))
            for (size_t j = 1; j < duration; j++)
                if (!(result_feasible_time & (1 << (i+j))) || ((i+j) % PAIRS_IN_DAY == 0)) {
                    result_feasible_time &= ~(1 << i);
                    break;
                }
    }

    return result_feasible_time;
}

void Subject::Print() const {
    std::cout << "##########" << std::endl;
    std::cout << "### Subject " << name << std::endl;
    std::cout << "### with duration of " << duration << std::endl;
    std::cout << "### and total participants " << total_participants << std::endl;
    teacher->Print();
    for (auto i = groups.begin(); i != groups.end(); i++)
        (*i)->Print();
    std::cout << "##########" << std::endl;
}

size_t Subject::GetDuration() const {
    return duration;
}

size_t Subject::GetParticipantsNumber() const {
    return total_participants;
}

std::string Subject::GetName() const {
    return name;
}

Teacher* Subject::GetTeacher() const {
    return teacher;
}

std::vector<Group*> Subject::GetGroups() const {
    return groups;
}

size_t Subject::GetType() const {
    return type;
}

void Subject::Reconstruct(const Subject& example, std::map<std::string,Teacher>& teachers_,
                          std::map<std::string,Group>& groups_) {
    name = example.name;
    duration = example.duration;
    total_participants = example.total_participants;
    teacher = &teachers_.at(example.teacher->GetName());
    groups.clear();
    for (auto i = example.groups.begin(); i != example.groups.end(); i++)
        groups.push_back(&groups_.at((*i)->GetName()));
}

// ##################
// new helper class
// ##################
struct SubjectComporator {
    bool operator() (const Subject* a, const Subject* b) {
            return a->GetTeacherTimeLen() < b->GetTeacherTimeLen();
    }
};

// ##################
// new class
// ##################
class SubjectStorage { // ВАЖНО: реализация класса специально не позволяет проводить операции, нарушающие целостность хранилища,
                       // то есть возможен только перенос из очереди в стек или обратно.

    std::vector<Subject*> stack;
    std::multiset<Subject*, SubjectComporator> priority_queue;

    void QueueInsert(Subject* obj);
    void QueueEraiseMin();
    void StackPop();

public:

    bool QueueEmpty() const;
    bool StackEmpty() const;
    void FillStorage(std::map<std::string, Subject>& subjects);
    Subject* QueueGetMin() const;
    void MoveMinToStack();
    Subject* StackGetTop();
    void MoveTopToQueue();
    void StorajeDump();
    void StorageSize();

};

bool SubjectStorage::QueueEmpty() const {
    return priority_queue.empty();
}

bool SubjectStorage::StackEmpty() const {
    return stack.empty();
}

void SubjectStorage::QueueInsert(Subject* obj) {
    priority_queue.insert(obj);
}

void SubjectStorage::QueueEraiseMin() {
    priority_queue.erase(priority_queue.begin());
}

Subject* SubjectStorage::QueueGetMin() const {
    return *priority_queue.begin();
}

void SubjectStorage::FillStorage(std::map<std::string, Subject>& subjects) {
    for (auto subject = subjects.begin(); subject != subjects.end(); subject++)
        QueueInsert(&(*subject).second);
}

void SubjectStorage::MoveMinToStack() {
    stack.push_back(QueueGetMin());
    QueueEraiseMin();
}

void SubjectStorage::MoveTopToQueue() {
    priority_queue.insert(StackGetTop());
    StackPop();
}

Subject* SubjectStorage::StackGetTop() {
    return stack.back();
}

void SubjectStorage::StackPop() {
    stack.pop_back();
}

void SubjectStorage::StorajeDump() {
    std::cout << "Now in queue: " << std::endl;
    for (auto i = priority_queue.begin(); i != priority_queue.end(); i++)
        (*i)->Print();
    std::cout << "Now in stack: " << std::endl;
    for (auto i = stack.begin(); i != stack.end(); i++)
        (*i)->Print();
}

void SubjectStorage::StorageSize() {
    std::cout << "Queue size: " ;
    std::cout << priority_queue.size()<< std::endl;
    std::cout << "Stack size: " ;
    std::cout << stack.size()<< std::endl;
}

#endif //TIMETABLE_SUBJECTCLASS_H
