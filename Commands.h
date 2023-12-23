#ifndef KODI_CMDS_H
#define KODI_CMDS_H

#include <dpp/dpp.h>

#include <dotenv.h>

#include <unordered_map>

#include "Types.h"

namespace Commands {
    void handle_language_command(const dpp::slashcommand_t *event, std::unordered_map<std::string, Types::Language> *languages);
    void handle_person_command(const dpp::slashcommand_t *event, std::unordered_map<std::string, Types::Person> *people);

    template <typename T>
    void register_command(dpp::cluster *bot, const std::unordered_map<std::string, T> *options, const std::string &name,
                          const std::string &description, const std::string &option_name,
                          const std::string &option_description) {
        auto command = dpp::slashcommand(name, description, bot->me.id);

        auto kv = std::views::keys(*options);
        const std::vector<std::string> keys{kv.begin(), kv.end()};

        auto option = dpp::command_option(dpp::co_string, option_name, option_description, true);

        for (const auto &key : keys) {
            option.add_choice(dpp::command_option_choice((*options).at(key).title, key));
        }

        command.add_option(option);

        bot->guild_command_create(command, dpp::snowflake(dotenv::env["GUILD_ID"]));
    }
}

#endif