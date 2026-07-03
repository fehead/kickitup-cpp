/*
 * stepdata.h - Step/note data from .ksf files
 */

#ifndef __STEPDATA_H__
#define __STEPDATA_H__

#include <cstdint>

#define MAX_DATA 2048
#define STEP_LEN 14

struct StepData {
    char   data[MAX_DATA + 1][STEP_LEN];
    char   judge[MAX_DATA + 1][STEP_LEN];
    double y[MAX_DATA + 1];

    void   clear();

    /* Populate from a STEP_NEW struct (output of ReadKSF) */
    void   fromStepNew(const void *stepNew, double bpm, int tick);

    /* Populate y[] positions from step data and BPM */
    void   buildY(double bpm, int bunki, int bunki2);

    /* Apply per-player speed to y[] positions */
    void   applySpeed(int mul);

    int    count() const;
};

extern StepData g_step1, g_step2, g_stepDouble;

#endif
