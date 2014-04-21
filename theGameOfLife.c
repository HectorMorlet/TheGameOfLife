
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


void initCurses();


// .oO0-------------------------------------------------------0Oo. //
// ----------------------------- Main ---------------------------- //
// .oO0-------------------------------------------------------0Oo. //

int main(int argc, char *argv[]) {
   initCurses();

   int x = 0;
   while(x < 60) {
      move(5, x);
      addch('#');
      move(5, x-1);
      addch(' ');
      usleep(50000);
      refresh();
      x++;
   }

   endwin();

   return EXIT_SUCCESS;
}


// .oO0--------------------------------------------------------0Oo. //
// ------------------ Starting and ending curses ------------------ //
// .oO0--------------------------------------------------------0Oo. //

void initCurses() {
   initscr();
   //cbreak();
   noecho();
   //keypad(stdscr, TRUE);
}
