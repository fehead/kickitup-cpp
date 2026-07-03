/*
 * audio.cpp - AudioManager implementation
 */

#include "audio.h"

AudioManager g_audio;
Sound *g_dsOpening, *g_dsDead, *g_dsMode, *g_dsCancel;
Sound *g_dsMove, *g_dsBeat, *g_dsSelectSong;

AudioManager::AudioManager()
    : _opening(nullptr), _dead(nullptr), _mode(nullptr), _cancel(nullptr),
      _move(nullptr), _beat(nullptr), _selSong(nullptr),
      _intro(nullptr), _songBg(nullptr) {}

AudioManager::~AudioManager() { unloadAll(); }

bool AudioManager::loadAll() {
    _opening = Sound::LoadMP3("wave/opening.mp3");
    _dead    = Sound::LoadMP3("wave/dead.mp3");
    _mode    = Sound::LoadMP3("wave/mode.mp3");
    _cancel  = Sound::LoadMP3("wave/cancel.mp3");
    _move    = Sound::LoadMP3("wave/move.mp3");
    _beat    = Sound::LoadMP3("wave/beat.mp3");
    _selSong = Sound::LoadMP3("wave/musicSelect.mp3");

    g_dsOpening    = _opening;
    g_dsDead       = _dead;
    g_dsMode       = _mode;
    g_dsCancel     = _cancel;
    g_dsMove       = _move;
    g_dsBeat       = _beat;
    g_dsSelectSong = _selSong;

    return _opening && _dead && _mode && _cancel && _move && _beat && _selSong;
}

void AudioManager::unloadAll() {
    if (_opening) { _opening->Stop(); delete _opening; _opening = nullptr; }
    if (_dead)    { _dead->Stop();    delete _dead;    _dead = nullptr; }
    if (_mode)    { _mode->Stop();    delete _mode;    _mode = nullptr; }
    if (_cancel)  { _cancel->Stop();  delete _cancel;  _cancel = nullptr; }
    if (_move)    { _move->Stop();    delete _move;    _move = nullptr; }
    if (_beat)    { _beat->Stop();    delete _beat;    _beat = nullptr; }
    if (_selSong) { _selSong->Stop(); delete _selSong; _selSong = nullptr; }
    if (_intro)   { _intro->OnMediaStop(); delete _intro; _intro = nullptr; }
    if (_songBg)  { _songBg->OnMediaStop(); delete _songBg; _songBg = nullptr; }
    g_dsOpening = g_dsDead = g_dsMode = g_dsCancel = nullptr;
    g_dsMove = g_dsBeat = g_dsSelectSong = nullptr;
}
