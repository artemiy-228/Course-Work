#include <iostream>
#include "menu.h"
#include "game.h"
#include "lvlchoose.h"
#include "graphics.h"


Button levels[MAX_LEVEL];
int lvl = -1;

void lvl_choose(){
   delay(100);
   define_levels();
   while(lvl == -1){
      draw_levels();
      swapbuffers();
      lvl = choose_button(MAX_LEVEL, levels);
      if(kbhit() && getch() == KEY_ESC)
         return;
      if(lvl != -1){
         game();
         if(complete == true){
            complete = false;
            return;
         }
      }
   }
}

void draw_levels(){
   draw_backg();
   for(int i = 0;i < MAX_LEVEL;i++){
      putimage(levels[i].pos_x, levels[i].pos_y, levels[i].image, COPY_PUT);
   }
}


void define_levels(){
   // Это страшный код просто делает конкатенацию для создания пути и потом преобразует 
   // в char, тк loadBMP принимает char аргумент, а не string
   for(int i = 0; i < MAX_LEVEL;i++){
      std::string path = "buttons/lvls/lvl" + std::to_string(i + 1) + ".bmp";
      define_array(i, 200 * ((i % 5) + 1), 200 * ((i / 5) + 1), imageresize(loadBMP(path.c_str()), 150, 150), levels);
   }
}

void clear_levels(){
   for(int i =0; i < MAX_LEVEL;i++){
      freeimage(levels[i].image);
   }
}

