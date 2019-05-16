//
// Created by Gregory Postnikov on 2019-05-03.
//

#ifndef TIMETABLE_SCHEDULECLASS_H
#define TIMETABLE_SCHEDULECLASS_H


// ##################
// new class
// ##################
template <typename T>
class Schedule {
public:
    std::vector<std::vector<T>> schedule;



    Schedule(size_t days_in_week, size_t pairs_in_day);
    Schedule(Schedule& example, std::map<std::string,Teacher>& teachers, std::map<std::string,Cabinet>& cabinets);
    Schedule(Schedule *example);
    Schedule();
    T& operator[](Time time);
    void Print();
    T& GetElem(Time time);

};

template <typename T>
Schedule<T>::Schedule(size_t days_in_week, size_t pairs_in_day)
    :schedule(days_in_week, std::vector<T> (pairs_in_day))
{}

template <typename T>
Schedule<T>::Schedule(Schedule& example, std::map<std::string,Teacher>& teachers, std::map<std::string,Cabinet>& cabinets)
        :schedule(example.schedule)
{
            for (auto i = schedule.begin(); i != schedule.end(); i++)
                for (auto j = (*i).begin(); j != (*i).end(); j++) {
                    if ((*j).GetTeacher()) {
                        (*j).SetTeacher(&teachers.at((*j).GetTeacher()->GetName()));
                        (*j).SetCabinet(&cabinets.at((*j).GetCabinet()->GetName()));
                    }
                }
}

template <typename T>
Schedule<T>::Schedule(Schedule *example) {

}

template <typename T>
Schedule<T>::Schedule() {}

template <typename T>
void Schedule<T>::Print() {
    for (auto i = schedule.begin(); i != schedule.end(); i++)
        for (auto j = (*i).begin(); j != (*i).end(); j++)
            (*j).PrintIfActive();
}

template <typename T>
T& Schedule<T>::operator[](const Time time) {
    return schedule[time.day][time.pair];
}


template <typename T>
T& Schedule<T>::GetElem(const Time time) {
    return schedule[time.day][time.pair];
}

#endif //TIMETABLE_SCHEDULECLASS_H
