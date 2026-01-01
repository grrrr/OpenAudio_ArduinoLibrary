/*
 * AudioFilterBiquad_F32.cpp
 *
 * Chip Audette, OpenAudio, Apr 2017
 * MIT License,  Use at your own risk.
 *
 * This filter has been running in F32 as a single stage.  This
 * would work by using multiple instantations, but compute time and
 * latency suffer.  So, Feb 2021 convert to MAX_STAGES 4 as is the I16
 * Teensy Audio library.    Bob Larkin
 *
 * See AudioFilterBiquad_F32.h for more notes.
*/
#include "AudioFilterBiquad_F32.h"
