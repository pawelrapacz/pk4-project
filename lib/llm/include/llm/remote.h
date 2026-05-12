#pragma once

#include <iso646.h>
#include <llm/common.h>
#include <llm/detail/exception.h>
#include <llm/detail/llm_interface.h>
#include <llm/detail/remote_client.h>

#include <nlohmann/json.hpp>

#include <concepts>
#include <string>

namespace llm {

    template<typename Tp>
    concept api_formatter = requires {
        typename Tp::options;
        requires std::default_initializable<typename Tp::options>;
    } and requires {
        { Tp::path::generate } -> std::convertible_to<const char*>;
        { Tp::path::chat } -> std::convertible_to<const char*>;
    } and requires(const std::string& prompt, const nlohmann::ordered_json& format, Tp::options opt, chat_messages messages) {
        // requests
        { Tp::generate(prompt, opt) } -> std::same_as<nlohmann::ordered_json>;
        {
            Tp::generate(prompt, opt, format)
        } -> std::same_as<nlohmann::ordered_json>;
        { Tp::chat(messages, opt) } -> std::same_as<nlohmann::ordered_json>;
        {
            Tp::chat(messages, opt, format)
        } -> std::same_as<nlohmann::ordered_json>;
    } and requires(std::string response) {
        // responses
        { Tp::parse_chat_response(response) } -> std::same_as<message>;
        { Tp::parse_generate_response(response) } -> std::same_as<std::string>;
    };


    struct ollama_api {
        struct path {
            static constexpr auto generate = "/api/generate";
            static constexpr auto chat     = "/api/chat";
        };

        struct options {
            std::string model;
            bool think = false;
            bool raw   = false;
        };

        static nlohmann::ordered_json generate(const std::string&,
                                               const options&);
        static nlohmann::ordered_json generate(const std::string&,
                                               const options&,
                                               const nlohmann::ordered_json&);
        static nlohmann::ordered_json chat(const chat_messages&,
                                           const options&);
        static nlohmann::ordered_json chat(const chat_messages&, const options&,
                                           const nlohmann::ordered_json&);

        static message parse_chat_response(const std::string&);
        static std::string parse_generate_response(const std::string&);
    };


    template<api_formatter api>
    class remote : public detail::llm_interface {
      public:
        remote(const std::string& url, api::options opt = {})
            : _impl(url), _opt(opt) { }

        std::string& chat(chat_messages& msg) override {
            auto res = _impl.request(api::path::chat, api::chat(msg, _opt));
            msg.push_back(api::parse_chat_response(res));
            return msg.back().content;
        }

        std::string& chat(chat_messages& msg,
                          const nlohmann::ordered_json& format) override {
            auto res
                = _impl.request(api::path::chat, api::chat(msg, _opt, format));
            msg.push_back(api::parse_chat_response(res));
            return msg.back().content;
        }

        std::string generate(const std::string& prompt) override {
            auto res = _impl.request(api::path::generate,
                                     api::generate(prompt, _opt));
            return api::parse_generate_response(res);
        }

        std::string generate(const std::string& prompt,
                             const nlohmann::ordered_json& format) override {
            auto res = _impl.request(api::path::generate,
                                     api::generate(prompt, _opt, format));
            return api::parse_generate_response(res);
        }

      private:
        detail::remote_client _impl;
        api::options _opt;
    };


    using ollama_remote = remote<ollama_api>;


    class remote_error : public detail::exception {
      public:
        explicit remote_error(const char* msg)
            : exception(msg) { }

        explicit remote_error(const std::string& msg)
            : exception(msg) { }

        remote_error(remote_error&&) noexcept            = default;
        remote_error& operator=(remote_error&&) noexcept = default;
        remote_error(const remote_error&)                = default;
        remote_error& operator=(const remote_error&)     = default;

        virtual ~remote_error() = default;
    };

} // namespace llm
