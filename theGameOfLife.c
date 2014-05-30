
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

#define SIZE_OF_A_CELL 15
#define SIZE_OF_AN_INT 4
#define MAX_INPUT_COORDS 2000
#define BUFFER 1
#define MAX_COORD_VALUE 1000000000
#define NUMBERS_PER_COORD 2

#define LIVE_CELL_CHAR '#'
#define DEAD_CELL_CHAR ' '
#define NEWLINE_CHARACTER '\n'
#define END_STRING_CHARACTER '\0'

#define DELAY_PER_GENERATION 600000
#define ITERATIONS 1000
#define INTRO_DELAY_PER_GENERATION 400000
#define INTRO_ITERATIONS 5

#define NEIGHBOURS_PER_CELL 8
#define MAX_DISTANCE_TO_SURROUNDING_CELL 1

#define FIRST_NUMBER_OF_SURROUNDING_LIVE_CELLS 2
#define SECOND_NUMBER_OF_SURROUNDING_LIVE_CELLS 3
#define NUMBER_OF_CELLS_FOR_DEAD_TO_LIVING 3

#define INTRO_TEXT "######## ##     ## ########     ######      ###    ##     ## ########\n   ##    ##     ## ##          ##    ##    ## ##   ###   ### ##      \n   ##    ##     ## ##          ##         ##   ##  #### #### ##      \n   ##    ######### ######      ##   #### ##     ## ## ### ## ######  \n   ##    ##     ## ##          ##    ##  ######### ##     ## ##      \n   ##    ##     ## ##          ##    ##  ##     ## ##     ## ##      \n   ##    ##     ## ########     ######   ##     ## ##     ## ########\n                                                                     \n #######  ########    ##       #### ######## ########                \n##     ## ##          ##        ##  ##       ##                      \n##     ## ##          ##        ##  ##       ##                      \n##     ## ######      ##        ##  ######   ######                  \n##     ## ##          ##        ##  ##       ##                      \n##     ## ##          ##        ##  ##       ##                      \n #######  ##          ######## #### ##       ########                \0"
#define INTRO_TEXT_WIDTH 70
#define INTRO_TEXT_HEIGHT 14
#define LAST_COLOR 6
#define POINTS_IN_INTRO_TEXT 364


// .oO0-------------------------------------------------------0Oo. //
// ----------------------------- Main ---------------------------- //
// .oO0-------------------------------------------------------0Oo. //

int main(int argc, char *argv[]) {
   int *input = getInput();

   initCurses();

   // Finding terminal dimentions
   int height;
   int width;
   getmaxyx(stdscr, height, width);

   // Creating map
   cell *map = malloc(width * height * SIZE_OF_A_CELL);
   map = createMap(map, width, height, input);

   // Iterating over the generations and rendering
   iterateGeneration(map, width, height);

   endwin();

   return EXIT_SUCCESS;
}


// .oO0-------------------------------------------------------0Oo. //
// ------------------------ Getting input ------------------------ //
// .oO0-------------------------------------------------------0Oo. //

int *getInput() {
   int *coords = NULL;

   // Determining whether to use default or input
   char choiceChar = 'n';
   printf("Would you like to enter your own coordinates? (y/n): ");
   scanf(" %c", &choiceChar);
   if (choiceChar == 'y') {
      // Allocating memory
      coords = malloc(MAX_INPUT_COORDS * SIZE_OF_AN_INT + BUFFER);

      // Instructions
      printf("Enter an x and y to make a point at their coordinates. ");
      printf("The maximum amount of input coordinate sets is %d.\n", 
         MAX_INPUT_COORDS / NUMBERS_PER_COORD);

      // Getting the input
      int coord = 0;
      char exitChar = 'y';
      int i = 0;
      while (exitChar == 'y' && i < MAX_INPUT_COORDS) {
         // Getting and setting x
         printf("Enter x: ");
         scanf("%d", &coord);
         assert(coord < MAX_COORD_VALUE);
         coords[i] = coord;

         // Getting and setting y
         printf("Enter y: ");
         scanf("%d", &coord);
         assert(coord < MAX_COORD_VALUE);
         coords[i + 1] = coord;

         // Determining whether to exit or not
         printf("Do you want to enter another point? (y/n): ");
         scanf(" %c", &exitChar);
         i += NUMBERS_PER_COORD;
      }

      // Setting terminating value
      coords[i] = MAX_COORD_VALUE;
   } else {
      // Making no-go coord array
      coords = malloc(SIZE_OF_AN_INT);
      coords[0] = MAX_COORD_VALUE;
   }

   return coords;
}


// .oO0-------------------------------------------------------0Oo. //
// ---------------- Iterating Through Generations ---------------- //
// .oO0-------------------------------------------------------0Oo. //

void iterateGeneration(cell *map, int width, int height) {
   int *introInput = malloc(POINTS_IN_INTRO_TEXT);
   introInput = translateIntroText(introInput, 
                                   INTRO_TEXT, 
                                   INTRO_TEXT_WIDTH);

   cell *introMap = malloc(width * height * SIZE_OF_A_CELL);
   introMap = createMap(introMap, width, height, introInput);

   int i = 0;
   while (i < INTRO_ITERATIONS) {
      renderMap(introMap, width, height);
      introMap = updateMap(introMap, width, height);

      if (i == 0) {
         move(INTRO_TEXT_HEIGHT + BUFFER, 0);
         attron(COLOR_PAIR(LAST_COLOR));
         addstr("Press any key to continue.");
         attroff(COLOR_PAIR(LAST_COLOR));
         getch();
      }

      i++;
      usleep(INTRO_DELAY_PER_GENERATION);
   }

   i = 0;
   while (i < ITERATIONS) {
      renderMap(map, width, height);
      map = updateMap(map, width, height);

      i++;
      usleep(DELAY_PER_GENERATION);
   }
}


