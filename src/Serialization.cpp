#include "Serialization.h"

#include "Enemy.h"
#include "Player.h"
#include "Game.h"
#include "PlayerBuilder.h"
#include "nlohmann/json_fwd.hpp"

#include <logging/logging.h>
#include <nlohmann/json.hpp>

#include <cstdint>
#include <filesystem>
#include <fstream>
#include <memory>
#include <optional>
#include <string_view>

namespace fs = std::filesystem;
using namespace Battleships;
using nlohmann::ordered_json;


ordered_json Battleships::GridToJson(const Grid& grid) {
    logging::info("Converting Grid to json.");

    ordered_json json = ordered_json::array();

    for (std::size_t i {}; i < GRID_SIZE; i++) {
        ordered_json row = ordered_json::array();

        for (std::size_t j {}; j < GRID_SIZE; j++) {
            switch (grid[i][j]) {
            case GridSquare::Hit:
                row.push_back("H");
                break;
            case GridSquare::Missed:
                row.push_back("M");
                break;
            case GridSquare::Ship:
                row.push_back("S");
                break;
            case GridSquare::None:
                row.push_back(".");
                break;
            }
        }
        json.push_back(row);
    }

    logging::debug(json.dump());
    return json;
}


std::optional<Grid> Battleships::JsonToGrid(const ordered_json& json) {
    logging::info("Converting json to Grid.");
    logging::debug(json.dump());

    auto fail = [] {
        logging::warn("Failed to convert json to Grid.");
        return std::nullopt;
    };

    Grid grid;

    if (not json.is_array() || json.size() != GRID_SIZE) {
        logging::warn("Json input is not an array.");
        return fail();
    }

    for (std::size_t i {}; i < GRID_SIZE; i++) {
        if (not json[i].is_array() || json[i].size() != GRID_SIZE) {
            logging::warn("Json input is not a proper 2D array.");
            return fail();
        }

        for (std::size_t j {}; j < GRID_SIZE; j++) {
            if (not json[i][j].is_string()) return fail();

            std::string s = json[i][j].get<std::string>();

            switch (s.front()) {
            case 'H':
                grid[i][j] = GridSquare::Hit;
                break;
            case 'M':
                grid[i][j] = GridSquare::Missed;
                break;
            case 'S':
                grid[i][j] = GridSquare::Ship;
                break;
            case '.':
                grid[i][j] = GridSquare::None;
                break;
            default:
                logging::warn("Unknown character '{}'.", s.front());
                return fail();
            }
        }
    }
    return grid;
}


static inline std::optional<Player> MakePlayer(const ordered_json& json) {
    if (auto optGrid = JsonToGrid(json))
        return PlayerBuilder::BuildFromGrid(optGrid.value());
    else
        return std::nullopt;
}

static inline std::unique_ptr<Enemy> MakeEnemy(const std::string& enemyType, const ordered_json& json) {
    std::unique_ptr<Enemy> enemy;

    if (auto plrOpt = MakePlayer(json)) {
        if (enemyType == "SimpleEnemy")
            enemy = std::make_unique<SimpleEnemy>(std::move(plrOpt.value()));
        else if (enemyType == "AIEnemy")
            enemy = std::make_unique<AIEnemy>(std::move(plrOpt.value()));
        else
            logging::warn("Json field \"enemyType\" has an invalid value.");
    }  
    return enemy;
}


std::optional<DeserializeGameResult> Battleships::DeserializeGame(const fs::path& path) {
    auto fail = [&path] {
        logging::warn("Failed to deserialize Game from file: {}", path.string());
        return std::nullopt;
    };

    logging::info("Deserializing Game from file {}", path.string());

    if (not fs::is_regular_file(path)) {
        logging::warn("File is not a regular file or does not exist");
        return fail();
    }

    std::ifstream file(path);

    if (not file.good()) {
        logging::warn("File could not be opened.");
        return fail();
    }
    ordered_json json;
    file >> json;

    logging::debug(json.dump());

    if (file.fail()) {
        logging::warn("Reading data from file failed");
        return fail();
    }

    if (not(json.contains("turn") && json.contains("player") && json.contains("enemy")
            && json.contains("enemyType"))) {
        logging::warn("Data in file is incomplete.");
        return fail();
    }

    if (not json["enemyType"].is_string()) {
        logging::warn("Json field \"enemyType\" is of an invalid type.");
        return fail();
    }
    std::string enemyType = json["enemyType"].get<std::string>();

    std::optional<Player> player = MakePlayer(json["player"]);
    if (not player) {
        logging::warn("Failed to MakePlayer");
        return fail();
    }

    std::unique_ptr<Enemy> enemy = MakeEnemy(enemyType, json["enemy"]);
    if (not enemy) {
        logging::warn("Failed to MakeEnemy");
        return fail();
    }
    
    std::uint32_t turn;

    if (json["turn"].is_number_unsigned()) {
        turn = json["turn"].get<std::uint32_t>();
    } else {
        logging::warn("Json field \"turn\" has an invalid value.");
        return fail();
    }

    return {{turn, std::move(player.value()), std::move(enemy)}};
}

bool Battleships::SerializeGame(const fs::path& path, const Game& game) {
    logging::info("Serializing Game to file {}", path.string());

    std::ofstream file(path);

    if (not file.good()) {
        logging::error("Opening/Creating file {} failed", path.string());
        return false;
    }

    std::string_view enemyType;
    if (dynamic_cast<const SimpleEnemy*>(&game.GetEnemy())) {
        enemyType = "SimpleEnemy";
    } else if (dynamic_cast<const AIEnemy*>(&game.GetEnemy())) {
        enemyType = "AIEnemy";
    } else {
        logging::warn("Failed to determine Enemy type");
        return false;
    }

    ordered_json json = {
        {   "player", GridToJson(game.GetPlayer().GetGrid())},
        {    "enemy", GridToJson(game.GetEnemy().GetGrid())},
        {"enemyType", enemyType},
        {     "turn", game.GetTurn()}
    };

    logging::debug(json.dump());
    file << json << std::endl;

    if (file.fail()) {
        logging::warn("Writing data to file {} failed", path.string());
        return false;
    }

    return true;
}
