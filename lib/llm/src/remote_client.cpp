#include "llm/detail/remote_client.h"

#include <httplib.h>
#include <nlohmann/json.hpp>

using namespace llm::detail;

remote_client::remote_client(const std::string& url)
    : _client(new httplib::Client(url)) { }

remote_client::~remote_client() { delete _client; }

std::string remote_client::request(const char* path,
                                   const nlohmann::ordered_json& req) {
    auto res = _client->Post(path, req.dump(), "application/json");

    if (res.error() != httplib::Error::Success)
        throw remote_client_error("api request error");

    return res->body;
}
