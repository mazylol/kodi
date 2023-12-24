#ifndef TYPES_H
#define TYPES_H

#include <string>
#include <unordered_map>
#include <vector>

namespace Types {
    struct Field {
        std::string name;
        std::string value;
    };

    struct Language {
        std::string title;
        std::string description;
        std::vector<Field> fields;
        std::string thumbnail_url;

        void deserialize(const std::string &input);
    };

    struct Person {
        std::string title;
        std::string description;
        std::vector<Field> fields;
        std::string thumbnail_url;

        void deserialize(const std::string &input);
    };

    template<typename T>
    std::unordered_map<std::string, T> load(const std::string &path);
};

#endif
