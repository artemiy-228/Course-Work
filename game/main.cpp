#include "menu.h"
#include "settings.h"
#include "lvlchoose.h"
#include "graphics.h"

int main(){
   initwindow(1300, 850, "Puzzle");
   load();
   define_field();
   menu();
   clear_image();
   clear_levels();
   clear_menu();
}