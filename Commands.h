#ifndef KODI_COMMANDS_H
#define KODI_COMMANDS_H

#include <dpp/dpp.h>

#include <unordered_map>

#include "Types.h"

namespace Commands {
    void handle_language_command(const dpp::slashcommand_t *event, std::unordered_map<std::string, Types::Language> *languages);
    void handle_person_command(const dpp::slashcommand_t *event, std::unordered_map<std::string, Types::Person> *people);

    template <typename T>
    void register_command(dpp::cluster *bot, std::string *guildId, bool prod, const std::unordered_map<std::string, T> *options, const std::string &name,
                          const std::string &description, const std::string &option_name,
                          const std::string &option_description);
}

#endif