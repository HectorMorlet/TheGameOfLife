
//
// theGameOfLife.c
// The Game Of Life by John Conway generated using ncurses
// By Hector Morlet
// 19 April 2014
//


#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include <math.h>
#include <ncurses.h>
#include <unistd.h>

#include "theGameOfLife.h"


#define TRUE 1
#define FALSE 0

#define liveCellChar '#'
#define deadCellChar ' '

#define delay 50000


cell *createMap(cell *map, int width, int height);
void renderMap(cell *map, int width, int height);

void iterateGeneration();
void isAlive();

void iterateGeneration();

void initCurses();


// .oO0-------------------------------------------------------0Oo. //
// ----------------------------- Main ---------------------------- //
// .oO0-------------------------------------------------------0Oo. //

int main(int argc, char *argv[]) {
   initCurses();

   int height;
   int width;
   getmaxyx(stdscr, height, width);
   cell *map = malloc(100000); // Work the size of thew map out
   map = createMap(map, width, height);

   renderMap(map, width, height);
   refresh();
   usleep(5000000);

   endwin();

   return EXIT_SUCCESS;
}


cell *createMap(cell *map, int width, int height) {
   int i = 0;
   while (i < height * width) {
      map[i].changeNextGeneration = FALSE;
      map[i].lifeStatus = FALSE;
      if (i == 20 || i == 21 || i == 360 || i == 61) {
         map[i].lifeStatus = TRUE;
      }
      i++;
   }

   return map;
}


void renderMap(cell *map, int width, int height) {

   int y = 0;
   while (y < height) {
      int x = 0;
      while (x < width) {
         move(y, x);
         if (map[y * width + x].lifeStatus) {
            addch(liveCellChar);
         }
         x++;
      }
      y++;
   }
}


void iterateGeneration() {
   int i = 0;
   while (i < 1000) {
      
   }
}


void isAlive() {

}


// .oO0--------------------------------------------------------0Oo. //
// -------------------------- Init curses ------------------------- //
// .oO0--------------------------------------------------------0Oo. //

void initCurses() {
   initscr();
   cbreak();
   noecho();
}
