#include <iostream>
#include <ctime>
#include "game.h"
#include "menu.h"
#include "lvlchoose.h"
#include "settings.h"
#include "graphics.h"



IMAGE *background, *puzzle, *field,*curr_part;

Detail array[MAX_SIZE][MAX_SIZE];
// - Это сам пазл
int mask[MAX_SIZE][MAX_SIZE];
// А  это поле.
bool complete = false;


void game() {
   load_images();
   create_puzzle();
   mixing();
   while (true) {
      cleardevice();
      put_secondary();
      draw_field();
      put_details();
      if (mousebuttons() == 0) {
      /*Если ЛКМ не нажата, то мы делаем вставку по координатам
         (фактически деталь пазла примагничивается к полю)
         */
         is_field();
      }
      else if (mousebuttons() == 1) {
         //  это ЛКМ. Проверка на нажатие ЛКМ
         drag_detail();
      }
      swapbuffers();
      if (is_end()) {
         draw_end();
         swapbuffers();
         complete = true;
         getch();
         break;
      }
      else if(kbhit() && getch() == KEY_ESC)
         break;
   }
   clear_all();
   lvl = -1;
}

void load_images() {
   // Загрузка изображений.
   background = loadBMP(("levels/lvl" + std::to_string(lvl+1) + "/backg.jpg").c_str());
   field = loadBMP("levels/field.jpg");
   puzzle = loadBMP(("levels/lvl" + std::to_string(lvl+1) +  "/puzzle.jpg").c_str());
   curr_part = imageresize(loadBMP("levels/curr_part.bmp"), size, size);
}

void put_secondary() {
   // Вставка заднего фона
   putimage(0, 0, background, COPY_PUT);
   setfillstyle(SOLID_FILL, BLACK);

   bar(968, 63, 1178, 273);
   // Зарисовка рамки для примера. Добавляет красоту и отделяет пример от фона.
   putimage(974, 68, puzzle, COPY_PUT, 200, 200);
   // Эта часть кода вставляет пример, по которому собираем основной паззл.
   putimage(968, 10, lvl_change[6][f], TRANSPARENT_PUT);
   // Вставка текста. Сделана для того, что бы удобнее работать с разными языками.
   putimage(0, 800, lvl_change[3][f], TRANSPARENT_PUT);
   // Параметр TRANSPARENT_PUT убирает черный фон, на котором написан текст
}


void create_puzzle() {
   // Эта функция разбивает основное изображение на детали пазла.
   srand(time(NULL));
   int cur_iter = 0;
   for (int i = 0; i < diff; i++)
      for (int j = 0; j < diff; j++) {
         int x = (rand() % (MAGIC_NUM * diff)) +WORKPLACE_X;
         int y = (rand() % (MAGIC_NUM * diff)) + WORKPLACE_Y;
         // Промежуток координат, куда попадают пазлы при запуске уровня.
         Detail temp{x, y, cur_iter,
            imagecopy(puzzle, i * size, j * size, size, size), 1};
         array[i][j] = temp;
         mask[i][j] = cur_iter++;
      }
}


void mixing() {
   // Эта функция сортирует массив по рандомным числам
   for (int i = 0; i < diff; i++)
      for (int j = 0; j < diff; j++) {
         int rand_i = rand() % diff,rand_j = rand() % diff;
         Detail temp = array[i][j];
         array[i][j] = array[rand_i][rand_j];
         array[rand_i][rand_j] = temp;
      }
}


void put_details() {
   // Эта функция ставит детальки по их координатам
   for (int i = 0; i < diff; i++)
      for (int j = 0; j < diff; j++)
         putimage(array[i][j].pos_x, array[i][j].pos_y, array[i][j].image, COPY_PUT);
}


void draw_field() {
   // Эта функция рисует поле diff x diff количества серых квадратов
   putimage(FIELD_X - 10, FIELD_Y - 10, field, COPY_PUT);
   for (int i = 0; i < diff; i++)
      for (int j = 0; j < diff; j++) {
         int m_x = mousex(), m_y = mousey();
         if (m_x > i * size && m_x < size * (i + 1) && m_y > j * size && m_y < size * (j + 1)) {
            putimage(i * size + FIELD_X,j * size + FIELD_Y, curr_part, COPY_PUT);
         }
      }
}


void drag_detail() {
   // Эта функция берет детальку, передвигает ее в последнее место массива
   for (int i = diff - 1; i >= 0; i--) {
      for (int j = diff - 1; j >= 0; j--) {
         int m_x = mousex();
         int m_y = mousey();
         if (m_x > array[i][j].pos_x && m_x < array[i][j].pos_x + size
               && m_y > array[i][j].pos_y && m_y < array[i][j].pos_y + size) {
            array[i][j].move = 1;
            Detail temp = array[i][j];
            array[i][j] = array[diff-1][diff-1];
            array[diff-1][diff-1] = temp;
            array[diff-1][diff-1].pos_x = m_x - size / 2;
            array[diff-1][diff-1].pos_y = m_y - size / 2;
            return;
         }
      }
   }
}


void is_field() {
   /*Проверяет на поле. Если точнее, то он смотрит координаты детальки и поля. 
   Нужна для эффекта "примагничивания"
   */
   for (int i = 0; i < diff; i++) {
      for (int j = 0; j < diff; j++) {
         int temp_x = array[diff-1][diff-1].pos_x - size / 2;
         int temp_y = array[diff-1][diff-1].pos_y - size / 2;
         if (i * size + FIELD_X  > temp_x && i * size + FIELD_X <temp_x + size &&
            j * size + FIELD_Y > temp_y && j * size + FIELD_Y < temp_y + size) {
            array[diff-1][diff-1].pos_x = i * size + FIELD_X;
            array[diff-1][diff-1].pos_y =  j * size + FIELD_X;
            if (mask[i][j] == array[diff-1][diff-1].cur_num) {
               array[diff-1][diff-1].move = 0;
            }
            return;
         }
      }
   }
}

bool is_end() {
   /* Эта функция проверяет на конец. Условием конца является то, 
   что все детальки имеют параметр move = 0 (все детали в нужных местах)
   */
   for (int i = 0; i < diff; i++)
      for (int j = 0; j < diff; j++)
         if (array[i][j].move == 1)
            return 0;
   return 1;
}

void clear_all() {
   // Функция, которая отчищатет память от картинок
   freeimage(background);
   freeimage(puzzle);
   freeimage(field);
   freeimage(curr_part);
   for (int i = 0; i < diff; i++)
      for (int j = 0; j < diff; j++)
         freeimage(array[i][j].image);
}

void draw_end() {
   putimage(0, 0, background, COPY_PUT);
   putimage(250, 0, field, COPY_PUT);
   putimage(260, 10, puzzle, COPY_PUT);
   putimage(250, 810,  lvl_change[7][f], TRANSPARENT_PUT);
}
