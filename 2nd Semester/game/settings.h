#ifndef SETTINGS_H
#define SETTINGS_H
#include "graphics.h"

extern int diff, size;
enum difficult {EASY =0, MEDIUM, HARD, IMPOSSIBLE, ENG, RUS, C_BUTTONS};


void settings();
void define_field();
void put_field();
void clear_image();
int choose_diff();
void choose_language();

#endif
