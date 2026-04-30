#include "Game.h"

#include "Application.h"
#include "Interface/GameOverMenu.h"
#include "Player.h"
#include "Enemy.h"
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
      _enm(std::move(enm)) {
        logging::info("Initializing Game");
      }

Game::~Game() {
    logging::info("Closing game");
    
    // wait for std::future to finish its task
    // prevents use after free (hopefully)
    if (_enmAttackFuture.valid()) {
        logging::info("Waiting for enemy attack to resolve");
        _enmAttackFuture.wait();
        logging::info("Enemy attack resolved");
    }
}


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

void Game::ResolvePlayerTurn(Pos pos) {
    if (_enm->Attack(pos) == Player::AttackResult::Missed)
        _turn++;

    if (_enm->HasLost()) {
        logging::info("Player won");
        _playing = false;
        _app.state.playerWon = true;
        _app.SetMenu<GameOverMenu>();
    }
}

void Game::ResolveEnemyTurn(Pos pos) {
    if (_plr.Attack(pos) == Player::AttackResult::Missed)
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

    if (not _enmAttackFuture.valid()) {
        // create new attack future
        _enmAttackFuture = std::async(std::launch::async, [this, snapshot = Player::RemoveShips(_plr.GetGrid())]() {
            // MakeTurn is thread safe (stateless)
            // it uses just the grid snapshot
            return this->_enm->MakeTurn(snapshot);
        });
    } else if (_enmAttackFuture.wait_for(1ms) == std::future_status::ready) {
        // if ready, attack
        ResolveEnemyTurn(_enmAttackFuture.get());
    } else {
        // if awaiting for decision continue to next frame
    }
}
