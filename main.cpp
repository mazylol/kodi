#include <dotenv.h>
#include <dpp/dpp.h>
#include <fmt/format.h>
#include <string>

#include "Commands.h"
#include "Types.h"

int main() {
    dotenv::env.load_dotenv();

    auto languages = Types::load<Types::Language>("languages");
    auto people = Types::load<Types::Person>("people");

    bool prod = !dotenv::env["PROD"].empty();

    std::string token;

    if (prod) {
        token = dotenv::env["PROD_BOT_TOKEN"];
    } else {
        token = dotenv::env["DEV_BOT_TOKEN"];
    }

    dpp::cluster bot(token);

    bot.on_log(dpp::utility::cout_logger());

    bot.on_slashcommand([&bot, &prod, &languages, &people](const dpp::slashcommand_t &event) {
        if (const std::string cmdName = event.command.get_command_name(); cmdName == "language") {
            Commands::handle_language_command(&event, &languages);
        } else if (cmdName == "person") {
            Commands::handle_person_command(&event, &people);
        } else if (cmdName == "shutdown" && !prod) {
            bot.shutdown();
        }
    });

    bot.on_ready([&bot, &prod, &languages, &people](const dpp::ready_t & /*event*/) {
        bot.log(dpp::ll_info, fmt::format("Logged in as {}#{}", bot.me.username, bot.me.discriminator));

        std::string guildId(dotenv::env["DEV_GUILD_ID"]);

        bot.log(dpp::ll_info, fmt::format("Running in {} mode", prod ? "production" : "development"));

        if (dpp::run_once<struct register_bot_commands>()) {
            Commands::register_command(&bot, &guildId, prod, &languages, "language", "A command for programming languages", "language",
                                       "the language you want");
            Commands::register_command(&bot, &guildId, prod, &people, "person", "A command for influential programming figures",
                                       "person", "the person you want");

            if (!prod) {
                bot.guild_command_create(
                    dpp::slashcommand("shutdown", "Turn off the bot", bot.me.id).set_default_permissions(dpp::permissions::p_administrator), guildId);
            }
        }

        bot.set_presence(dpp::presence(dpp::ps_online, dpp::at_watching, "you steal from stackoverflow"));
    });

    bot.start(dpp::st_wait);

    return 0;
}
