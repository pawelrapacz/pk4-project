#include "Game.h"
#include "Application.h"
#include "Interface/GameOverMenu.h"
#include "Player.h"
#include "Interface/GridView.h"

#include <raylib.h>
#include <logging/logging.h>

#include <utility>
#include <memory>
#include <chrono>
#include <future>

using namespace Battleships;

Game::Game(Application& app, std::unique_ptr<Enemy> enm, Player plr)
    : _app(app),
      _pghnd(50, 50, "Your fleet"),
      _eghnd(600, 50, "Enemy fleet"),
      _plr(plr),
      _enm(std::move(enm)) { }


void Game::OnUpdate() {
    static uint32_t lastTurn {};
    if (not _playing)
        return;

    if (lastTurn != GetTurn()) {
        logging::info("{}'s turn", GetTurn() ? "Enemy" : "Player");
        lastTurn = GetTurn();
    }

    if (GetTurn() == 0) { // player turn
        // get the attack position
        auto pos = _eghnd.GetClick();

        if (pos.has_value()) // chceck if exists
            ResolvePlayerTurn(pos.value());
    } else {
        EnemyTurn();
    }
}

void Game::Draw() const {
    _pghnd.Draw(_plr.GetGrid());
    _eghnd.Draw(Player::RemoveShips(_enm->GetGrid()));
}

std::unique_ptr<Enemy> Game::ReleaseEnemy() {
    return std::move(_enm);
}

std::uint32_t Game::GetTurn() const noexcept {
    return _turn % 2;
}

void Game::ResolvePlayerTurn(Player::Pos pos) noexcept {
    if (_enm->Attack(pos))
        _turn++;

    if (_enm->HasLost()) {
        logging::info("Player won");
        _playing = false;
        _app.state.playerWon = true;
        _app.SetMenu<GameOverMenu>();
    }
}

void Game::ResolveEnemyTurn(Player::Pos pos) noexcept {
    if (_plr.Attack(pos))
        _turn++;

    if (_plr.HasLost()) {
        logging::info("Enemy won");
        _playing = false;
        _app.state.playerWon = false;
        _app.SetMenu<GameOverMenu>();
    }
}

void Game::EnemyTurn() {
    using namespace std::chrono_literals;

    static std::future<Player::Pos> posF;

    if (posF.valid() and posF.wait_for(1ms) == std::future_status::ready) {
        ResolveEnemyTurn(posF.get());
    } else {
        // TODO: Make it thread safe (_enm access, read/write)
        posF = std::async(std::launch::async, [enm = _enm.get(), grid = _plr.GetGrid()]() {
            return enm->MakeTurn(grid);
        });
    }
}
