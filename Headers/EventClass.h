//
// Created by Gregory Postnikov on 2019-04-13.
//

#ifndef TIMETABLE_EVENTCLASS_H
#define TIMETABLE_EVENTCLASS_H

#include "TeacherClass.h"
#include "TimeClass.h"
#include "CabinetClass.h"

// ##################
// new class
// ##################
class Event {

    std::string name;
    size_t duration;
    size_t type;
    Teacher *teacher;
    Time start_time;
    Cabinet *cabinet;
    bool is_active;

public:

    Event();
    Event(std::string name_, size_t duration_, size_t type, Teacher *teacher_, Time start_time_, Cabinet *cabinet_);
    void Print();
    Event& operator=(const Event& event);
    bool operator==(Event& event);
    std::string GetName() const;
    size_t GetDuration() const;
    Teacher* GetTeacher() const;
    void SetTeacher(Teacher* teacher_);
    Time& GetStartTime();
    Cabinet* GetCabinet() const;
    size_t GetType() const;
    void SetCabinet(Cabinet* cabinet_);
    void AssignCabinet(Cabinet *cab);
    bool IsActive() const;
    void Deactivate();
    void AssignEvent(std::string name_, size_t duration_, Teacher *teacher_, Time start_time_, Cabinet *cabinet_);
    void PrintIfActive();
    void Compare(Event &b);
};

Event::Event()
    : name(""),
    duration(0),
    teacher(nullptr),
    start_time(0,0),
    cabinet(nullptr),
    is_active(false)
{}

Event::Event(std::string name_, size_t duration_, size_t type_, Teacher *teacher_, Time start_time_, Cabinet *cabinet_)
    : name(name_),
    duration(duration_),
    type(type_),
    teacher(teacher_),
    start_time(start_time_),
    cabinet(cabinet_),
    is_active(true)
{}

void Event::Print() {

    std::cout << "##########" << std::endl;
    std::cout << "Event " << name << std::endl;
    std::cout << "starts at ";
    start_time.Print();
    std::cout << "of length " << duration << std::endl;
    if (!is_active)
        std::cout << "NOT ";
    std::cout << "ACTIVE" << std::endl;
}

void Event::PrintIfActive() {

    if (is_active) {
        std::cout << "##########" << std::endl;
        std::cout << "Event " << name << std::endl;
        std::cout << "starts at ";
        start_time.Print();
        std::cout << "of length " << duration << std::endl;
    }
}

Event& Event::operator=(const Event& event) {
    name = event.name;
    duration = event.duration;
    type = event.type;
    teacher = event.teacher;
    start_time = event.start_time;
    is_active = event.is_active;
    cabinet = event.cabinet;

    return *this;
}

std::string Event::GetName() const {
    return name;
}

size_t Event::GetDuration() const {
    return duration;
}

Teacher* Event::GetTeacher() const {
    return teacher;
}

void Event::SetTeacher(Teacher* teacher_) {
    teacher = teacher_;
}

Time& Event::GetStartTime() {
    return start_time;
}

Cabinet* Event::GetCabinet() const {
    return cabinet;
}

size_t Event::GetType() const {
    return type;
}

void Event::SetCabinet(Cabinet *cabinet_) {
    cabinet = cabinet_;
}

void Event::AssignCabinet(Cabinet *cab) {
    cabinet = cab;
}

bool Event::IsActive() const {
    return is_active;
}

void Event::Deactivate() {
    is_active = false;
}

void Event::AssignEvent(std::string name_, size_t duration_, Teacher *teacher_, Time start_time_, Cabinet *cabinet_) {
    name = name_;
    duration = duration_;
    teacher = teacher_;
    start_time = start_time_;
    cabinet = cabinet_;
}

bool Event::operator==(Event& event) {
    if (name != event.name)
        return false;
    if (duration != event.duration)
        return false;
    if (teacher != event.teacher)
        return false;

    return true;
}

void Event::Compare(Event &b) {
    if (*this == b)
        return;
    std::cout << std::endl;
    std::cout << std::endl;
    Print();
    std::cout << std::endl;
    b.Print();
    std::cout << std::endl;
    std::cout << std::endl;

}

#endif //TIMETABLE_EVENTCLASS_H
