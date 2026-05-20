#include "llm/chat.h"

#include <nlohmann/json.hpp>

using namespace llm;

chat::chat(llm_ptr ptr, const std::string& system_prompt)
    : _llm(std::move(ptr)) {
    _msg.emplace_back("system", system_prompt.data());
}

const std::string& chat::prompt(const std::string& p) {
    _msg.emplace_back("user", p);
    return _llm->chat(_msg);
}

const std::string& chat::prompt(const std::string& p, const nlohmann::ordered_json& f) {
    _msg.emplace_back("user", p);
    return _llm->chat(_msg, f);
}


const message& chat::last_message() const { return _msg.back(); }

const chat_messages& chat::messages() const noexcept { return _msg; }
