#include "Enemy.h"

#include <httplib.h>
#include <algorithm>
#include <chrono>
#include <nlohmann/json.hpp>
#include <logging/logging.h>

#include <optional>
#include <random>
#include <thread>
#include <vector>

using namespace Battleships;

thread_local extern std::mt19937 mt; // defined in Player.cpp
static std::uniform_int_distribution<std::size_t> dist(0, 9);

static constexpr auto NeighbourVectors =  std::to_array<SimpleEnemy::Vec>({
    {-1,  0}, // top
    { 1,  0}, // bottom
    { 0, -1}, // left
    { 0,  1}, // right
});

Pos RandomMove(const Grid& grid) noexcept {
    logging::debug("Making random move");
    Pos pos;
    
    do {
        pos.x = dist(mt);
        pos.y = dist(mt);
    } while (grid[pos.x][pos.y] != GridSquare::None);
    
    logging::debug("Random move: {}{}.", char(pos.x + 'a'), pos.y + 1);

    return pos;
}

bool InBounds(Pos pos) noexcept {
    return pos.x >= 0 && pos.x < GRID_SIZE &&
           pos.y >= 0 && pos.y < GRID_SIZE;
}

std::optional<Pos> SimpleEnemy::CheckDirection(const Grid& grid, Pos pos, Vec dir) const {
    Pos currentPos = {pos.x + dir.x, pos.y + dir.y};
    
    while (InBounds(currentPos)) {
        if (grid[currentPos.x][currentPos.y] == GridSquare::Missed)
            break;
        if (grid[currentPos.x][currentPos.y] == GridSquare::None)
            return {currentPos};

        currentPos.x += dir.x;
        currentPos.y += dir.y;
    }

    dir.x *= -1;
    dir.y *= -1;
    currentPos = {pos.x + dir.x, pos.y + dir.y};

    while (InBounds(currentPos)) {
        if (grid[currentPos.x][currentPos.y] == GridSquare::Missed)
            break;
        if (grid[currentPos.x][currentPos.y] == GridSquare::None)
            return {currentPos};

        currentPos.x += dir.x;
        currentPos.y += dir.y;
    }

    return std::nullopt;
}

std::optional<Pos> SimpleEnemy::SearchPossible(const Grid& grid, Pos pos) const {
    logging::debug("Checking {}{} neighbours", char(pos.x + 'a'), pos.y + 1);

    std::vector<Pos> possible;

    for (auto i : NeighbourVectors) {
        Pos current = {pos.x + i.x, pos.y + i.y};
        if (InBounds(current)) {
            if (grid[current.x][current.y] == GridSquare::Hit) {
                if (auto d = CheckDirection(grid, pos, i))
                    return d.value();
            }
            if (grid[current.x][current.y] == GridSquare::None)
                possible.push_back(current);
        }
    }

    if (not possible.empty()) {
        std::ranges::shuffle(possible, mt);
        return possible.front();
    } else {
        return std::nullopt;
    }
}

std::optional<Pos> SimpleEnemy::SmartMove(const Grid& grid) const {
    logging::debug("Trying SmartMove");
    for (std::size_t i = 0; i < GRID_SIZE; i++) {
        for (std::size_t j = 0; j < GRID_SIZE; j++) {
            if (grid[i][j] == GridSquare::Hit) {
                if (auto pos = SearchPossible(grid, {i, j}))
                    return pos;
            }
        }
    }
    return std::nullopt;
}


Pos SimpleEnemy::MakeTurn(const Grid& grid) const {
    std::this_thread::sleep_for(std::chrono::milliseconds(300));
    if (auto pos = SmartMove(grid))
        return pos.value();
    else
        return RandomMove(grid);
}


nlohmann::json GridToJson(const Grid& grid) {
    nlohmann::json jgrid = nlohmann::json::array();

    for (std::size_t i {}; i < GRID_SIZE; i++) {
        nlohmann::json row = nlohmann::json::array();
        for (std::size_t j {}; j < GRID_SIZE; j++) {
            switch (grid[i][j]) {
                case GridSquare::Hit:
                    row.push_back("H");
                    break;
                case GridSquare::Missed:
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


Pos AIEnemy::MakeTurn(const Grid& grid) const {
    try {
        httplib::Client client("http://localhost:11434");

        std::string prompt = "You are playing battleship game. With the given table make the best possible move, the table contains values like \".\" (empty posible to attack), \"M\" (already attacked - missed), \"H\" (already attacked hit), these values represent points. You can only attack empty points. The given table is in json format: ";
        prompt += GridToJson(grid).dump();

        nlohmann::json req = {
            {"model", "gemma:2b"},
            {"prompt", prompt},
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
        
        std::size_t x = attackVec["x"].get<std::size_t>();
        std::size_t y = attackVec["y"].get<std::size_t>();

        return {x, y};
    }
    catch (...) {
        // fallback
        return SimpleEnemy::MakeTurn(grid);
    }
}