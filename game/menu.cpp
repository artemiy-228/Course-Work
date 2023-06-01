#include "settings.h"
#include "menu.h"
#include "lvlchoose.h"
#include "game.h"

#include "graphics.h"

IMAGE *backg, *quote, *info, *r_game, *r_exit;
IMAGE *lvl_change[COUNT_L][2];
Button button[N_BUTTONS];
bool f = 1;

void menu() {
   /* Я не стал описывать отдельно функцию start
         По причине одинакового функционала с
         draw_backg();
   */
   draw_backg();
   getch();
   while (true) {
      cleardevice();
      draw_menu();
      swapbuffers();
      if ((mousebuttons() == 1)) {
         int statement = choose_button(N_BUTTONS, button);
         switch (statement) {
         case GAME:
            lvl_choose();
            break;
         case EXIT:
            return;
         case INFO:
            draw_info();
            break;
         case SETTINGS:
            settings();
         }
      }
   }
}

void load() {

   backg = loadBMP("levels/backg.jpg");
   quote = loadBMP("levels/quote.bmp");
   info = loadBMP("levels/info.jpg");
   r_game = loadBMP("buttons/r_game.bmp");
   r_exit = loadBMP("buttons/r_exit.bmp");
   
   const char* languages[2] = {"eng", "rus"};
   const char* lvl_names[8] = {"settings", "difficulty", "language", "info",
      "game", "exit", "example", "end"};
   for (int i = 0; i < 2; i++) {
      for (int j = 0; j < 8; j++) {
         char file_name[100];
         sprintf(file_name, "language/%s/%s.bmp", 
         languages[i], lvl_names[j]);
         lvl_change[j][i] = loadBMP(file_name);
      }
   }

   define_array(GAME, 525, 300,
   loadBMP("buttons/game.bmp"), button);
   define_array(EXIT, 525, 500, 
   loadBMP("buttons/exit.bmp"), button);
   define_array(INFO, 1200, 700, 
   loadBMP("buttons/info.bmp"), button);
   define_array(SETTINGS, 1200, 600, 
   loadBMP("buttons/settings.bmp"), button);
}

//Определение элемента массива(кнопки)
void define_array(int pos, int x, int y, IMAGE *img, 
Button *array) {

   Button temp{x, y, imagewidth(img), imageheight(img), img};
   array[pos] = temp;
}

//Отрисовка заднего фона
void draw_backg() {
   putimage(0, 0, backg, COPY_PUT);
   putimage(0, 800, lvl_change[3][f], TRANSPARENT_PUT);
   putimage(0, 0, quote, TRANSPARENT_PUT);
}

//Отрисовка Меню
void draw_menu() {
   draw_backg();

   for (int i =0; i < N_BUTTONS; i++) {
      putimage(button[i].pos_x, button[i].pos_y, button[i].image, 
      COPY_PUT);
   }
   if(f == 1){
       putimage(button[0].pos_x, button[0].pos_y, r_game, 
      COPY_PUT);
      putimage(button[1].pos_x, button[1].pos_y, r_exit, 
      COPY_PUT);      
   }
}

//Отрисовка окна «Информация»
void draw_info() {
   putimage(0, 0, info, COPY_PUT);
   putimage(0, 800, lvl_change[3][f], TRANSPARENT_PUT);
   putimage(0, 0, quote, TRANSPARENT_PUT);
   swapbuffers();
   getch();
}


//Выбор кнопки
int choose_button(const int N, Button *array) {
   int state = -1;
   if (mousebuttons() == 1) {
      int x = mousex();
      int y = mousey();


      for (int i = 0; i < N; i++) {
         if (x > array[i].pos_x &&
               x < array[i].pos_x + array[i].size_x &&
               y > array[i].pos_y &&
               y < array[i].pos_y + array[i].size_y)
         {
            state = i;
            break;
         }
      }
   }
   return state;
}

void clear_menu(){
   freeimage(r_exit);
   freeimage(r_game);
   freeimage(backg);
   freeimage(quote);
   freeimage(info);
   for(int i = 0; i < N_BUTTONS;i++)
      freeimage(button[i].image);
}
