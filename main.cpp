#include <dotenv.h>
#include <dpp/dpp.h>
#include <fmt/core.h>

#include <string>
#include <fstream>
#include <ranges>

#include "Types.h"

int main() {
    dotenv::env.load_dotenv();

    std::unordered_map<std::string, Types::Language> languages;

    for (const std::string languages_path = "languages"; const auto& entry : std::filesystem::directory_iterator(languages_path)) {
        std::ifstream f(entry.path());
        if (!f.is_open()) {
            continue;
        }

        std::string str((std::istreambuf_iterator(f)), std::istreambuf_iterator<char>());

        Types::Language language;
        language.deserialize(str);

        const std::filesystem::path& filePath = entry.path();
        std::string fileName = filePath.stem().string();
        languages[fileName] = language;
    }

    dpp::cluster bot(dotenv::env["BOT_TOKEN"], dpp::i_default_intents | dpp::i_message_content);

    bot.on_log(dpp::utility::cout_logger());

    bot.on_slashcommand([&bot, &languages](const dpp::slashcommand_t& event) {
        if (const std::string cmdName = event.command.get_command_name(); cmdName == "language") {
            const std::string option = std::get<std::string>(event.get_parameter("language"));
            auto language = languages[option];
            event.reply(language.title);
        } else {
            bot.log(dpp::ll_error, "No idea how but a command was called that does not exist");
        }
    });

    bot.on_ready([&bot, &languages](const dpp::ready_t& /*event*/) {
        if (dpp::run_once<struct register_bot_commands>()) {
            auto language_command = dpp::slashcommand("language", "A command for programming languages", bot.me.id);

            auto kv = std::views::keys(languages);
            const std::vector<std::string> keys{ kv.begin(), kv.end() };

            auto language_option = dpp::command_option(dpp::co_string, "language", "the language you want", true);

            for (const auto& key : keys) {
                language_option.add_choice(dpp::command_option_choice(languages[key].title, key));
            }

            language_command.add_option(language_option);

            bot.guild_command_create(language_command, dpp::snowflake(dotenv::env["GUILD_ID"]));
        }

        bot.set_presence(dpp::presence(dpp::ps_online, dpp::at_custom, "twerking and stuff"));
    });

    bot.on_message_create([](const dpp::message_create_t& event) {
        if (event.msg.author.is_bot())
            return;

        event.reply(event.msg.content);
    });

    bot.start(dpp::st_wait);

    return 0;
}
