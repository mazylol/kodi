#include "Types.h"

#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

#include <nlohmann/json.hpp>

void Types::Language::deserialize(const std::string &input) {
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
    } catch (const std::exception &e) {
        std::cerr << "Error parsing JSON: " << e.what() << std::endl;
    }
}

void Types::Person::deserialize(const std::string &input) {
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
    } catch (const std::exception &e) {
        std::cerr << "Error parsing JSON: " << e.what() << std::endl;
    }
}

template<typename T>
std::unordered_map<std::string, T> Types::load(const std::string &path) {
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

template std::unordered_map<std::string, Types::Language> Types::load(const std::string &path);

template std::unordered_map<std::string, Types::Person> Types::load(const std::string &path);