//
// Created by Gregory Postnikov on 2019-04-13.
//

#ifndef TIMETABLE_ABCCLASS_H
#define TIMETABLE_ABCCLASS_H

template <typename T>
class ArtificialBeeColony() {

    T GetSource() = 0;

    double CostFunction(const T& sample) = 0;

    void ModifySolution(T& solution) = 0;


};


class TimeTableCreator : public ArtificialBeeColony<T>() {

    T GetSource() {

    }

};

#endif //TIMETABLE_ABCCLASS_H
