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
    Teacher *teacher;
    Time start_time;
    Cabinet *cabinet;
    bool is_active;

public:

    Event();
    Event(std::string name_, size_t duration_, Teacher *teacher_, Time start_time_, Cabinet *cabinet_);
    void Print();
    Event& operator=(Event event);
    size_t GetDuration() const;
    Teacher* GetTeacher() const;
    Time GetStartTime() const;
    Cabinet* GetCabinet() const;
    void Deactivate();
    void AssignEvent(std::string name_, size_t duration_, Teacher *teacher_, Time start_time_, Cabinet *cabinet_);
    void PrintIfActive();
};

Event::Event()
    : name(""),
    duration(0),
    teacher(nullptr),
    start_time(0,0),
    cabinet(nullptr),
    is_active(false)
{}

Event::Event(std::string name_, size_t duration_, Teacher *teacher_, Time start_time_, Cabinet *cabinet_)
    : name(name_),
    duration(duration_),
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

Event& Event::operator=(const Event event) {
    name = event.name;
    duration = event.duration;
    teacher = event.teacher;
    start_time = event.start_time;
    cabinet = event.cabinet;
    is_active = event.is_active;

    return *this;
}

size_t Event::GetDuration() const {
    return duration;
}

Teacher* Event::GetTeacher() const {
    return teacher;
}

Time Event::GetStartTime() const {
    return start_time;
}

Cabinet* Event::GetCabinet() const {
    return cabinet;
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

#endif //TIMETABLE_EVENTCLASS_H
