//
// Created by Gregory Postnikov on 2019-04-28.
//

#ifndef TIMETABLE_KEYACCESSSTORAGE_H
#define TIMETABLE_KEYACCESSSTORAGE_H

template <typename T>
class KeyAccessStorage {

    std::map<std::string, T> storage;

public:

    T operator[](const std::string key);

};

template <typename T>
T KeyAccessStorage<T>::operator[](const std::string key) {
    return storage.at(key);
}



#endif //TIMETABLE_KEYACCESSSTORAGE_H
