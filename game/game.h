#ifndef GAME_H
#define GAME_H

#define FIELD_X 20
#define FIELD_Y 20
#define FIELD_SIZE 800
#define WORKPLACE_X 890
#define WORKPLACE_Y 420
#define MAX_SIZE 5
#define MAGIC_NUM 25


#include "graphics.h"

struct Detail {
   int pos_x;
   int pos_y;
   int cur_num;
   IMAGE *image;
   bool move;
};


extern IMAGE *example, *end, *curr_part;
extern bool complete;

void game();
void load_images();
void put_secondary();
void create_puzzle();
void create_field();
void mixing();
void put_details();
void draw_field();
void drag_detail();
void is_field();
bool is_end();
void clear_all();
void draw_end();



#endif