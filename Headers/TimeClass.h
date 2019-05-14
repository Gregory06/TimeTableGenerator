//
// Created by Gregory Postnikov on 2019-05-03.
//

#ifndef TIMETABLE_TIMECLASS_H
#define TIMETABLE_TIMECLASS_H


// ##################
// new class
// ##################
class Time {
public:

    size_t day;
    size_t pair;

    Time(size_t day_=0, size_t pair_=0);
    Time(const Time &time);
    void Print();
    size_t GetIndex() const;
    Time& operator=(const Time &a);
    Time operator+(int a);

};

Time::Time(size_t day_, size_t pair_)
        : day(day_),
          pair(pair_)
{}

Time::Time(const Time &time) {
    day = time.day;
    pair = time.pair;
}


void Time::Print() {
    std::cout << "DAY " << day << std::endl;
    std::cout << "PAIR " << pair << std::endl;
}

size_t Time::GetIndex() const {
    return day*PAIRS_IN_DAY + pair;
}

Time& Time::operator=(const Time &a) {
    this->day = a.day;
    this->pair = a.pair;

    return *this;
}

Time Time::operator+(int a) {
    return Time(day, pair + a);
}

int64_t Vector2Int(std::vector<Time>& times) {
    int64_t result = 0;
    for (auto i = times.begin(); i != times.end(); i++) {
        assert(((*i).day < DAYS_IN_WEEK) && ((*i).pair < PAIRS_IN_DAY));

        size_t index = (*i).GetIndex();
        if (result & (1 << index))
            std::cout << "Time has allready choosen" << std::endl;
        else
            result |= (1u << index);
    }
    return result;
}

void Int2Vector(std::vector<Time> &result, int64_t times) {
    for (size_t i = 0; i < INT64_SIZE; i++)
        if (times & (1 << i))
            result.push_back(Time(i / PAIRS_IN_DAY, i % PAIRS_IN_DAY));
}

#endif //TIMETABLE_TIMECLASS_H
