#include <doctest/doctest.h>
#include <llm/llama_wrapper.h>
#include <nlohmann/json.hpp>

using nlohmann::json;

TEST_SUITE_BEGIN("llama_wrapper");

TEST_CASE("generate") {
    llm::llama_wrapper llm(MODELS_PATH "/gemma-4-E2B-it-Q8_0.gguf");

    SUBCASE("unformatted") {
        std::string res
            = llm.generate("Hey! write lorem ipsum, 1 paragraph please");

        CHECK(!res.empty());
    }

    // SUBCASE("formatted") {
    //     json format = {
    //         {"type", "object"},
    //         {"properties", {
    //             {"text", json::object({{"type", "string"}})},
    //         }},
    //         {"required", {}},
    //     };

    //     json res = json::parse(llm.generate("Pick two numbers from 1 to 10",
    //     f));


    //     CHECK(res.contains("x"));
    //     CHECK(res.contains("y"));
    // }
}


TEST_SUITE_END();
