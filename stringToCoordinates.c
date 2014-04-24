
//
// stringToCoordinates.c
// Program to convert each '#' in a string to a coordinate
// By Hector Morlet
// 22 April 2014
//


#include <stdio.h>
#include <stdlib.h>


#define WIDTH 54


int main(int argc, char *argv[]) {
   char *introText = "######## ##     ## ########     ######      ###    ##     ## ########\n      ##    ##     ## ##          ##    ##    ## ##   ###   ### ##      \n      ##    ##     ## ##          ##         ##   ##  #### #### ##      \n      ##    ######### ######      ##   #### ##     ## ## ### ## ######  \n      ##    ##     ## ##          ##    ##  ######### ##     ## ##      \n      ##    ##     ## ##          ##    ##  ##     ## ##     ## ##      \n      ##    ##     ## ########     ######   ##     ## ##     ## ########\n    \n    #######  ########    ##       #### ######## ########\n   ##     ## ##          ##        ##  ##       ##      \n   ##     ## ##          ##        ##  ##       ##      \n   ##     ## ######      ##        ##  ######   ######  \n   ##     ## ##          ##        ##  ##       ##      \n   ##     ## ##          ##        ##  ##       ##      \n    #######  ##          ######## #### ##       ########\0"

   int *introArray = malloc(1000); // sort this shit out

   char currChar = ' ';
   int i = 0;
   int newLineCount = 0;
   int arrayPos = 0;
   while (currChar != '\0') {
      currChar = introTest[i];
      if (currChar == '\n') {
         newLineCount++;
      } else if (currChar == '#') {
         introArray[arrayPos] = i % WIDTH;
         introArray[arrayPos + 1] = newLineCount;

         arrayPos += 2;
      }
      i++;
   }

   i = 0;
   while (i < 1000) {
      printf("%d\n", introArray[i]);
   }

   return EXIT_SUCCESS;
}

######## ##     ## ########     ######      ###    ##     ## ########\n
   ##    ##     ## ##          ##    ##    ## ##   ###   ### ##      \n
   ##    ##     ## ##          ##         ##   ##  #### #### ##      \n
   ##    ######### ######      ##   #### ##     ## ## ### ## ######  \n
   ##    ##     ## ##          ##    ##  ######### ##     ## ##      \n
   ##    ##     ## ##          ##    ##  ##     ## ##     ## ##      \n
   ##    ##     ## ########     ######   ##     ## ##     ## ########\n
 #######  ########    ##       #### ######## ########                \n
##     ## ##          ##        ##  ##       ##                      \n
##     ## ##          ##        ##  ##       ##                      \n
##     ## ######      ##        ##  ######   ######                  \n
##     ## ##          ##        ##  ##       ##                      \n
##     ## ##          ##        ##  ##       ##                      \n
 #######  ##          ######## #### ##       ########                \0
                                                                     














