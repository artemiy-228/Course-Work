#ifndef MENU_H
#define MENU_H

#define COUNT_L 8

#include "graphics.h"

struct Button{
   int pos_x;
   int pos_y;
   int size_x;
   int size_y;
   IMAGE *image;
   IMAGE *image2;
};

extern bool f;


enum butt {GAME = 0, EXIT, INFO, SETTINGS, N_BUTTONS};

//Массив  с  текстом  на  двух  языках
extern IMAGE *lvl_change[COUNT_L][2];
extern IMAGE *backg, *text, *quote;
extern Button button[N_BUTTONS];

void start();
void menu();
void load();
void draw_backg();
void define_array(int pos, int x, int y, IMAGE *img, Button*);
void draw_menu();
int choose_button(const int, Button*);
void draw_info();
void clear_menu();

#endif