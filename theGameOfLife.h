
//
// theGameOfLife.h
// The Game Of Life by John Conway generated using ncurses
// By Hector Morlet
// 19 April 2014
//


typedef struct _cell {
   int lifeStatNextGeneration;
   int lifeStatus;
   int color;
} cell;

int *getInput();

void iterateGeneration(cell *map, int width, int height);

int *translateIntroText(int *introArray,
                        char *introText,
                        int width);

void renderMap(cell *map, int width, int height);
cell *createMap(cell *map, int width, int height, int *input);

cell *updateMap(cell *map, int width, int height);
int cellNeighbours(cell *map, int cellPosition, int width);
cell *updateCell(cell *map, int cellPosition, int neighbours);

void initCurses();