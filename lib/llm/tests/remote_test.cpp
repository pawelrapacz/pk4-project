#include <doctest/doctest.h>
#include <llm/remote.h>

using nlohmann::json, nlohmann::ordered_json;

TEST_SUITE_BEGIN("remote");

#define MODEL "gemma4:e2b"

TEST_CASE("generate") {
    llm::ollama_remote llm("http://localhost:11434", {MODEL});

    SUBCASE("unformatted") {
        std::string res
            = llm.generate("Hey! write lorem ipsum, 1 paragraph please");
        CHECK(!res.empty());
    }

    SUBCASE("formatted") {
        json format = {
            {      "type","object"                          },
            {"properties",
             {
             {"x", json::object({{"type", "number"}})},
             {"y", json::object({{"type", "number"}})},
             }                       },
            {  "required", {"x", "y"}},
        };

        json res = ordered_json::parse(
            llm.generate("Pick two numbers from 1 to 10", format));

        CHECK(res.contains("x"));
        CHECK(res.contains("y"));
    }
}

TEST_CASE("chat") {
    llm::ollama_remote llm("http://localhost:11434", {MODEL});
    llm::chat_messages chat;

    SUBCASE("unformatted") {
        chat.push_back({"user", "Hey! write lorem ipsum, 1 paragraph please"});

        std::string res = llm.chat(chat);

        CHECK(!res.empty());
        CHECK(chat.size() == 2);
    }

    SUBCASE("formatted") {
        ordered_json format = {
            {      "type","object"                          },
            {"properties",
             {
             {"x", json::object({{"type", "number"}})},
             {"y", json::object({{"type", "number"}})},
             }                       },
            {  "required", {"x", "y"}},
        };

        chat.push_back({"user", "Pick two numbers from 1 to 10"});

        json res = json::parse(llm.chat(chat, format));

        CHECK(res.contains("x"));
        CHECK(res.contains("y"));
        CHECK(chat.size() == 2);
    }
}


TEST_SUITE_END();
