#include "PlayerBuilder.h"

#include "Player.h"

#include <logging/logging.h>

#include <exception>
#include <memory>
#include <optional>
#include <random>
#include <stdexcept>
#include <utility>


using namespace Battleships;

// global random
thread_local std::mt19937 mt(std::random_device {}());

Player PlayerBuilder::BuildRandom() {
    static PlayerBuilder bld;
    logging::info("Building random player");
    bld.GenerateRandomGrid();
    return bld.Build();
}

void PlayerBuilder::GenerateRandomGrid() {
    logging::info("Generating random Grid");
    Clear();

    try {

        while (auto next = GetNextShipToInsert())
            RandomInsertShip(next.value());

    } catch (...) {
        logging::error("Generating grid failed");
        std::throw_with_nested(std::runtime_error("Generating grid failed"));
    }

    logging::info("Successfully generated a grid");
}

void PlayerBuilder::Clear() noexcept {
    _fleet    = Rules::FLEET_DEFINITION;
    _grid     = {};
    _shipData = {};
}

std::optional<ShipSize> PlayerBuilder::GetNextShipToInsert() const noexcept {
    for (auto& i : _fleet)
        if (i.count > 0) return {i.size};

    return std::nullopt;
}

bool PlayerBuilder::TryInsertShip(Pos start, ShipOrientation ort) {
    logging::info("Trying to insert ship at pos: {}{}", char(start.x + 'a'),
                  start.y + 1);
    auto size = GetNextShipToInsert();
    if (not size.has_value()) {
        logging::info("No ship to insert!");
        return false;
    }

    auto ship = CreateShip(start, size.value(), ort);

    if (ValidateShipPlacement(ship)) {
        InsertShip(ship);
        InsertShipMargin(ship);
        logging::info("Successfully inserted a ship.");
        return true;
    } else {
        logging::info("Insert unsuccessful.");
        return false;
    }
}

void PlayerBuilder::RemoveShip(Pos pos) {
    // obtain ShipData shared_ptr
    auto ship = _shipData[pos.x][pos.y];

    // remove ship and data
    for (std::size_t i = ship->start.x; i < ship->end.x; i++) {
        for (std::size_t j = ship->start.y; j < ship->end.y; j++) {
            _grid[i][j] = GridSquare::None;
            _shipData[i][j].reset();
        }
    }

    // update fleet state
    for (auto& i : _fleet)
        if (i.size == ship->size) i.count++;

    // reset margins
    _grid = RemoveMargins(_grid);
    for (std::size_t i = 0; i < GRID_SIZE; i++)
        for (std::size_t j = 0; j < GRID_SIZE; j++)
            if (_shipData[i][j]) InsertShipMargin(*_shipData[i][j]);

    logging::info("Successfully removed ship");
}

const Grid& PlayerBuilder::GetGrid() const noexcept { return _grid; }

bool PlayerBuilder::Ready() const noexcept {
    for (auto i : _fleet)
        if (i.count > 0) return false;
    return true;
}

Player PlayerBuilder::Build() const {
    logging::info("Building player.");
    if (!Ready()) {
        logging::error("Player not ready for build.");
        throw std::runtime_error("Player not ready for build.");
    }

    return {RemoveMargins(_grid), _shipData};
}


static PlayerBuilder::ShipOrientation RandomOrientation() {
    using Ort = PlayerBuilder::ShipOrientation;

    std::uniform_int_distribution<std::underlying_type_t<Ort>> direction(
        std::to_underlying(Ort::Horizontal), std::to_underlying(Ort::Vertical));

    return static_cast<Ort>(direction(mt));
}

PlayerBuilder::ShipData
PlayerBuilder::CreateShip(Pos start, ShipSize size,
                          ShipOrientation ort) noexcept {
    Pos end;
    if (ort == ShipOrientation::Horizontal)
        end = {start.x + size, start.y + 1};
    else // vertical
        end = {start.x + 1, start.y + size};

    return {size, start, end};
}

Grid PlayerBuilder::RemoveMargins(const Grid& grid) noexcept {
    Grid out = grid;
    // erase GridSquare::Missed (margins for insertion)
    for (std::size_t i {}; i < GRID_SIZE; i++)
        for (std::size_t j {}; j < GRID_SIZE; j++)
            if (out[i][j] != GridSquare::Ship) out[i][j] = GridSquare::None;

    return out;
}

void PlayerBuilder::RandomInsertShip(const ShipSize size) {
    // unocnstrained
    std::uniform_int_distribution<std::size_t> udist(0z, GRID_SIZE - 1);
    // constrained because of size
    std::uniform_int_distribution<std::size_t> cdist(0z, GRID_SIZE - size);

    ShipData ship {};
    uint32_t hits {};
    bool foundPlacement = false;

    // searching for possible ship placement
    while (!foundPlacement) {
        auto ort = RandomOrientation();
        if (ort == ShipOrientation::Horizontal)
            ship = CreateShip({cdist(mt), udist(mt)}, size, ort);
        else // vertical
            ship = CreateShip({udist(mt), cdist(mt)}, size, ort);

        // checking collision with other ships/margins
        // if it collides the placment is rejected
        foundPlacement = ValidateShipPlacement(ship);

        // Fail-safe: prevent infinite loop if random placement keeps failing.
        if (++hits > 100u) { // unlikely
            logging::error("Random insering a ship of size {} failed {} hits.",
                           size, hits);
            throw std::runtime_error("Inserting ship failed.");
        }
    }

    InsertShip(ship);       // insert ship and its data
    InsertShipMargin(ship); // sets margins
    logging::info("Successfully randomly inserted a ship of size {} ({} hits)",
                  size, hits);
}

void PlayerBuilder::InsertShipMargin(const ShipData& ship) noexcept {
    Player::InsertShipMargin(_grid, ship);
}

void PlayerBuilder::InsertShip(const ShipData& ship) {
    for (auto& i : _fleet) {
        if (i.size == ship.size) {
            if (i.count == 0) {
                logging::error("Ship insertion would violate the rules");
                throw std::runtime_error(
                    "Ship insertion would violate the rules");
            }
            i.count--;
            break;
        }
    }

    auto shipPtr = std::make_shared<ShipData>(ship);
    for (std::size_t i = ship.start.x; i < ship.end.x; i++) {
        for (std::size_t j = ship.start.y; j < ship.end.y; j++) {
            // assert that position is empty
            assert(not _shipData[i][j] && _grid[i][j] == GridSquare::None
                   && "Cannot insert ship, position already taken.");

            _grid[i][j]     = GridSquare::Ship;
            _shipData[i][j] = shipPtr;
        }
    }
}

bool PlayerBuilder::ValidateShipPlacement(const ShipData& ship) const noexcept {
    // overflow
    if (ship.end.x > GRID_SIZE || ship.end.y > GRID_SIZE) return false;

    // collisions
    for (std::size_t i = ship.start.x; i < ship.end.x; i++)
        for (std::size_t j = ship.start.y; j < ship.end.y; j++)
            if (_grid[i][j] != GridSquare::None) return false;
    return true;
}
