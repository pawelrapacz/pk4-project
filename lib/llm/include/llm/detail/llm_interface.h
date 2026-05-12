#pragma once

#include <llm/common.h>

#include <nlohmann/json_fwd.hpp>

#include <string>

namespace llm {
    namespace detail {

        class llm_interface {
          public:
            virtual ~llm_interface() = default;

            virtual std::string& chat(chat_messages&)                   = 0;
            virtual std::string& chat(chat_messages&,
                                      const nlohmann::ordered_json&)    = 0;
            virtual std::string generate(const std::string&)            = 0;
            virtual std::string generate(const std::string&,
                                         const nlohmann::ordered_json&) = 0;
        };

    } // namespace detail
} // namespace llm
