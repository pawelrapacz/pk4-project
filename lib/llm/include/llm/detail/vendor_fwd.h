#pragma once

#include <nlohmann/json_fwd.hpp>

// llama forward declarations
struct llama_model;
struct llama_context;
struct llama_sampler;
struct llama_vocab;

typedef int32_t llama_token;


// httplib forward declarations
namespace httplib {
    class Client;
}
