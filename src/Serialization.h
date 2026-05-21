#pragma once

#include "Game.h"

#include <nlohmann/json_fwd.hpp>

#include <cstdint>
#include <filesystem>
#include <memory>
#include <optional>

namespace Battleships {

    nlohmann::ordered_json GridToJson(const Grid&);
    std::optional<Grid> JsonToGrid(const nlohmann::ordered_json&);

    bool SerializeGame(const std::filesystem::path&, const Game&);

    struct DeserializeGameResult {
        std::uint32_t turn;
        Player player;
        std::unique_ptr<Enemy> enemy;
    };

    std::optional<DeserializeGameResult> DeserializeGame(const std::filesystem::path&);

} // namespace Battleships
