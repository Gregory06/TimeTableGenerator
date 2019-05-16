//
// Created by Gregory Postnikov on 2019-05-06.
//

#ifndef TIMETABLE_COSTFUNCTIONCLASS_H
#define TIMETABLE_COSTFUNCTIONCLASS_H

class CostFunction {
public:
    double Count(TimeTable<Event>& timetable);
};

double CostFunction::Count(TimeTable<Event>& timetable) {
    size_t cost(0);

    for (auto group = timetable.GetTable().begin(); group != timetable.GetTable().end(); group++)
        for (size_t day = 0; day < DAYS_IN_WEEK; day++)
            for (size_t pair = 0; pair < PAIRS_IN_DAY; pair++)

                if ((*group).second->GetElem(Time(day,pair)).IsActive())
                    if ((*group).second->GetElem(Time(day,pair)).GetType() == LECTURE)
                        cost+=2;
                    else
                        cost++;

    return cost;
}

class TestCostFunction {
public:
    double Count(double a);
};

double TestCostFunction::Count(double a) {
    return 0.1*a*a*a*a-a*a*a+3*a*a-3*a + 10;
}

#endif //TIMETABLE_COSTFUNCTIONCLASS_H
