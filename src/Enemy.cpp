#include "Enemy.h"

#include <httplib.h>
#include <nlohmann/json.hpp>

#include <random>
#include "Interface/Config.h"
#include "Player.h"

using namespace Battleships;

extern std::mt19937 mt; // defined in Player.cpp
static std::uniform_int_distribution<std::size_t> dist(0, 9);

SimpleEnemy::Pos SimpleEnemy::MakeTurn(const Grid& grid [[maybe_unused]]) const noexcept  {
    return {dist(mt), dist(mt)};
}


nlohmann::json GridToJson(const Player::Grid& grid) {
    nlohmann::json jgrid = nlohmann::json::array();

    for (std::size_t i {}; i < Player::GRID_SIZE; i++) {
        nlohmann::json row = nlohmann::json::array();
        for (std::size_t j {}; j < Player::GRID_SIZE; j++) {
            switch (grid[i][j]) {
                case Square::Hit:
                    row.push_back("H");
                    break;
                case Square::Missed:
                    row.push_back("M");
                    break;
                default:
                    row.push_back(".");
                    break;
            }
        }
        jgrid.push_back(row);
    }

    return jgrid;
}


AIEnemy::Pos AIEnemy::MakeTurn(const Grid& grid [[maybe_unused]]) const noexcept  {
    httplib::Client client("http://localhost:11434");

    static std::string prompt = "You are playing battleship game. With the given table make the best possible move, the table contains values like \".\" (empty posible to attack), \"M\" (already attacked - missed), \"H\" (already attacked hit), these values represent points. You can only attack empty points. The given table is in json format: ";

    nlohmann::json req = {
        {"model", "mistral:latest"},
        {"prompt", prompt + GridToJson(grid).dump()},
        {"stream", false},
        {"format", {
            {"type", "object"},
            {"properties", {
                {"x", nlohmann::json::object({{"type", "number"}})},
                {"y", nlohmann::json::object({{"type", "number"}})}
            }},
            {"required", nlohmann::json::array({"x", "y"})}
        }}
    };

    auto res = client.Post("/api/generate", req.dump(), "application/json");
    

    nlohmann::json attackVec = nlohmann::json::parse(nlohmann::json::parse(res->body)["response"].get<std::string>());

    logging::warn(attackVec.dump());
    
    std::size_t x = attackVec["x"].get<std::size_t>();
    std::size_t y = attackVec["y"].get<std::size_t>();

    // should do AI stuff
    return {x, y};
}