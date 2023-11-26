#include <dotenv.h>
#include <dpp/dpp.h>
#include <fmt/core.h>

#include <string>

#include "Commands.h"

int main() {
    dotenv::env.load_dotenv();

    auto languages = Commands::Language::load_languages();

    dpp::cluster bot(dotenv::env["BOT_TOKEN"], dpp::i_default_intents | dpp::i_message_content);

    bot.on_log(dpp::utility::cout_logger());

    bot.on_slashcommand([&bot, &languages](const dpp::slashcommand_t &event) {
        if (const std::string cmdName = event.command.get_command_name(); cmdName == "language") {
            Commands::Language::handle_command(&event, &languages);
        } else {
            bot.log(dpp::ll_error, "No idea how but a command was called that does not exist");
        }
    });

    bot.on_ready([&bot, &languages](const dpp::ready_t & /*event*/) {
        if (dpp::run_once<struct register_bot_commands>()) {
            Commands::Language::register_command(&bot, &languages);
        }

        bot.set_presence(dpp::presence(dpp::ps_online, dpp::at_custom, "watching you steal from stackoverflow"));
    });

    bot.start(dpp::st_wait);

    return 0;
}
