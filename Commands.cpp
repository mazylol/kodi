#include "Commands.h"

#include <fmt/format.h>

namespace Commands {
    void handle_language_command(const dpp::slashcommand_t *event, std::unordered_map<std::string, Types::Language> *languages) {
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

    void handle_person_command(const dpp::slashcommand_t *event, std::unordered_map<std::string, Types::Person> *people) {
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
} // namespace Commands