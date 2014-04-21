
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

#define SIZE_OF_A_CELL 10

#define LIVE_CELL_CHAR '#'
#define DEAD_CELL_CHAR ' '

#define DELAY_PER_GENERATION 500000
#define ITERATIONS 1000

#define NEIGHBOURS_PER_CELL 8
#define MAX_DISTANCE_TO_SURROUNDING_CELL 1

#define FIRST_NUMBER_OF_SURROUNDING_LIVE_CELLS 2
#define SECOND_NUMBER_OF_SURROUNDING_LIVE_CELLS 3
#define NUMBER_OF_CELLS_FOR_DEAD_TO_LIVING 3


void iterateGeneration(cell *map, int width, int height);

cell *createMap(cell *map, int width, int height);
void renderMap(cell *map, int width, int height);

cell *updateMap(cell *map, int width, int height);
int cellNeighbours(cell *map, int cellPosition, int width);
cell *updateCell(cell *map, int cellPosition, int neighbours);

void initCurses();


// .oO0-------------------------------------------------------0Oo. //
// ----------------------------- Main ---------------------------- //
// .oO0-------------------------------------------------------0Oo. //

int main(int argc, char *argv[]) {
   initCurses();

   // Finding terminal dimentions
   int height;
   int width;
   getmaxyx(stdscr, height, width);

   // Creating map
   cell *map = malloc(width * height * SIZE_OF_A_CELL);
   map = createMap(map, width, height);

   // Rendering
   iterateGeneration(map, width, height);

   endwin();

   return EXIT_SUCCESS;
}


// .oO0-------------------------------------------------------0Oo. //
// ---------------- Iterating Through Generations ---------------- //
// .oO0-------------------------------------------------------0Oo. //

void iterateGeneration(cell *map, int width, int height) {
   usleep(DELAY_PER_GENERATION);

   int i = 0;
   while (i < ITERATIONS) {
      renderMap(map, width, height);
      map = updateMap(map, width, height);

      i++;
      usleep(DELAY_PER_GENERATION);
   }
}


// .oO0-------------------------------------------------------0Oo. //
// ---------------- Creating and Rendering the Map --------------- //
// .oO0-------------------------------------------------------0Oo. //

void renderMap(cell *map, int width, int height) {
   int y = 0;
   while (y < height) {
      int x = 0;
      while (x < width) {
         move(y, x);
         if (map[y * width + x].lifeStatus) {
            addch(LIVE_CELL_CHAR);
         } else {
            addch(DEAD_CELL_CHAR);
         }
         x++;
      }
      y++;
   }
   refresh();
}


cell *createMap(cell *map, int width, int height) {
   int i = 0;
   while (i < height * width) {
      map[i].lifeStatNextGeneration = FALSE;
      map[i].lifeStatus = FALSE;
      if (i == 23 || 
         i == 18 || 
         i == 20 || 
         i == 21 || 
         i == 99 || 
         i == 100 ||
         i == 101 ||
         i == 180 ||
         i == 181 ||
         i == 105 ||
         i == 129 ||
         i == 185 ||
         i == 123 ||
         i == 483 ||
         i == 970 ||
         i == 264 ||
         i == 957 ||
         i == 274 ||
         i == 265 ||
         i == 287 ||
         i == 241 ||
         i == 207 ||
         i == 396 ||
         i == 69 ||
         i == 185 ||
         i == 106 ||
         i == 16 ||
         i == 96 ||
         i == 98 ||
         i == 97) {
         map[i].lifeStatus = TRUE;
      }
      i++;
   }

   return map;
}


// .oO0-------------------------------------------------------0Oo. //
// ---------------- Updating to the New Generation --------------- //
// .oO0-------------------------------------------------------0Oo. //

cell *updateMap(cell *map, int width, int height) {
   int i = 0;
   while (i < (width * height)) {
      int neighbours = cellNeighbours(map, i, width);
      map = updateCell(map, i, neighbours);
      i++;
   }
   i = 0;
   while (i < (height * width)) {
      map[i].lifeStatus = map[i].lifeStatNextGeneration;
      i++;
   }

   return map;
}


int cellNeighbours(cell *map, int cellPosition, int width) {
   // Finding the cell's neighbours
   cell cellNeighbours[NEIGHBOURS_PER_CELL] = {
      map[cellPosition - width - MAX_DISTANCE_TO_SURROUNDING_CELL],
      map[cellPosition - width],
      map[cellPosition - width + MAX_DISTANCE_TO_SURROUNDING_CELL],
      map[cellPosition - MAX_DISTANCE_TO_SURROUNDING_CELL],
      map[cellPosition + MAX_DISTANCE_TO_SURROUNDING_CELL],
      map[cellPosition + width - MAX_DISTANCE_TO_SURROUNDING_CELL],
      map[cellPosition + width],
      map[cellPosition + width + MAX_DISTANCE_TO_SURROUNDING_CELL]
   };

   // Finding how many neighbours are alive
   int neighbourCount = 0;
   int i = 0;
   while (i < NEIGHBOURS_PER_CELL) {
      if (cellNeighbours[i].lifeStatus == TRUE) {
         neighbourCount++;
      }
      i++;
   }

   return neighbourCount;
}


cell *updateCell(cell *map, int cellPosition, int neighbours) {
   if (map[cellPosition].lifeStatus == TRUE) {
      if (neighbours == FIRST_NUMBER_OF_SURROUNDING_LIVE_CELLS || 
         neighbours == SECOND_NUMBER_OF_SURROUNDING_LIVE_CELLS) {
         map[cellPosition].lifeStatNextGeneration = TRUE;
      } else {
         map[cellPosition].lifeStatNextGeneration = FALSE;
      }
   } else {
      if (neighbours == NUMBER_OF_CELLS_FOR_DEAD_TO_LIVING) {
         map[cellPosition].lifeStatNextGeneration = TRUE;
      } else {
         map[cellPosition].lifeStatNextGeneration = FALSE;
      }
   }

   return map;
}


// .oO0--------------------------------------------------------0Oo. //
// -------------------------- Init curses ------------------------- //
// .oO0--------------------------------------------------------0Oo. //

void initCurses() {
   initscr();
   cbreak();
   noecho();
}
