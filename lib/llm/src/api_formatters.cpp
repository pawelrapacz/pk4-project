#include "llm/remote.h"

#include <nlohmann/json.hpp>

using namespace llm;
using nlohmann::ordered_json;

ordered_json ollama_api::generate(const std::string& prompt,
                                  const options& opt) {
    ordered_json req = {
        { "model", opt.model},
        {"prompt",    prompt},
        {"stream",     false},
        { "think", opt.think},
        {   "raw",   opt.raw},
    };

    return req;
}

ordered_json ollama_api::generate(const std::string& prompt, const options& opt,
                                  const ordered_json& format) {
    ordered_json req = ollama_api::generate(prompt, opt);
    req.push_back(ordered_json::object_t::value_type("format", format));
    return req;
}

ordered_json ollama_api::chat(const chat_messages& msg, const options& opt) {
    ordered_json::array_t messages;

    for (auto& m : msg) {
        messages.push_back({
            {   "role",    m.role},
            {"content", m.content},
        });
    }

    ordered_json req = {
        {   "model", opt.model},
        {"messages",  messages},
        {  "stream",     false},
    };
    return req;
}

ordered_json ollama_api::chat(const chat_messages& msg, const options& opt,
                              const ordered_json& format) {
    ordered_json req = ollama_api::chat(msg, opt);
    req.push_back(ordered_json::object_t::value_type("format", format));
    logging::debug("formatted chat req: {}", req.dump(4));
    return req;
}

std::string ollama_api::parse_generate_response(const std::string& res) {
    ordered_json jres = ordered_json::parse(res);

    if (jres.contains("error"))
        throw remote_error(jres["error"].get<std::string>());

    return jres.at("response").get<std::string>();
}

llm::message ollama_api::parse_chat_response(const std::string& res) {
    ordered_json jres = ordered_json::parse(res);
    logging::debug(jres.dump(4));

    if (jres.contains("error"))
        throw remote_error(jres["error"].get<std::string>());

    return {
        .role    = jres.at("message").at("role").get<std::string>(),
        .content = jres.at("message").at("content").get<std::string>(),
    };
}
