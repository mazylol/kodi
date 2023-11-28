#ifndef TYPES_H
#define TYPES_H

#include <iostream>
#include <string>
#include <vector>

#include <nlohmann/json.hpp>

class Types {
    struct Field {
        std::string name;
        std::string value;
    };

public:
    struct Language {
        std::string title;
        std::string description;
        std::vector<Field> fields;
        std::string thumbnail_url;

        void deserialize(const std::string &input) {
            try {
                nlohmann::json object = nlohmann::json::parse(input);

                title = object.at("title").get<std::string>();
                description = object.at("description").get<std::string>();
                thumbnail_url = object.at("thumbnail_url").get<std::string>();

                const auto &fieldsJson = object.at("fields");
                fields.clear();
                for (const auto &fieldJson: fieldsJson) {
                    Field field;
                    field.name = fieldJson.at("name").get<std::string>();
                    field.value = fieldJson.at("value").get<std::string>();
                    fields.push_back(field);
                }
            }
            catch (const std::exception &e) {
                std::cerr << "Error parsing JSON: " << e.what() << std::endl;
            }
        }
    };

public:
    template<typename T>
    static std::unordered_map<std::string, T> load(const std::string &path) {
        std::unordered_map<std::string, T> items_map;

        for (const auto &entry: std::filesystem::directory_iterator(
                path)) {
            std::ifstream f(entry.path());

            if (f.is_open()) {
                std::string str((std::istreambuf_iterator(f)), std::istreambuf_iterator<char>());

                T type;
                type.deserialize(str);

                const std::filesystem::path &filePath = entry.path();
                std::string fileName = filePath.stem().string();
                items_map[fileName] = type;
            } else {
                std::cerr << "Error opening file: " << entry.path() << std::endl;
                continue;
            }
        }

        return items_map;
    }
};

#endif //TYPES_H
