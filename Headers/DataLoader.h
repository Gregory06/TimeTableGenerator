//
// Created by Gregory Postnikov on 2019-04-28.
//

#ifndef TIMETABLE_DATALOADER_H
#define TIMETABLE_DATALOADER_H



namespace DataLoader {

    std::map<std::string, Cabinet>& GetCabinets(std::map<std::string, Cabinet> &cabinets, const std::string filename) {
        std::string name;
        size_t capacity = 0;
        int64_t availible_time = 0;

        std::ifstream file(filename);
        if (!file) {
            std::cout << "ERROR" << std::endl;
            return cabinets;
        }
        size_t pos = 0;
        std::string line;

        while (std::getline(file, line))
        {
            std::vector<Time> times {};
            std::vector<std::string> tokens {};
            while ((pos = line.find(DELIMETER)) != std::string::npos) {
                tokens.push_back(line.substr(0, pos));
                line.erase(0, pos + DELIMETER.length());
            }
            name = tokens[0];
            capacity = (size_t) std::stoi(tokens[1]);

            for (auto i = tokens.begin() + 2; i != tokens.end(); i++) {
                Time time ((size_t) std::stoi(*i) / 10, (size_t) std::stoi(*i) % 10);
                times.push_back(time);
            }

            cabinets.insert(std::pair(name, Cabinet(name, capacity, Vector2Int(times))));

        }
        return cabinets;
    }

    std::map<std::string, Subject>& GetSubjects(std::map<std::string, Subject> &subjects, const std::string &filename,
                                                std::map<std::string, Teacher> &teachers, std::map<std::string,
                                                Group> &groups) {
        std::string name, teacher_name, group_name;
        size_t duration = 0, type = 0;
        std::ifstream file(filename);
        if (!file) {
            std::cout << "ERROR" << std::endl;
            return subjects;
        }
        size_t pos = 0;
        std::string line;

        while (std::getline(file, line))
        {
            std::vector<Group*> involved_groups {};
            std::vector<std::string> tokens {};
            while ((pos = line.find(DELIMETER)) != std::string::npos) {
                tokens.push_back(line.substr(0, pos));
                line.erase(0, pos + DELIMETER.length());
            }
            name = tokens[0];
            duration = (int64_t) std::stoull(tokens[1]);
            type = (int64_t) std::stoull(tokens[2]);
            teacher_name = tokens[3];

            for (auto i = tokens.begin()+4; i != tokens.end(); i++) {
                involved_groups.push_back(&groups.at(*i));
            }
            subjects.insert(std::pair(name, Subject(name, duration, type, &teachers.at(teacher_name), involved_groups)));

        }
        return subjects;
    }

    std::map<std::string, Teacher>& GetTeachers(std::map<std::string, Teacher> &teachers, const std::string filename) {
        std::string name;
        int64_t feasible_time = 0;
        std::ifstream file(filename);
        if (!file) {
            std::cout << "ERROR" << std::endl;
            return teachers;
        }
        size_t pos = 0;
        std::string line;

        while (std::getline(file, line))
        {
            std::vector<Time> times {};
            std::vector<std::string> tokens {};
            while ((pos = line.find(DELIMETER)) != std::string::npos) {
                tokens.push_back(line.substr(0, pos));
                line.erase(0, pos + DELIMETER.length());
            }
            name = tokens[0];

            for (auto i = tokens.begin() + 1; i != tokens.end(); i++) {
                Time time ((size_t) std::stoi(*i) / 10, (size_t) std::stoi(*i) % 10);
                times.push_back(time);
            }

            teachers.insert(std::pair(name, Teacher(name, Vector2Int(times))));

        }
        return teachers;
    }

    std::map<std::string, Group>& GetGroups(std::map<std::string, Group> &groups, const std::string filename) {
        std::string name;
        size_t student_number = 0;
        int64_t feasible_time = 0;
        std::ifstream file(filename);
        if (!file) {
            std::cout << "ERROR" << std::endl;
            return groups;
        }
        size_t pos = 0;
        std::string line;

        while (std::getline(file, line))
        {
            std::vector<Time> times {};
            std::vector<std::string> tokens {};
            while ((pos = line.find(DELIMETER)) != std::string::npos) {
                tokens.push_back(line.substr(0, pos));
                line.erase(0, pos + DELIMETER.length());
            }
            name = tokens[0];
            student_number = (size_t) std::stoi(tokens[1]);

            for (auto i = tokens.begin() + 2; i != tokens.end(); i++) {
                Time time ((size_t) std::stoi(*i) / 10, (size_t) std::stoi(*i) % 10);
                times.push_back(time);
            }

            groups.insert(std::pair(name, Group(name, student_number, Vector2Int(times), DAYS_IN_WEEK, PAIRS_IN_DAY)));

        }
        return groups;
    }

}

#endif //TIMETABLE_DATALOADER_H
