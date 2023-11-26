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

    struct Footer {
        std::string text;
        std::string icon_url;
    };

public:
    struct Language {
        std::string title;
        std::string description;
        std::vector<Field> fields;
        std::string thumbnail;
        Footer footer;

        void deserialize(const std::string& input) {
            try {
                nlohmann::json object = nlohmann::json::parse(input);

                title = object.at("title").get<std::string>();
                description = object.at("description").get<std::string>();
                thumbnail = object.at("thumbnail").at("url").get<std::string>();

                const auto&footerJson = object.at("footer");
                footer.text = footerJson.at("text").get<std::string>();
                footer.icon_url = footerJson.at("icon_url").get<std::string>();

                const auto&fieldsJson = object.at("fields");
                fields.clear();
                for (const auto&fieldJson: fieldsJson) {
                    Field field;
                    field.name = fieldJson.at("name").get<std::string>();
                    field.value = fieldJson.at("value").get<std::string>();
                    fields.push_back(field);
                }
            }
            catch (const std::exception&e) {
                std::cerr << "Error parsing JSON: " << e.what() << std::endl;
            }
        }
    };
};

#endif //TYPES_H
