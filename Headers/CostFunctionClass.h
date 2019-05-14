//
// Created by Gregory Postnikov on 2019-05-06.
//

#ifndef TIMETABLE_COSTFUNCTIONCLASS_H
#define TIMETABLE_COSTFUNCTIONCLASS_H

class CostFunction {
public:
    double Count(TimeTable<Event> timetable);
};

double CostFunction::Count(TimeTable <Event> timetable) {
    return 10;
}

class TestCostFunction {
public:
    double Count(double a);
};

double TestCostFunction::Count(double a) {
    return 0.1*a*a*a*a-a*a*a+3*a*a-3*a + 10;
}

#endif //TIMETABLE_COSTFUNCTIONCLASS_H
