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
};

#endif //TYPES_H
