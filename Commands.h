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
        static std::unordered_map<std::string, Types::Language> load_languages() {
            std::unordered_map<std::string, Types::Language> languages;

            for (const std::string languages_path = "languages"; const auto &entry: std::filesystem::directory_iterator(
                    languages_path)) {
                std::ifstream f(entry.path());
                if (!f.is_open()) {
                    continue;
                }

                std::string str((std::istreambuf_iterator(f)), std::istreambuf_iterator<char>());

                Types::Language language;
                language.deserialize(str);

                const std::filesystem::path &filePath = entry.path();
                std::string fileName = filePath.stem().string();
                languages[fileName] = language;
            }

            return languages;
        }

        static void
        register_command(dpp::cluster *bot, const std::unordered_map<std::string, Types::Language> *languages) {
            auto language_command = dpp::slashcommand("language", "A command for programming languages", bot->me.id);

            auto kv = std::views::keys(*languages);
            const std::vector<std::string> keys{kv.begin(), kv.end()};

            auto language_option = dpp::command_option(dpp::co_string, "language", "the language you want", true);

            for (const auto &key: keys) {
                language_option.add_choice(dpp::command_option_choice((*languages).at(key).title, key));
            }

            language_command.add_option(language_option);

            bot->guild_command_create(language_command, dpp::snowflake(dotenv::env["GUILD_ID"]));
        }

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
};


#endif //KODI_COMMANDS_H
