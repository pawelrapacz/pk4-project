#pragma once

#include <llm/detail/exception.h>
#include <llm/detail/vendor_fwd.h>

namespace llm {
    namespace detail {
        class remote_client {
            httplib::Client* _client;

          public:
            remote_client(const std::string&);
            ~remote_client();
            std::string request(const char*, const nlohmann::ordered_json&);
        };

        class remote_client_error : public exception {
          public:
            explicit remote_client_error(const char* msg)
                : exception(msg) { }

            explicit remote_client_error(const std::string& msg)
                : exception(msg) { }

            remote_client_error(remote_client_error&&) noexcept = default;
            remote_client_error& operator=(remote_client_error&&) noexcept
                = default;
            remote_client_error(const remote_client_error&) = default;
            remote_client_error& operator=(const remote_client_error&)
                = default;

            virtual ~remote_client_error() = default;
        };


    } // namespace detail
} // namespace llm
