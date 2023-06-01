#include <iostream>
#include "settings.h"
#include "game.h"
#include "menu.h"
#include "graphics.h"

IMAGE *sett, *diffic, *language, *set_bag, *pointer;
int diff = 2; // текстовый флаг для отображения выбраного языка.
int size = FIELD_SIZE / diff;
Button difficult[C_BUTTONS];

void settings() {
   //Основной цикл настроек
   while (true) {
      cleardevice();
      put_field();
      swapbuffers();
      diff = choose_diff();
      choose_language();
      size = FIELD_SIZE / diff;
      curr_part = imageresize(loadBMP("levels/curr_part.bmp"), size, size);
      if (kbhit() == 1)
         break;
   }
   getch();
}

void define_field() {
   //Создание всех нужных переменных
   set_bag = loadBMP("levels/settings.jpg");
   pointer = loadBMP("buttons/pointer.bmp");

   define_array(EASY, 185, 255, loadBMP("buttons/easy.bmp"), difficult);
   define_array(MEDIUM, 185, 345, loadBMP("buttons/medium.bmp"), difficult);
   define_array(HARD, 185, 435, loadBMP("buttons/hard.bmp"), difficult);
   define_array(IMPOSSIBLE, 185, 525, loadBMP("buttons/impossible.bmp"), difficult);
   define_array(ENG, 950, 260, loadBMP("language/eng/eng.bmp"), difficult);
   define_array(RUS, 950, 360, loadBMP("language/rus/rus.bmp"), difficult);
}

void put_field() {
   //Зарисовка всех объектов на экране
   setfillstyle(SOLID_FILL, WHITE);
   setcolor(WHITE);

   putimage(0, 0, set_bag, COPY_PUT);
   putimage(0, 800, lvl_change[3][f], TRANSPARENT_PUT);
   putimage(0, 0, quote, TRANSPARENT_PUT);
   putimage(425, 100, lvl_change[0][f], TRANSPARENT_PUT);
   putimage(150, 200, lvl_change[1][f], TRANSPARENT_PUT);
   putimage(850, 200, lvl_change[2][f], TRANSPARENT_PUT);

   putimage(difficult[diff - 2].pos_x - 20, difficult[diff-2].pos_y, pointer, TRANSPARENT_PUT);

   if (f == 0) {
      setfillstyle(SOLID_FILL, LIGHTGREEN);
      setcolor(LIGHTGREEN);
      bar(difficult[4].pos_x - 3, difficult[4].pos_y - 3,
          difficult[4].pos_x + difficult[4].size_x + 3, difficult[4].pos_y +  difficult[4].size_y + 3);

      setfillstyle(SOLID_FILL, WHITE);
      setcolor(WHITE);
      bar(difficult[5].pos_x - 3, difficult[5].pos_y - 3,
          difficult[5].pos_x + difficult[5].size_x + 3, difficult[5].pos_y +  difficult[5].size_y + 3);
   }
   else {
      setfillstyle(SOLID_FILL, LIGHTGREEN);
      setcolor(LIGHTGREEN);
      bar(difficult[5].pos_x - 3, difficult[5].pos_y - 3,
          difficult[5].pos_x + difficult[5].size_x + 3, difficult[5].pos_y +  difficult[5].size_y + 3);

      setfillstyle(SOLID_FILL, WHITE);
      setcolor(WHITE);
      bar(difficult[4].pos_x - 3, difficult[4].pos_y - 3,
          difficult[4].pos_x + difficult[4].size_x + 3, difficult[4].pos_y +  difficult[4].size_y + 3);
   }

   for (int i = 0; i < C_BUTTONS; i++) {
      putimage(difficult[i].pos_x, difficult[i].pos_y, difficult[i].image, COPY_PUT);
   }
}

void clear_image() {
   freeimage(sett);
   freeimage(diffic);
   freeimage(language);
   freeimage(set_bag);
}

int choose_diff() {
   // Выбор сложности
   int state = diff;
   if (mousebuttons() == 1) {
      int x = mousex();
      int y = mousey();

      for (int i = 0; i < C_BUTTONS - 2; i++) {
         if (x > difficult[i].pos_x &&
               x < difficult[i].pos_x + difficult[i].size_x &&
               y > difficult[i].pos_y &&
               y < difficult[i].pos_y + difficult[i].size_y)
         {
            state = i + 2;
            break;
         }
      }
   }
   return state;
}

void choose_language() {
   // Выбор языка
   if (mousebuttons() == 1) {
      int x = mousex();
      int y = mousey();
      if (x > difficult[5].pos_x &&
           x < difficult[5].pos_x + difficult[5].size_x &&
           y > difficult[5].pos_y &&
           y < difficult[5].pos_y + difficult[5].size_y)
      {
         f = 1;
      }
      else if (x > difficult[4].pos_x &&
           x < difficult[4].pos_x + difficult[4].size_x &&
           y > difficult[4].pos_y &&
           y < difficult[4].pos_y + difficult[4].size_y)
      {
         f = 0;
      }
   }
}