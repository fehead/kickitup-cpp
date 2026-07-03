#include "main.h"
#include "game.h"
#include "config.h"

Game g_game;

Game::Game() : _state(GS_TITLE), _mode(GS_MODE_HARD),
    _couple(false), _double(false),
    _songFlag(false), _introFlag(false), _active(true) {}

void Game::update() {
    switch (g_ProgramState) {
    case GS_TITLE:     StageTitle();       break;
    case GS_SELECTSONG: SelectSong();      break;
    case GS_STAGE1:     KIU_STAGE();       break;
    case GS_DOUBLE:     KIU_STAGE_DOUBLE(); break;
    case GS_COUPLE:     KIU_STAGE();       break;
    case GS_DEAD:       Dead();            break;
    case GS_CONFIG:     Configuration();   break;
    case GS_RESULT:     Result();          break;
    case GS_GAMEOVER:   GameOver1();       break;
    case GS_END:        g_quitRequested = 1; break;
    default: break;
    }
}
