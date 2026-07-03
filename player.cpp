/*
 * player.cpp - Player state implementation
 */

#include "player.h"
#include <cstring>

Player g_p1, g_p2;

Player::Player() { std::memset(this, 0, sizeof(*this)); gauge = 10; }

void Player::clearModes() {
    speedBase = 1;
    speed1 = speed3 = speed5 = speed7 = speed9 = 1;
    mirror = nonstep = synchro = union_ = random = dMix = false;
    vanish = crazy = suddenR = randomS = false;
}

void Player::resetStats() {
    perfect = great = good = bad = miss = 0;
    maxCombo = combo = 0; score = 0; gauge = 10;
}

char Player::judgeAnaly() {
    uint32_t total = perfect + great + good + bad + miss;
    if (gauge < 0) return 'F';
    if (good == 0 && bad == 0 && miss == 0) return 'S';
    if ((double)(perfect + great) / total * 100 > 95) return 'A';
    if ((double)(perfect + great) / total * 100 > 80) return 'B';
    if ((double)(perfect + great) / total * 100 > 60) return 'C';
    return 'F';
}
