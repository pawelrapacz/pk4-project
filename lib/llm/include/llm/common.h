#pragma once

#include <string>
#include <vector>

namespace llm {

    struct message {
        std::string role;
        std::string content;
    };

    using chat_messages = std::vector<message>;

} // namespace llm
