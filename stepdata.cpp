/*
 * stepdata.cpp - StepData implementation
 *  ksf parsing helper + data management
 */

#include "stepdata.h"
#include <cstring>
#include <cmath>

StepData g_step1, g_step2, g_stepDouble;

void StepData::clear() {
    std::memset(data, 0, sizeof(data));
    std::memset(judge, 0, sizeof(judge));
    std::memset(y, 0, sizeof(y));
}

void StepData::fromStepNew(const void *stepNew, double bpm, int tick) {
    clear();

    /* STEP_NEW layout: char step[2048][14], then double BPM/BPM2/BPM3, int start/start2/start3/tick/bunki/bunki2, int _dummy, char name[64] */
    const char *src = (const char *)stepNew;
    double bpmCount = 60.0 / bpm * 100.0;
    int bunki = ((const int *)(src + 2048*14 + sizeof(double)*3))[3 + 1];  // after BPM,BPM2,BPM3,start,start2,start3,tick → bunki

    /* Copy step data */
    for (int i = 0; i < MAX_DATA; i++) {
        const char *row = src + i * STEP_LEN;
        bool hasNote = false;
        for (int j = 0; j < STEP_LEN; j++) {
            data[i][j] = row[j];
            judge[i][j] = row[j];
            if (row[j] != '0') hasNote = true;
        }
        if (!hasNote) break;
    }

    /* Build Y positions from BPM timing */
    int bunki2 = ((const int *)(src + 2048*14 + sizeof(double)*3))[3 + 2];
    buildY(bpm, bunki, bunki2);
}

void StepData::buildY(double bpm, int bunki, int bunki2) {
    double bpmCount = 60.0 / bpm * 100.0;
    double accY = 0.0;

    for (int i = 0; i < MAX_DATA; i++) {
        if (data[i][0] == '\0') break;
        y[i] = accY;
        if (bunki > 0 && i == bunki - 1) {
            bpmCount = 60.0 / bpm * 100.0; // reset at bunki point
        }
        accY += bpmCount;
    }
}

void StepData::applySpeed(int mul) {
    double factor = mul / 100.0;
    for (int i = 0; i < MAX_DATA; i++) {
        if (data[i][0] == '\0') break;
        y[i] *= factor;
    }
}

int StepData::count() const {
    int n = 0;
    for (; n < MAX_DATA; n++)
        if (data[n][0] == '\0') break;
    return n;
}
