#ifndef KODI_COMMANDS_H
#define KODI_COMMANDS_H

#include <fmt/core.h>

#include <unordered_map>
#include <string>
#include <fstream>
#include <ranges>

#include "Types.h"

class Commands {
public:
    class Language {
    public:
        static void
        handle_command(const dpp::slashcommand_t *event, std::unordered_map<std::string, Types::Language> *languages) {
            const std::string option = std::get<std::string>(event->get_parameter("language"));
            auto language = (*languages)[option];

            auto embed = dpp::embed()
                    .set_title(language.title)
                    .set_description(language.description)
                    .set_thumbnail(fmt::format("attachment://{}", language.thumbnail_url))
                    .set_footer(dpp::embed_footer().set_text("Kodi").set_icon("attachment://avatar.png"))
                    .set_color(5793266)
                    .set_timestamp(time(nullptr));

            for (const auto &field: language.fields) {
                embed.add_field(field.name, field.value, false);
            }

            auto msg = dpp::message(event->command.channel_id, embed)
                    .add_file(
                            language.thumbnail_url,
                            dpp::utility::read_file(fmt::format("assets/languages/{}", language.thumbnail_url)))
                    .add_file(
                            "avatar.png",
                            dpp::utility::read_file("assets/avatar.png")
                    );

            event->reply(msg);
        }
    };

public:
    template <typename T>
    static void
    register_command(dpp::cluster *bot, const std::unordered_map<std::string, T> *options, const std::string &name, const std::string &description, const std::string &option_name, const  std::string &option_description) {
        auto command = dpp::slashcommand(name, description, bot->me.id);

        auto kv = std::views::keys(*options);
        const std::vector<std::string> keys{kv.begin(), kv.end()};

        auto option = dpp::command_option(dpp::co_string, option_name, option_description, true);

        for (const auto &key: keys) {
            option.add_choice(dpp::command_option_choice((*options).at(key).title, key));
        }

        command.add_option(option);

        bot->guild_command_create(command, dpp::snowflake(dotenv::env["GUILD_ID"]));
    }
};


#endif //KODI_COMMANDS_H
