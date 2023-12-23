#include "Commands.h"

#include <fmt/format.h>

void Commands::handle_language_command(const dpp::slashcommand_t *event, std::unordered_map<std::string, Types::Language> *languages) {
    const std::string option = std::get<std::string>(event->get_parameter("language"));
    auto language = (*languages)[option];

    auto embed = dpp::embed()
                     .set_title(language.title)
                     .set_description(language.description)
                     .set_thumbnail(fmt::format("attachment://{}", language.thumbnail_url))
                     .set_footer(dpp::embed_footer().set_text("Kodi").set_icon("attachment://avatar.png"))
                     .set_color(5793266)
                     .set_timestamp(time(nullptr));

    for (const auto &field : language.fields) {
        embed.add_field(field.name, field.value, false);
    }

    auto msg = dpp::message(event->command.channel_id, embed)
                   .add_file(
                       language.thumbnail_url,
                       dpp::utility::read_file(fmt::format("assets/languages/{}", language.thumbnail_url)))
                   .add_file(
                       "avatar.png",
                       dpp::utility::read_file("assets/avatar.png"));

    event->reply(msg);
}

void Commands::handle_person_command(const dpp::slashcommand_t *event, std::unordered_map<std::string, Types::Person> *people) {
    const std::string option = std::get<std::string>(event->get_parameter("person"));
    auto person = (*people)[option];

    auto embed = dpp::embed()
                     .set_title(person.title)
                     .set_description(person.description)
                     .set_thumbnail(fmt::format("attachment://{}", person.thumbnail_url))
                     .set_footer(dpp::embed_footer().set_text("Kodi").set_icon("attachment://avatar.png"))
                     .set_color(5793266)
                     .set_timestamp(time(nullptr));

    for (const auto &field : person.fields) {
        embed.add_field(field.name, field.value, false);
    }

    auto msg = dpp::message(event->command.channel_id, embed)
                   .add_file(
                       person.thumbnail_url,
                       dpp::utility::read_file(fmt::format("assets/people/{}", person.thumbnail_url)))
                   .add_file(
                       "avatar.png",
                       dpp::utility::read_file("assets/avatar.png"));

    event->reply(msg);
}

template <typename T>
void Commands::register_command(dpp::cluster *bot, std::string *guildId, bool prod, const std::unordered_map<std::string, T> *options, const std::string &name,
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

    if (prod) {
        bot->global_command_create(command);
    } else {
        bot->guild_command_create(command, dpp::snowflake((*guildId)));
    }
}

template void Commands::register_command(dpp::cluster *bot, std::string *guildId, bool prod, const std::unordered_map<std::string, Types::Language> *options, const std::string &name,
                                         const std::string &description, const std::string &option_name,
                                         const std::string &option_description);
template void Commands::register_command(dpp::cluster *bot, std::string *guildId, bool prod, const std::unordered_map<std::string, Types::Person> *options, const std::string &name,
                                         const std::string &description, const std::string &option_name,
                                         const std::string &option_description);