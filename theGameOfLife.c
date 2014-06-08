
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

#define YES 'y'
#define NO 'n'

#define DEROGATORY_EQUATION i*3827022/5%3

#define MAX_INPUT_COORDS 2000
#define BUFFER 1
#define TERMINATING_COORD -1
#define NUMBERS_PER_COORD 2

#define PROMPT_COORDS (\
   "Would you like to enter your own coordinates? (y/n): ")
#define PROMPT_X "Enter x: "
#define PROMPT_Y "Enter y: "
#define PROMPT_MORE_COORDS (\
   "Do you want to enter another point? (y/n): ")

#define INSTRUCTION_XY (\
   "Enter an x and y to make a point at their coordinates. ")
#define INSTRUCTION_MAX_INPUT ("The maximum amount of input coordinate"\
   " sets is 1000.\n")

#define KEY_CONTINUE "Press any key to continue."

#define LIVE_CELL_CHAR '#'
#define DEAD_CELL_CHAR ' '

#define DELAY_PER_GENERATION 600000
#define ITERATIONS 1000
#define INTRO_DELAY_PER_GENERATION 400000
#define INTRO_ITERATIONS 5

#define NEIGHBOURS_PER_CELL 8
#define MAX_DISTANCE_TO_SURROUNDING_CELL 1

#define FIRST_NUMBER_OF_SURROUNDING_LIVE_CELLS 2
#define SECOND_NUMBER_OF_SURROUNDING_LIVE_CELLS 3
#define NUMBER_OF_CELLS_FOR_DEAD_TO_LIVING 3

#define INTRO_TEXT (\
"######## ##     ## ########    ######      ###    ##     ## #######\n"\
"   ##    ##     ## ##         ##    ##    ## ##   ###   ### ##     \n"\
"   ##    ##     ## ##         ##         ##   ##  #### #### ##     \n"\
"   ##    ######### ######     ##   #### ##     ## ## ### ## ###### \n"\
"   ##    ##     ## ##         ##    ##  ######### ##     ## ##     \n"\
"   ##    ##     ## ##         ##    ##  ##     ## ##     ## ##     \n"\
"   ##    ##     ## ########    ######   ##     ## ##     ## #######\n"\
"                                                                   \n"\
" #######  ########    ##       #### ######## ########              \n"\
"##     ## ##          ##        ##  ##       ##                    \n"\
"##     ## ##          ##        ##  ##       ##                    \n"\
"##     ## ######      ##        ##  ######   ######                \n"\
"##     ## ##          ##        ##  ##       ##                    \n"\
"##     ## ##          ##        ##  ##       ##                    \n"\
" #######  ##          ######## #### ##       ########              \0")
#define INTRO_TEXT_WIDTH 68
#define INTRO_TEXT_HEIGHT 14
#define LAST_COLOR 6
#define POINTS_IN_INTRO_TEXT 364
#define MAX_INPUT_WIDTH 500
#define MAX_INPUT_HEIGHT 200


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
   cell *map = malloc(width * height * sizeof(cell));
   map = createMap(map, width, height, input);

   // Map has been created - no more use for input
   free(input);

   // Iterating over the generations and rendering
   iterateGeneration(map, width, height);

   // All done
   free(map);
   endwin();

   return EXIT_SUCCESS;
}


// .oO0-------------------------------------------------------0Oo. //
// ------------------------ Getting input ------------------------ //
// .oO0-------------------------------------------------------0Oo. //

int *getInput() {
   int *coords = NULL;

   // Determining whether to use default or input
   char choiceChar = NO;
   printf(PROMPT_COORDS);
   scanf(" %c", &choiceChar);
   assert(choiceChar == YES || choiceChar == NO);
   if (choiceChar == YES) {
      // Allocating memory
      coords = malloc(MAX_INPUT_COORDS * sizeof(int) + BUFFER);

      // Instructions
      printf(INSTRUCTION_XY);
      printf(INSTRUCTION_MAX_INPUT);

      // Getting the input
      int coord = 0;
      char exitChar = YES;
      int i = 0;
      while (exitChar == YES && i < MAX_INPUT_COORDS) {
         // Getting and setting x
         printf(PROMPT_X);
         scanf("%d", &coord);
         assert(coord < MAX_INPUT_WIDTH);
         assert(coord > 0);
         coords[i] = coord;

         // Getting and setting y
         printf(PROMPT_Y);
         scanf("%d", &coord);
         assert(coord < MAX_INPUT_HEIGHT);
         assert(coord > 0);
         coords[i + 1] = coord;

         // Determining whether to exit or not
         printf(PROMPT_MORE_COORDS);
         scanf(" %c", &exitChar);
         i += NUMBERS_PER_COORD;
      }

      // Setting terminating value
      coords[i] = TERMINATING_COORD;
   } else {
      // Making no-go coord array
      coords = malloc(sizeof(int));
      coords[0] = TERMINATING_COORD;
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
   // Creating the map used in the intro
   cell *introMap = malloc(width * height * sizeof(cell));
   introMap = createMap(introMap, width, height, introInput);
   // Map has been created - no more use for introInput
   free(introInput);

   // Iterating over the generations of the intro map
   int i = 0;
   while (i < INTRO_ITERATIONS) {
      // Rendering and updating it
      renderMap(introMap, width, height);
      introMap = updateMap(introMap, width, height);

      // Drawing the "Press any key to continue." text
      if (i == 0) {
         move(INTRO_TEXT_HEIGHT + BUFFER, 0);
         attron(COLOR_PAIR(LAST_COLOR));
         addstr(KEY_CONTINUE);
         attroff(COLOR_PAIR(LAST_COLOR));
         getch();
      }

      i++;
      // Delay untill the next generation (so people can see)
      usleep(INTRO_DELAY_PER_GENERATION);
   }

   // Intro has finished - no more use for introMap
   free(introMap);

   // Iterating over the main map
   i = 0;
   while (i < ITERATIONS) {
      // Rendering
      renderMap(map, width, height);
      // Updating based on previous iteration
      map = updateMap(map, width, height);

      i++;
      // Delay untill the next generation (so people can see)
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
   // Iterating over every char in the string
   while (currChar != '\0') {
      currChar = introText[i];
      if (currChar == '\n') {
         // Changing the y of the next coords
         newLineCount++;
      } else if (currChar == LIVE_CELL_CHAR) {
         // Assigning an x and y to each coord
         introArray[arrayPos] = i % width;
         introArray[arrayPos + BUFFER] = newLineCount;

         // Changing position in new list of coords
         arrayPos += NUMBERS_PER_COORD;
      }
      i++;
   }

   // Terminating map
   introArray[i] = TERMINATING_COORD;

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
      if (input[0] == TERMINATING_COORD && DEROGATORY_EQUATION == 0) {
         map[i].lifeStatus = TRUE;
      } else {
         // Getting the x and y values of the cell
         int cellX = i % width;
         int cellY = i / width;

         // Looping over each coord
         int inputX = 0;
         int j = 0;
         while (inputX != TERMINATING_COORD) {
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
      // This is to avoid the error of changing one status in the
      // function above, which will affect the status of other cells,
      // when they should all use the same data to change.
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

   // Assigning a color and background color to each iteration number
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