// .oO0-------------------------------------------------------0Oo. //
// ------------------- Translating intro input ------------------- //
// .oO0-------------------------------------------------------0Oo. //

int *translateIntroText(int *introArray, char *introText, int width) {
   char currChar = LIVE_CELL_CHAR;
   int i = 0;
   int newLineCount = 0;
   int arrayPos = 0;
   while (currChar != END_STRING_CHARACTER) {
      currChar = introText[i];
      if (currChar == NEWLINE_CHARACTER) {
         newLineCount++;
      } else if (currChar == LIVE_CELL_CHAR) {
         introArray[arrayPos] = i % width;
         introArray[arrayPos + BUFFER] = newLineCount;

         arrayPos += NUMBERS_PER_COORD;
      }
      i++;
   }

   introArray[i] = MAX_COORD_VALUE;

   return introArray;
}


// .oO0-------------------------------------------------------0Oo. //
// ---------------- Creating and Rendering the Map --------------- //
// .oO0-------------------------------------------------------0Oo. //

void renderMap(cell *map, int width, int height) {
   int y = 0;
   while (y < height) {
      int x = 0;
      while (x < width) {
         // Moving the curser
         move(y, x);

         // Printing living cell if it is living, dead one if otherwise
         if (map[y * width + x].lifeStatus) {
            // Testing if the cell's color is bellow the max color
            if (map[y * width + x].color <= 7) {
               // Using cell's color
               attron(COLOR_PAIR(map[y * width + x].color));
               addch(LIVE_CELL_CHAR);
               attroff(COLOR_PAIR(map[y * width + x].color));
            } else {
               // Using the max color
               attron(COLOR_PAIR(7));
               addch(LIVE_CELL_CHAR);
               attroff(COLOR_PAIR(7));
            }
         } else {
            addch(DEAD_CELL_CHAR);
         }

         x++;
      }
      y++;
   }

   refresh();
}


cell *createMap(cell *map, int width, int height, int *input) {
   int i = 0;
   while (i < height * width) {
      // Setting the default status of the cell
      map[i].lifeStatNextGeneration = FALSE;
      map[i].lifeStatus = FALSE;
      map[i].color = 0;

      // The following condition (the second part of it) is completely
      // derogatory (the numbers don't mean anything).
      // Fiddle around with it for pretty patterns!
      if (input[0] == MAX_COORD_VALUE && i*3827022/5%3 == 0) {
         map[i].lifeStatus = TRUE;
      } else {
         // Getting the x and y values of the cell
         int cellX = i % width;
         int cellY = i / width;

         // Looping over each coord
         int inputX = 0;
         int j = 0;
         while (inputX != MAX_COORD_VALUE) {
            inputX = input[j];

            // Comparing to the cell's x and y
            if (input[j] == cellX && input[j + 1] == cellY) {
               // If they are the same, the cell is set to alive
               map[i].lifeStatus = TRUE;
            }

            j += NUMBERS_PER_COORD;
         }
      }
      i++;
   }

   return map;
}


// .oO0-------------------------------------------------------0Oo. //
// ---------------- Updating to the New Generation --------------- //
// .oO0-------------------------------------------------------0Oo. //

cell *updateMap(cell *map, int width, int height) {
   // Looping over each cell
   int i = 0;
   while (i < (width * height)) {
      // Getting the cell's number of neighbours and changing its status
      int neighbours = cellNeighbours(map, i, width);
      map = updateCell(map, i, neighbours);
      i++;
   }

   i = 0;
   while (i < (height * width)) {
      // Testing how to change the color
      if (map[i].lifeStatNextGeneration == TRUE &&
         map[i].lifeStatus == FALSE) {
         // If the cell is becoming alive, set to first color
         map[i].color = TRUE;
      } else if (map[i].lifeStatNextGeneration == TRUE) {
         // If the cell was alive and is still
         // alive, add one to their color
         map[i].color++;
      } else if (map[i].lifeStatNextGeneration == FALSE) {
         // If the cell is going to be dead, 
         // set their color to the dead color
         map[i].color = FALSE;
      }

      // Setting all the cells' lifestatuses to what they should be
      // This is to avoid the error of changing one status in the function
      // above, which will affect the status of other cells, when they
      // should all use the same data to change.
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
   // Determining if the cell is alive or dead
   // Different rules apply to each (see plan.txt/go to wikipedia)
   if (map[cellPosition].lifeStatus == TRUE) {
      // If the cell doesn't have 2 or 3 neighbours, it dies
      if (neighbours != FIRST_NUMBER_OF_SURROUNDING_LIVE_CELLS && 
         neighbours != SECOND_NUMBER_OF_SURROUNDING_LIVE_CELLS) {
         map[cellPosition].lifeStatNextGeneration = FALSE;
      }
   } else {
      // If the cell has exactly 3 live neighbours, it comes alive
      if (neighbours == NUMBER_OF_CELLS_FOR_DEAD_TO_LIVING) {
         map[cellPosition].lifeStatNextGeneration = TRUE;
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

   start_color();
   init_pair(0, COLOR_BLACK, COLOR_BLACK);
   init_pair(6, COLOR_WHITE, COLOR_BLACK);
   init_pair(2, COLOR_YELLOW, COLOR_BLACK);
   init_pair(3, COLOR_GREEN, COLOR_BLACK);
   init_pair(7, COLOR_CYAN, COLOR_BLACK);
   init_pair(5, COLOR_BLUE, COLOR_BLACK);
   init_pair(1, COLOR_RED, COLOR_BLACK);
   init_pair(4, COLOR_MAGENTA, COLOR_BLACK);
}
