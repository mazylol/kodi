#include <dotenv.h>
#include <dpp/dpp.h>

#include "Commands.h"
#include "Types.h"

int main() {
    dotenv::env.load_dotenv();

    std::string guildId = dotenv::env["GUILD_ID"];

    auto languages = Types::load<Types::Language>("languages");
    auto people = Types::load<Types::Person>("people");

    dpp::cluster bot(dotenv::env["BOT_TOKEN"]);

    bot.on_log(dpp::utility::cout_logger());

    bot.on_slashcommand([&bot, &languages, &people](const dpp::slashcommand_t &event) {
        if (const std::string cmdName = event.command.get_command_name(); cmdName == "language") {
            Commands::handle_language_command(&event, &languages);
        } else if (cmdName == "person") {
            Commands::handle_person_command(&event, &people);
        } else if (cmdName == "shutdown" && dotenv::env["PROD"].empty()) {
            bot.shutdown();
        }
    });

    bot.on_ready([&bot, &guildId, &languages, &people](const dpp::ready_t & /*event*/) {
        if (dpp::run_once<struct register_bot_commands>()) {
            Commands::register_command(&bot, &guildId, &languages, "language", "A command for programming languages", "language",
                                       "the language you want");
            Commands::register_command(&bot, &guildId, &people, "person", "A command for influential programming figures",
                                       "person", "the person you want");

            if (dotenv::env["PROD"].empty()) {
                bot.guild_command_create(
                    dpp::slashcommand("shutdown", "Turn off the bot", bot.me.id).set_default_permissions(dpp::permissions::p_administrator), guildId);
            }
        }

        bot.set_presence(dpp::presence(dpp::ps_online, dpp::at_custom, "watching you steal from stackoverflow"));
    });

    bot.start(dpp::st_wait);

    return 0;
}
