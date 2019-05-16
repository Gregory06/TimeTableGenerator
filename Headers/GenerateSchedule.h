//
// Created by Gregory Postnikov on 2019-04-27.
//

#ifndef TIMETABLE_GENERATESCHEDULE_H
#define TIMETABLE_GENERATESCHEDULE_H

#include "SubjectClass.h"
#include "TeacherClass.h"
#include "GroupClass.h"
#include "EventClass.h"
#include "Constants.h"


Cabinet* GetFeasibleCabinet(std::map<std::string,Cabinet>& cabinets, Time start_time, size_t duration, size_t total_participants) {
    Cabinet* fesible_cabinet = nullptr;
    for (auto i = cabinets.begin(); i != cabinets.end(); i++) {
        if ((*i).second.GetCapacity() < total_participants)
            continue;
        if (!(*i).second.IsFeasible(start_time, duration))
            continue;
        fesible_cabinet = &(*i).second;
        break;
    }
    return fesible_cabinet;
}

template <typename T>
void RandomPermutation(std::vector<T> &vector) {
    size_t permutations_count = vector.size();
    T helper;
    for (size_t i = 0; i < permutations_count; i++) {
        size_t first_index = random() % permutations_count;
        size_t second_index = random() % permutations_count;
        helper = vector[first_index];
        vector[first_index] = vector[second_index];
        vector[second_index] = helper;
    }
}

#include "TimeTableClass.h"

void DeleteEvent(TimeTable<Event>& timetable, Time time, std::vector<Group*> &groups) {
    Event &event(timetable[(*groups.begin())->GetName()]->GetElem(time));
    event.Deactivate();
    for (size_t i = 0; i < event.GetDuration(); i++) {
        for (auto group = groups.begin(); group != groups.end(); group++)
            (*group)->ReleaseTime(event.GetStartTime()+i);
        event.GetTeacher()->ReleaseTime(event.GetStartTime()+i);
        event.GetCabinet()->ReleaseTime(event.GetStartTime()+i);
    }
}

TimeTable<Event>& GenerateRandomSchedule(TimeTable<Event>& timetable, std::map<std::string,Subject>& subjects,
                            std::map<std::string,Teacher>& teachers, std::map<std::string,Group>& groups,
                            std::map<std::string,Cabinet>& cabinets) {

    SubjectStorage subject_storage;
    subject_storage.FillStorage(subjects);

    bool prev_place_founded(true);
    std::vector<std::vector<Time>> times_stack {};
    Subject *current_subject = nullptr;
    Time start_time;
    while (!subject_storage.QueueEmpty()) {
        std::srand(time(NULL));
//        std::cout << "_______" << std::endl;
//        subject_storage.StorageSize();
        if (prev_place_founded) {
            current_subject = subject_storage.QueueGetMin();
            subject_storage.MoveMinToStack();
            int64_t feasible_time = current_subject->GetResultingFeasibleTime();
            std::vector<Time> shufled_fesible_time {};
            Int2Vector(shufled_fesible_time, feasible_time);
            RandomPermutation(shufled_fesible_time);
            times_stack.push_back(shufled_fesible_time);
        } else {
            subject_storage.MoveTopToQueue();
            times_stack.pop_back();
            current_subject = subject_storage.StackGetTop();
            std::vector<Group *> groups(current_subject->GetGroups());
            DeleteEvent(timetable, times_stack.back().back(), groups);
            times_stack.back().pop_back();
        }

        while (!times_stack.back().empty()) {
            start_time = times_stack.back().back();
            Cabinet *feasible_cabinet = GetFeasibleCabinet(cabinets, start_time, current_subject->GetDuration(),
                                                           current_subject->GetParticipantsNumber());

            if (!feasible_cabinet) {
                times_stack.back().pop_back();
                continue;
            }

            std::vector<Group *> groups(current_subject->GetGroups());
            timetable.InsertEvent(current_subject->GetName(), current_subject->GetDuration(),
                                  current_subject->GetType(), current_subject->GetTeacher(),
                                  groups, start_time, feasible_cabinet);

            prev_place_founded = true;

            break;
        }
        if (times_stack.back().empty())
            prev_place_founded = false;
        if (subject_storage.StackEmpty()) {
            std::cout << "GENERATION FAIL" << std::endl;
            return timetable;
        }
    }
    return timetable;
}

#endif //TIMETABLE_GENERATESCHEDULE_H
