#pragma once

#include <exception>
#include <string>

namespace llm {
    namespace detail {

        class exception : public std::exception {
          public:
            explicit exception(const char* msg)
                : _msg(msg) { }

            explicit exception(const std::string& msg)
                : _msg(msg) { }

            exception(exception&&) noexcept            = default;
            exception& operator=(exception&&) noexcept = default;
            exception(const exception&)                = default;
            exception& operator=(const exception&)     = default;

            virtual ~exception() = default;

            const char* what() const noexcept override { return _msg.c_str(); }

          private:
            std::string _msg;
        };

    } // namespace detail
} // namespace llm
