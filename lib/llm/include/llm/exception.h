#pragma once

#include <exception>
#include <string>

namespace llm {

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

    class llama_wrapper_error : public exception {
      public:
        explicit llama_wrapper_error(const char* msg)
            : exception(msg) { }

        explicit llama_wrapper_error(const std::string& msg)
            : exception(msg) { }

        llama_wrapper_error(llama_wrapper_error&&) noexcept = default;
        llama_wrapper_error& operator=(llama_wrapper_error&&) noexcept
            = default;
        llama_wrapper_error(const llama_wrapper_error&)            = default;
        llama_wrapper_error& operator=(const llama_wrapper_error&) = default;

        virtual ~llama_wrapper_error() = default;
    };

} // namespace llm
