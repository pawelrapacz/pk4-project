#include "Enemy.h"

#include <httplib.h>
#include <nlohmann/json.hpp>
#include <logging/logging.h>

#include <random>
#include <thread>
#include <chrono>
#include <algorithm>
#include <optional>
#include <vector>


using namespace Battleships;

struct Vec {int x, y;};


thread_local extern std::mt19937 mt; // defined in PlayerBuilder.cpp
thread_local static std::uniform_int_distribution<std::size_t> dist(0, GRID_SIZE - 1);


static bool InBounds(Pos pos) noexcept {
    return pos.x >= 0 && pos.x < GRID_SIZE &&
           pos.y >= 0 && pos.y < GRID_SIZE;
}

static inline Pos RandomMove(const Grid& grid) noexcept {
    logging::debug("Making random move");
    Pos pos;
    
    do {
        pos.x = dist(mt);
        pos.y = dist(mt);
    } while (grid[pos.x][pos.y] != GridSquare::None);
    
    logging::debug("Random move: {}{}.", char(pos.x + 'a'), pos.y + 1);

    return pos;
}

static inline std::optional<Pos> ScanInLineOneDirection(const Grid& grid, Pos pos, Vec dir) {
    Pos currentPos = {pos.x + dir.x, pos.y + dir.y};
    
    while (InBounds(currentPos)) {
        // if there is a miss then its the end of a ship
        if (grid[currentPos.x][currentPos.y] == GridSquare::Missed)
            break;

        // it there is an empty sqare it is possible that the next
        // part of the ship is there
        if (grid[currentPos.x][currentPos.y] == GridSquare::None)
            return {currentPos};

        currentPos.x += dir.x;
        currentPos.y += dir.y;
    }
    return std::nullopt;
}

static inline std::optional<Pos> ScanInLineBothDirections(const Grid& grid, Pos pos, Vec dir) {
    logging::debug("Scanning in line {}{} neighbours", char(pos.x + 'a'), pos.y + 1);
    // scan forward and backwards
    for (auto d : {dir, {-dir.x, -dir.y}})
        if (auto res = ScanInLineOneDirection(grid, pos, d))
            return res.value();
    return std::nullopt;
}


std::optional<Pos> SearchPossible(const Grid& grid, Pos pos) {
    // vectors of pos neighbours
    static constexpr auto NeighbourVectors =  std::to_array<Vec>({
        {-1,  0}, // top
        { 1,  0}, // bottom
        { 0, -1}, // left
        { 0,  1}, // right
    });

    // if the position is not a hit then no predictions can be made
    // no ship, no prediction
    if (grid[pos.x][pos.y] != GridSquare::Hit)
        return std::nullopt;

    logging::debug("Checking {}{} neighbours", char(pos.x + 'a'), pos.y + 1);

    std::vector<Pos> possible;

    for (auto vec : NeighbourVectors) {
        Pos currentPos = {pos.x + vec.x, pos.y + vec.y};
        if (not InBounds(currentPos)) 
            continue;
        
        auto& square = grid[currentPos.x][currentPos.y];

        // chceck for any possible attacks in the direction of the ship
        // (whe know the orientation of the ship)
        if (square == GridSquare::Hit) {
            if (auto d = ScanInLineBothDirections(grid, pos, vec))
                return d.value();
        }

        // check for possible attacks
        // (the ones that we don't know the orientation of the ship)
        if (square == GridSquare::None) {
            possible.push_back(currentPos);
        }
    }

    if (not possible.empty()) {
        // get the random position out of possible
        std::ranges::shuffle(possible, mt);
        return possible.front();
    } else {
        return std::nullopt;
    }
}

static std::optional<Pos> SmartMove(const Grid& grid) {
    logging::debug("Trying SmartMove");
    for (std::size_t i = 0; i < GRID_SIZE; i++) {
        for (std::size_t j = 0; j < GRID_SIZE; j++) {
            // find the first possible move
            // TODO: evaluate the best move
            if (auto pos = SearchPossible(grid, {i, j}))
                return pos;
        }
    }
    return std::nullopt;
}


Pos SimpleEnemy::MakeTurnImpl(const Grid& grid) {
    logging::debug("Making a move");

    // slow down a bit
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    if (auto pos = SmartMove(grid))
        return pos.value();
    else
        return RandomMove(grid);
}


Pos SimpleEnemy::MakeTurn(const Grid& grid) const {
    return MakeTurnImpl(grid);
}



static inline nlohmann::json GridToJson(const Grid& grid) {
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


Pos AIEnemy::MakeTurnImpl(const Grid& grid) {
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
        return SimpleEnemy::MakeTurnImpl(grid);
    }
}

Pos AIEnemy::MakeTurn(const Grid& grid) const {
    return MakeTurnImpl(grid);
}