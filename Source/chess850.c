#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <conio.h>

#define OFF 99
#define EMPTY 0
#define MAX_DEPTH 3
#define MAX_MOVES 85

/* 
 * 100-LINE BINARY OPENING BOOK (10 Plies / 5 Full Turns)
 * 100 lines * 20 bytes = 2,000 bytes (2 KB) of ROM
 */
#define BOOK_LINES 100
#define BOOK_DEPTH 10
#define M(f1, r1, f2, r2) (((r1 - '1' + 2) * 10) + (f1 - 'a' + 1)), (((r2 - '1' + 2) * 10) + (f2 - 'a' + 1))

const unsigned char opening_book[BOOK_LINES][BOOK_DEPTH * 2] = {
    /* --- Group 1: Ruy Lopez (1-10) --- */
    { M('e','2','e','4'), M('e','7','e','5'), M('g','1','f','3'), M('b','8','c','6'), M('f','1','b','5'), M('a','7','a','6'), M('b','5','a','4'), M('g','8','f','6'), M('e','1','g','1'), M('f','8','e','7') }, /* Closed */
    { M('e','2','e','4'), M('e','7','e','5'), M('g','1','f','3'), M('b','8','c','6'), M('f','1','b','5'), M('a','7','a','6'), M('b','5','a','4'), M('g','8','f','6'), M('e','1','g','1'), M('b','7','b','5') }, /* Marshall Setup */
    { M('e','2','e','4'), M('e','7','e','5'), M('g','1','f','3'), M('b','8','c','6'), M('f','1','b','5'), M('g','8','f','6'), M('e','1','g','1'), M('f','6','e','4'), M('d','2','d','4'), M('e','4','d','6') }, /* Berlin */
    { M('e','2','e','4'), M('e','7','e','5'), M('g','1','f','3'), M('b','8','c','6'), M('f','1','b','5'), M('a','7','a','6'), M('b','5','c','6'), M('d','7','c','6'), M('e','1','g','1'), M('f','7','f','6') }, /* Exchange */
    { M('e','2','e','4'), M('e','7','e','5'), M('g','1','f','3'), M('b','8','c','6'), M('f','1','b','5'), M('f','7','f','5'), M('d','2','d','3'), M('f','5','e','4'), M('d','3','e','4'), M('g','8','f','6') }, /* Schliemann */
    { M('e','2','e','4'), M('e','7','e','5'), M('g','1','f','3'), M('b','8','c','6'), M('f','1','b','5'), M('d','7','d','6'), M('d','2','d','4'), M('c','8','d','7'), M('b','1','c','3'), M('g','8','f','6') }, /* Steinitz */
    { M('e','2','e','4'), M('e','7','e','5'), M('g','1','f','3'), M('b','8','c','6'), M('f','1','b','5'), M('a','7','a','6'), M('b','5','a','4'), M('g','8','f','6'), M('e','1','g','1'), M('f','6','e','4') }, /* Open */
    { M('e','2','e','4'), M('e','7','e','5'), M('g','1','f','3'), M('b','8','c','6'), M('f','1','b','5'), M('a','7','a','6'), M('b','5','a','4'), M('d','7','d','6'), M('c','2','c','3'), M('f','7','f','5') }, /* Deferred */
    { M('e','2','e','4'), M('e','7','e','5'), M('g','1','f','3'), M('b','8','c','6'), M('f','1','b','5'), M('c','6','d','4'), M('f','3','d','4'), M('e','5','d','4'), M('e','1','g','1'), M('f','8','c','5') }, /* Bird */
    { M('e','2','e','4'), M('e','7','e','5'), M('g','1','f','3'), M('b','8','c','6'), M('f','1','b','5'), M('a','7','a','6'), M('b','5','a','4'), M('b','7','b','5'), M('a','4','b','3'), M('f','8','c','5') }, /* Archangelsk */

    /* --- Group 2: Italian & Open Games (11-25) --- */
    { M('e','2','e','4'), M('e','7','e','5'), M('g','1','f','3'), M('b','8','c','6'), M('f','1','c','4'), M('f','8','c','5'), M('c','2','c','3'), M('g','8','f','6'), M('d','2','d','3'), M('d','7','d','6') }, /* Giuoco Pianissimo */
    { M('e','2','e','4'), M('e','7','e','5'), M('g','1','f','3'), M('b','8','c','6'), M('f','1','c','4'), M('f','8','c','5'), M('b','2','b','4'), M('c','5','b','4'), M('c','2','c','3'), M('b','4','a','5') }, /* Evans Gambit */
    { M('e','2','e','4'), M('e','7','e','5'), M('g','1','f','3'), M('b','8','c','6'), M('f','1','c','4'), M('g','8','f','6'), M('d','2','d','4'), M('e','5','d','4'), M('e','1','g','1'), M('f','6','e','4') }, /* Two Knights */
    { M('e','2','e','4'), M('e','7','e','5'), M('g','1','f','3'), M('b','8','c','6'), M('f','1','c','4'), M('g','8','f','6'), M('f','3','g','5'), M('d','7','d','5'), M('e','4','d','5'), M('f','6','d','5') }, /* Fried Liver */
    { M('e','2','e','4'), M('e','7','e','5'), M('g','1','f','3'), M('b','8','c','6'), M('f','1','c','4'), M('g','8','f','6'), M('f','3','g','5'), M('f','8','c','5'), M('g','5','f','7'), M('c','5','f','2') }, /* Traxler */
    { M('e','2','e','4'), M('e','7','e','5'), M('g','1','f','3'), M('b','8','c','6'), M('d','2','d','4'), M('e','5','d','4'), M('f','3','d','4'), M('g','8','f','6'), M('b','1','c','3'), M('f','8','b','4') }, /* Scotch */
    { M('e','2','e','4'), M('e','7','e','5'), M('g','1','f','3'), M('b','8','c','6'), M('d','2','d','4'), M('e','5','d','4'), M('f','1','c','4'), M('f','8','c','5'), M('c','2','c','3'), M('g','8','f','6') }, /* Scotch Gambit */
    { M('e','2','e','4'), M('e','7','e','5'), M('g','1','f','3'), M('b','8','c','6'), M('b','1','c','3'), M('g','8','f','6'), M('f','1','b','5'), M('f','8','b','4'), M('e','1','g','1'), M('e','8','g','8') }, /* Four Knights */
    { M('e','2','e','4'), M('e','7','e','5'), M('g','1','f','3'), M('g','8','f','6'), M('f','3','e','5'), M('d','7','d','6'), M('e','5','f','3'), M('f','6','e','4'), M('d','2','d','4'), M('d','6','d','5') }, /* Petrov */
    { M('e','2','e','4'), M('e','7','e','5'), M('g','1','f','3'), M('g','8','f','6'), M('d','2','d','4'), M('e','5','d','4'), M('e','4','e','5'), M('f','6','e','4'), M('d','1','d','4'), M('d','7','d','5') }, /* Petrov 3.d4 */
    { M('e','2','e','4'), M('e','7','e','5'), M('g','1','f','3'), M('d','7','d','6'), M('d','2','d','4'), M('e','5','d','4'), M('f','3','d','4'), M('g','8','f','6'), M('b','1','c','3'), M('f','8','e','7') }, /* Philidor */
    { M('e','2','e','4'), M('e','7','e','5'), M('f','2','f','4'), M('e','5','f','4'), M('g','1','f','3'), M('g','7','g','5'), M('h','2','h','4'), M('g','5','g','4'), M('f','3','e','5'), M('g','8','f','6') }, /* King's Gambit */
    { M('e','2','e','4'), M('e','7','e','5'), M('f','2','f','4'), M('f','8','c','5'), M('g','1','f','3'), M('d','7','d','6'), M('c','2','c','3'), M('g','8','f','6'), M('d','2','d','4'), M('e','5','d','4') }, /* KG Declined */
    { M('e','2','e','4'), M('e','7','e','5'), M('b','1','c','3'), M('g','8','f','6'), M('f','1','c','4'), M('f','8','c','5'), M('d','2','d','3'), M('d','7','d','6'), M('c','1','g','5'), M('c','8','e','6') }, /* Vienna Game */
    { M('e','2','e','4'), M('e','7','e','5'), M('d','2','d','4'), M('e','5','d','4'), M('d','1','d','4'), M('b','8','c','6'), M('d','4','e','3'), M('g','8','f','6'), M('b','1','c','3'), M('f','8','e','7') }, /* Center Game */

    /* --- Group 3: Sicilian Defense (26-45) --- */
    { M('e','2','e','4'), M('c','7','c','5'), M('g','1','f','3'), M('d','7','d','6'), M('d','2','d','4'), M('c','5','d','4'), M('f','3','d','4'), M('g','8','f','6'), M('b','1','c','3'), M('a','7','a','6') }, /* Najdorf */
    { M('e','2','e','4'), M('c','7','c','5'), M('g','1','f','3'), M('d','7','d','6'), M('d','2','d','4'), M('c','5','d','4'), M('f','3','d','4'), M('g','8','f','6'), M('b','1','c','3'), M('g','7','g','6') }, /* Dragon */
    { M('e','2','e','4'), M('c','7','c','5'), M('g','1','f','3'), M('d','7','d','6'), M('d','2','d','4'), M('c','5','d','4'), M('f','3','d','4'), M('g','8','f','6'), M('b','1','c','3'), M('e','7','e','6') }, /* Scheveningen */
    { M('e','2','e','4'), M('c','7','c','5'), M('g','1','f','3'), M('b','8','c','6'), M('d','2','d','4'), M('c','5','d','4'), M('f','3','d','4'), M('g','8','f','6'), M('b','1','c','3'), M('e','7','e','5') }, /* Sveshnikov */
    { M('e','2','e','4'), M('c','7','c','5'), M('g','1','f','3'), M('e','7','e','6'), M('d','2','d','4'), M('c','5','d','4'), M('f','3','d','4'), M('b','8','c','6'), M('b','1','c','3'), M('a','7','a','6') }, /* Taimanov */
    { M('e','2','e','4'), M('c','7','c','5'), M('g','1','f','3'), M('e','7','e','6'), M('d','2','d','4'), M('c','5','d','4'), M('f','3','d','4'), M('a','7','a','6'), M('f','1','d','3'), M('g','8','f','6') }, /* Kan */
    { M('e','2','e','4'), M('c','7','c','5'), M('g','1','f','3'), M('b','8','c','6'), M('d','2','d','4'), M('c','5','d','4'), M('f','3','d','4'), M('g','8','f','6'), M('b','1','c','3'), M('d','7','d','6') }, /* Classical */
    { M('e','2','e','4'), M('c','7','c','5'), M('c','2','c','3'), M('g','8','f','6'), M('e','4','e','5'), M('f','6','d','5'), M('d','2','d','4'), M('c','5','d','4'), M('c','3','d','4'), M('d','7','d','6') }, /* Alapin */
    { M('e','2','e','4'), M('c','7','c','5'), M('c','2','c','3'), M('d','7','d','5'), M('e','4','d','5'), M('d','8','d','5'), M('d','2','d','4'), M('c','5','d','4'), M('c','3','d','4'), M('b','8','c','6') }, /* Alapin 2 */
    { M('e','2','e','4'), M('c','7','c','5'), M('b','1','c','3'), M('b','8','c','6'), M('f','2','f','4'), M('g','7','g','6'), M('g','1','f','3'), M('f','8','g','7'), M('f','1','c','4'), M('d','7','d','6') }, /* Grand Prix */
    { M('e','2','e','4'), M('c','7','c','5'), M('b','1','c','3'), M('b','8','c','6'), M('g','2','g','3'), M('g','7','g','6'), M('f','1','g','2'), M('f','8','g','7'), M('d','2','d','3'), M('d','7','d','6') }, /* Closed Sic */
    { M('e','2','e','4'), M('c','7','c','5'), M('g','1','f','3'), M('b','8','c','6'), M('f','1','b','5'), M('g','7','g','6'), M('e','1','g','1'), M('f','8','g','7'), M('c','2','c','3'), M('g','8','f','6') }, /* Rossolimo */
    { M('e','2','e','4'), M('c','7','c','5'), M('g','1','f','3'), M('d','7','d','6'), M('f','1','b','5'), M('c','8','d','7'), M('b','5','d','7'), M('d','8','d','7'), M('c','2','c','4'), M('b','8','c','6') }, /* Moscow */
    { M('e','2','e','4'), M('c','7','c','5'), M('d','2','d','4'), M('c','5','d','4'), M('c','2','c','3'), M('d','4','c','3'), M('b','1','c','3'), M('b','8','c','6'), M('g','1','f','3'), M('e','7','e','6') }, /* Smith-Morra */
    { M('e','2','e','4'), M('c','7','c','5'), M('b','2','b','4'), M('c','5','b','4'), M('a','2','a','3'), M('d','7','d','5'), M('e','4','d','5'), M('d','8','d','5'), M('c','1','b','2'), M('e','7','e','5') }, /* Wing Gambit */
    { M('e','2','e','4'), M('c','7','c','5'), M('f','1','c','4'), M('e','7','e','6'), M('d','1','e','2'), M('b','8','c','6'), M('g','1','f','3'), M('g','8','f','6'), M('c','2','c','3'), M('d','7','d','5') }, /* Bowdler */
    { M('e','2','e','4'), M('c','7','c','5'), M('g','1','f','3'), M('e','7','e','6'), M('d','2','d','3'), M('d','7','d','5'), M('b','1','d','2'), M('b','8','c','6'), M('g','2','g','3'), M('f','8','d','6') }, /* KIA Sic */
    { M('e','2','e','4'), M('c','7','c','5'), M('g','1','f','3'), M('e','7','e','6'), M('b','2','b','3'), M('b','8','c','6'), M('c','1','b','2'), M('d','7','d','6'), M('f','1','b','5'), M('g','8','e','7') }, /* Snyder */
    { M('e','2','e','4'), M('c','7','c','5'), M('c','2','c','4'), M('b','8','c','6'), M('b','1','c','3'), M('g','7','g','6'), M('g','2','g','3'), M('f','8','g','7'), M('f','1','g','2'), M('d','7','d','6') }, /* Botvinnik */
    { M('e','2','e','4'), M('c','7','c','5'), M('g','1','e','2'), M('g','8','f','6'), M('b','1','c','3'), M('d','7','d','5'), M('e','4','d','5'), M('f','6','d','5'), M('d','2','d','4'), M('c','5','d','4') }, /* Keres */

    /* --- Group 4: French Defense (46-55) --- */
    { M('e','2','e','4'), M('e','7','e','6'), M('d','2','d','4'), M('d','7','d','5'), M('e','4','e','5'), M('c','7','c','5'), M('c','2','c','3'), M('b','8','c','6'), M('g','1','f','3'), M('d','8','b','6') }, /* Advance */
    { M('e','2','e','4'), M('e','7','e','6'), M('d','2','d','4'), M('d','7','d','5'), M('b','1','c','3'), M('f','8','b','4'), M('e','4','e','5'), M('c','7','c','5'), M('a','2','a','3'), M('b','4','c','3') }, /* Winawer */
    { M('e','2','e','4'), M('e','7','e','6'), M('d','2','d','4'), M('d','7','d','5'), M('b','1','c','3'), M('g','8','f','6'), M('e','4','e','5'), M('f','6','d','7'), M('f','2','f','4'), M('c','7','c','5') }, /* Classical */
    { M('e','2','e','4'), M('e','7','e','6'), M('d','2','d','4'), M('d','7','d','5'), M('b','1','c','3'), M('g','8','f','6'), M('c','1','g','5'), M('f','8','e','7'), M('e','4','e','5'), M('f','6','d','7') }, /* Burn */
    { M('e','2','e','4'), M('e','7','e','6'), M('d','2','d','4'), M('d','7','d','5'), M('b','1','c','3'), M('g','8','f','6'), M('c','1','g','5'), M('f','8','b','4'), M('e','4','e','5'), M('h','7','h','6') }, /* McCutcheon */
    { M('e','2','e','4'), M('e','7','e','6'), M('d','2','d','4'), M('d','7','d','5'), M('b','1','d','2'), M('c','7','c','5'), M('e','4','d','5'), M('e','6','d','5'), M('g','1','f','3'), M('b','8','c','6') }, /* Tarrasch Open */
    { M('e','2','e','4'), M('e','7','e','6'), M('d','2','d','4'), M('d','7','d','5'), M('b','1','d','2'), M('g','8','f','6'), M('e','4','e','5'), M('f','6','d','7'), M('f','1','d','3'), M('c','7','c','5') }, /* Tarrasch Closed */
    { M('e','2','e','4'), M('e','7','e','6'), M('d','2','d','4'), M('d','7','d','5'), M('e','4','d','5'), M('e','6','d','5'), M('g','1','f','3'), M('g','8','f','6'), M('f','1','d','3'), M('f','8','d','6') }, /* Exchange */
    { M('e','2','e','4'), M('e','7','e','6'), M('d','2','d','4'), M('d','7','d','5'), M('b','1','c','3'), M('d','5','e','4'), M('c','3','e','4'), M('b','8','d','7'), M('g','1','f','3'), M('g','8','f','6') }, /* Rubinstein */
    { M('e','2','e','4'), M('e','7','e','6'), M('d','2','d','3'), M('d','7','d','5'), M('b','1','d','2'), M('g','8','f','6'), M('g','1','f','3'), M('c','7','c','5'), M('g','2','g','3'), M('b','8','c','6') }, /* KIA */

    /* --- Group 5: Caro-Kann (56-65) --- */
    { M('e','2','e','4'), M('c','7','c','6'), M('d','2','d','4'), M('d','7','d','5'), M('b','1','c','3'), M('d','5','e','4'), M('c','3','e','4'), M('c','8','f','5'), M('e','4','g','3'), M('f','5','g','6') }, /* Classical */
    { M('e','2','e','4'), M('c','7','c','6'), M('d','2','d','4'), M('d','7','d','5'), M('b','1','c','3'), M('d','5','e','4'), M('c','3','e','4'), M('b','8','d','7'), M('f','1','c','4'), M('g','8','f','6') }, /* Karpov */
    { M('e','2','e','4'), M('c','7','c','6'), M('d','2','d','4'), M('d','7','d','5'), M('e','4','e','5'), M('c','8','f','5'), M('g','1','f','3'), M('e','7','e','6'), M('f','1','e','2'), M('c','6','c','5') }, /* Advance */
    { M('e','2','e','4'), M('c','7','c','6'), M('d','2','d','4'), M('d','7','d','5'), M('e','4','e','5'), M('c','8','f','5'), M('b','1','c','3'), M('e','7','e','6'), M('g','2','g','4'), M('f','5','g','6') }, /* Advance Tal */
    { M('e','2','e','4'), M('c','7','c','6'), M('d','2','d','4'), M('d','7','d','5'), M('e','4','d','5'), M('c','6','d','5'), M('c','2','c','4'), M('g','8','f','6'), M('b','1','c','3'), M('e','7','e','6') }, /* Panov */
    { M('e','2','e','4'), M('c','7','c','6'), M('d','2','d','4'), M('d','7','d','5'), M('e','4','d','5'), M('c','6','d','5'), M('f','1','d','3'), M('b','8','c','6'), M('c','2','c','3'), M('g','8','f','6') }, /* Exchange */
    { M('e','2','e','4'), M('c','7','c','6'), M('d','2','d','4'), M('d','7','d','5'), M('f','2','f','3'), M('d','5','e','4'), M('f','3','e','4'), M('e','7','e','5'), M('g','1','f','3'), M('e','5','d','4') }, /* Fantasy */
    { M('e','2','e','4'), M('c','7','c','6'), M('g','1','f','3'), M('d','7','d','5'), M('b','1','c','3'), M('c','8','g','4'), M('h','2','h','3'), M('g','4','f','3'), M('d','1','f','3'), M('g','8','f','6') }, /* Two Knights */
    { M('e','2','e','4'), M('c','7','c','6'), M('d','2','d','3'), M('d','7','d','5'), M('b','1','d','2'), M('g','7','g','6'), M('g','1','f','3'), M('f','8','g','7'), M('g','2','g','3'), M('e','7','e','5') }, /* KIA */
    { M('e','2','e','4'), M('c','7','c','6'), M('d','2','d','4'), M('d','7','d','5'), M('b','1','c','3'), M('g','7','g','6'), M('h','2','h','4'), M('h','7','h','5'), M('g','1','f','3'), M('c','8','g','4') }, /* Gurgenidze */

    /* --- Group 6: Pirc / Modern / Alekhine / Scandi (66-75) --- */
    { M('e','2','e','4'), M('d','7','d','6'), M('d','2','d','4'), M('g','8','f','6'), M('b','1','c','3'), M('g','7','g','6'), M('g','1','f','3'), M('f','8','g','7'), M('f','1','e','2'), M('e','8','g','8') }, /* Pirc Class */
    { M('e','2','e','4'), M('d','7','d','6'), M('d','2','d','4'), M('g','8','f','6'), M('b','1','c','3'), M('g','7','g','6'), M('f','2','f','4'), M('f','8','g','7'), M('g','1','f','3'), M('e','8','g','8') }, /* Pirc Austrian */
    { M('e','2','e','4'), M('g','7','g','6'), M('d','2','d','4'), M('f','8','g','7'), M('b','1','c','3'), M('d','7','d','6'), M('f','1','e','3'), M('a','7','a','6'), M('d','1','d','2'), M('b','7','b','5') }, /* Modern */
    { M('e','2','e','4'), M('g','8','f','6'), M('e','4','e','5'), M('f','6','d','5'), M('d','2','d','4'), M('d','7','d','6'), M('c','2','c','4'), M('d','5','b','6'), M('f','2','f','4'), M('d','6','e','5') }, /* Alekhine 4P */
    { M('e','2','e','4'), M('g','8','f','6'), M('e','4','e','5'), M('f','6','d','5'), M('d','2','d','4'), M('d','7','d','6'), M('g','1','f','3'), M('c','8','g','4'), M('f','1','e','2'), M('e','7','e','6') }, /* Alekhine Mod */
    { M('e','2','e','4'), M('d','7','d','5'), M('e','4','d','5'), M('d','8','d','5'), M('b','1','c','3'), M('d','5','a','5'), M('d','2','d','4'), M('g','8','f','6'), M('g','1','f','3'), M('c','7','c','6') }, /* Scandi Qa5 */
    { M('e','2','e','4'), M('d','7','d','5'), M('e','4','d','5'), M('g','8','f','6'), M('d','2','d','4'), M('f','6','d','5'), M('g','1','f','3'), M('c','8','g','4'), M('f','1','e','2'), M('e','7','e','6') }, /* Scandi Nf6 */
    { M('e','2','e','4'), M('d','7','d','5'), M('e','4','d','5'), M('d','8','d','5'), M('b','1','c','3'), M('d','5','d','6'), M('d','2','d','4'), M('g','8','f','6'), M('g','1','f','3'), M('c','7','c','6') }, /* Scandi Qd6 */
    { M('e','2','e','4'), M('b','8','c','6'), M('d','2','d','4'), M('d','7','d','5'), M('e','4','e','5'), M('c','8','f','5'), M('g','1','f','3'), M('e','7','e','6'), M('f','1','e','2'), M('f','7','f','6') }, /* Nimzowitsch */
    { M('d','2','d','4'), M('b','7','b','6'), M('e','2','e','4'), M('c','8','b','7'), M('f','1','d','3'), M('e','7','e','6'), M('g','1','f','3'), M('c','7','c','5'), M('c','2','c','3'), M('g','8','f','6') }, /* Owen's Def */

    /* --- Group 7: QGD / QGA / Slav (76-85) --- */
    { M('d','2','d','4'), M('d','7','d','5'), M('c','2','c','4'), M('e','7','e','6'), M('b','1','c','3'), M('g','8','f','6'), M('c','1','g','5'), M('f','8','e','7'), M('e','2','e','3'), M('e','8','g','8') }, /* QGD Orth */
    { M('d','2','d','4'), M('d','7','d','5'), M('c','2','c','4'), M('e','7','e','6'), M('b','1','c','3'), M('g','8','f','6'), M('c','1','g','5'), M('f','8','e','7'), M('e','2','e','3'), M('h','7','h','6') }, /* QGD Tartakower */
    { M('d','2','d','4'), M('d','7','d','5'), M('c','2','c','4'), M('e','7','e','6'), M('b','1','c','3'), M('g','8','f','6'), M('c','4','d','5'), M('e','6','d','5'), M('c','1','g','5'), M('c','7','c','6') }, /* QGD Exchange */
    { M('d','2','d','4'), M('d','7','d','5'), M('c','2','c','4'), M('c','7','c','6'), M('g','1','f','3'), M('g','8','f','6'), M('b','1','c','3'), M('d','5','c','4'), M('a','2','a','4'), M('c','8','f','5') }, /* Slav */
    { M('d','2','d','4'), M('d','7','d','5'), M('c','2','c','4'), M('c','7','c','6'), M('g','1','f','3'), M('g','8','f','6'), M('b','1','c','3'), M('a','7','a','6'), M('e','2','e','3'), M('b','7','b','5') }, /* Slav Chebanenko */
    { M('d','2','d','4'), M('d','7','d','5'), M('c','2','c','4'), M('c','7','c','6'), M('g','1','f','3'), M('g','8','f','6'), M('b','1','c','3'), M('e','7','e','6'), M('e','2','e','3'), M('b','8','d','7') }, /* Semi-Slav */
    { M('d','2','d','4'), M('d','7','d','5'), M('c','2','c','4'), M('d','5','c','4'), M('g','1','f','3'), M('g','8','f','6'), M('e','2','e','3'), M('e','7','e','6'), M('f','1','c','4'), M('c','7','c','5') }, /* QGA */
    { M('d','2','d','4'), M('d','7','d','5'), M('c','2','c','4'), M('d','5','c','4'), M('e','2','e','4'), M('b','7','b','5'), M('a','2','a','4'), M('c','7','c','6'), M('a','4','b','5'), M('c','6','b','5') }, /* QGA Alekhine */
    { M('d','2','d','4'), M('d','7','d','5'), M('c','2','c','4'), M('e','7','e','5'), M('d','4','e','5'), M('d','5','d','4'), M('g','1','f','3'), M('b','8','c','6'), M('g','2','g','3'), M('c','8','g','4') }, /* Albin CG */
    { M('d','2','d','4'), M('d','7','d','5'), M('c','2','c','4'), M('b','8','c','6'), M('g','1','f','3'), M('c','8','g','4'), M('c','4','d','5'), M('g','4','f','3'), M('g','2','f','3'), M('d','8','d','5') }, /* Chigorin */

    /* --- Group 8: Indian Defenses (86-95) --- */
    { M('d','2','d','4'), M('g','8','f','6'), M('c','2','c','4'), M('g','7','g','6'), M('b','1','c','3'), M('f','8','g','7'), M('e','2','e','4'), M('d','7','d','6'), M('g','1','f','3'), M('e','8','g','8') }, /* KID Mar del Plata */
    { M('d','2','d','4'), M('g','8','f','6'), M('c','2','c','4'), M('g','7','g','6'), M('b','1','c','3'), M('f','8','g','7'), M('e','2','e','4'), M('d','7','d','6'), M('f','2','f','3'), M('e','8','g','8') }, /* KID Saemisch */
    { M('d','2','d','4'), M('g','8','f','6'), M('c','2','c','4'), M('e','7','e','6'), M('b','1','c','3'), M('f','8','b','4'), M('e','2','e','3'), M('e','8','g','8'), M('f','1','d','3'), M('d','7','d','5') }, /* Nimzo-Indian */
    { M('d','2','d','4'), M('g','8','f','6'), M('c','2','c','4'), M('e','7','e','6'), M('b','1','c','3'), M('f','8','b','4'), M('d','1','c','2'), M('c','7','c','5'), M('d','4','c','5'), M('e','8','g','8') }, /* Nimzo Classical */
    { M('d','2','d','4'), M('g','8','f','6'), M('c','2','c','4'), M('e','7','e','6'), M('g','1','f','3'), M('b','7','b','6'), M('g','2','g','3'), M('c','8','a','6'), M('b','2','b','3'), M('f','8','b','4') }, /* Queen's Indian */
    { M('d','2','d','4'), M('g','8','f','6'), M('c','2','c','4'), M('g','7','g','6'), M('b','1','c','3'), M('d','7','d','5'), M('c','4','d','5'), M('f','6','d','5'), M('e','2','e','4'), M('d','5','c','3') }, /* Gruenfeld */
    { M('d','2','d','4'), M('g','8','f','6'), M('c','2','c','4'), M('c','7','c','5'), M('d','4','d','5'), M('e','7','e','6'), M('b','1','c','3'), M('e','6','d','5'), M('c','4','d','5'), M('d','7','d','6') }, /* Benoni */
    { M('d','2','d','4'), M('g','8','f','6'), M('c','2','c','4'), M('c','7','c','5'), M('d','4','d','5'), M('b','7','b','5'), M('c','4','b','5'), M('a','7','a','6'), M('b','5','a','6'), M('c','8','a','6') }, /* Benko Gambit */
    { M('d','2','d','4'), M('g','8','f','6'), M('c','2','c','4'), M('e','7','e','6'), M('g','1','f','3'), M('f','8','b','4'), M('c','1','d','2'), M('a','7','a','5'), M('g','2','g','3'), M('e','8','g','8') }, /* Bogo-Indian */
    { M('d','2','d','4'), M('g','8','f','6'), M('c','1','g','5'), M('f','6','e','4'), M('g','5','f','4'), M('c','7','c','5'), M('f','2','f','3'), M('d','8','a','5'), M('c','2','c','3'), M('e','4','f','6') }, /* Trompowsky */

    /* --- Group 9: Flank Openings (96-100) --- */
    { M('c','2','c','4'), M('c','7','c','5'), M('b','1','c','3'), M('b','8','c','6'), M('g','1','f','3'), M('g','8','f','6'), M('d','2','d','4'), M('c','5','d','4'), M('f','3','d','4'), M('e','7','e','6') }, /* English Sym */
    { M('c','2','c','4'), M('e','7','e','5'), M('b','1','c','3'), M('g','8','f','6'), M('g','1','f','3'), M('b','8','c','6'), M('g','2','g','3'), M('f','8','b','4'), M('f','1','g','2'), M('e','8','g','8') }, /* English Rev Sic */
    { M('g','1','f','3'), M('d','7','d','5'), M('c','2','c','4'), M('d','5','c','4'), M('e','2','e','3'), M('g','8','f','6'), M('f','1','c','4'), M('e','7','e','6'), M('e','1','g','1'), M('c','7','c','5') }, /* Reti Accepted */
    { M('d','2','d','4'), M('f','7','f','5'), M('c','2','c','4'), M('g','8','f','6'), M('g','2','g','3'), M('g','7','g','6'), M('f','1','g','2'), M('f','8','g','7'), M('g','1','f','3'), M('e','8','g','8') }, /* Dutch Leningrad */
    { M('d','2','d','4'), M('f','7','f','5'), M('c','2','c','4'), M('g','8','f','6'), M('g','2','g','3'), M('e','7','e','6'), M('f','1','g','2'), M('d','7','d','5'), M('g','1','f','3'), M('c','7','c','6') }  /* Dutch Stonewall */
};

const int piece_val[7] = {0, 100, 300, 325, 500, 900, 20000};
const int pst_pawn[64] = {
    0,  0,  0,  0,  0,  0,  0,  0,
    50, 50, 50, 50, 50, 50, 50, 50,
    10, 10, 20, 30, 30, 20, 10, 10,
     5,  5, 10, 25, 25, 10,  5,  5,
     0,  0,  0, 20, 20,  0,  0,  0,
     5, -5,-10,  0,  0,-10, -5,  5,
     5, 10, 10,-20,-20, 10, 10,  5,
     0,  0,  0,  0,  0,  0,  0,  0
};
const int pst_knight[64] = {
    -50,-40,-30,-30,-30,-30,-40,-50,
    -40,-20,  0,  0,  0,  0,-20,-40,
    -30,  0, 10, 15, 15, 10,  0,-30,
    -30,  5, 15, 20, 20, 15,  5,-30,
    -30,  0, 15, 20, 20, 15,  0,-30,
    -30,  5, 10, 15, 15, 10,  5,-30,
    -40,-20,  0,  5,  5,  0,-20,-40,
    -50,-40,-30,-30,-30,-30,-40,-50
};

const int START_BOARD[120] = {
    OFF, OFF, OFF, OFF, OFF, OFF, OFF, OFF, OFF, OFF,
    OFF, OFF, OFF, OFF, OFF, OFF, OFF, OFF, OFF, OFF,
    OFF,  4,  2,  3,  5,  6,  3,  2,  4, OFF, 
    OFF,  1,  1,  1,  1,  1,  1,  1,  1, OFF, 
    OFF,  0,  0,  0,  0,  0,  0,  0,  0, OFF, 
    OFF,  0,  0,  0,  0,  0,  0,  0,  0, OFF, 
    OFF,  0,  0,  0,  0,  0,  0,  0,  0, OFF, 
    OFF,  0,  0,  0,  0,  0,  0,  0,  0, OFF, 
    OFF, -1, -1, -1, -1, -1, -1, -1, -1, OFF, 
    OFF, -4, -2, -3, -5, -6, -3, -2, -4, OFF, 
    OFF, OFF, OFF, OFF, OFF, OFF, OFF, OFF, OFF, OFF,
    OFF, OFF, OFF, OFF, OFF, OFF, OFF, OFF, OFF, OFF
};

struct GameState_t {
    int board[120];
    int nodes_searched;
    int current_depth;
    int current_eval;
    int scroll_offset;
    int history_count;
    char move_history[50][8];
    unsigned char history_from[50];
    unsigned char history_to[50];
    int engine_book_line;
    int w_king_moved;
    int w_rook_a_moved;
    int w_rook_h_moved;
    int b_king_moved;
    int b_rook_a_moved;
    int b_rook_h_moved;
    int en_passant_sq;
    char top_moves[5][8];
    int top_scores[5];
    int top_count;
    char engine_last_move[8];
} g = {
    {
        OFF, OFF, OFF, OFF, OFF, OFF, OFF, OFF, OFF, OFF,
        OFF, OFF, OFF, OFF, OFF, OFF, OFF, OFF, OFF, OFF,
        OFF,  4,  2,  3,  5,  6,  3,  2,  4, OFF, 
        OFF,  1,  1,  1,  1,  1,  1,  1,  1, OFF, 
        OFF,  0,  0,  0,  0,  0,  0,  0,  0, OFF, 
        OFF,  0,  0,  0,  0,  0,  0,  0,  0, OFF, 
        OFF,  0,  0,  0,  0,  0,  0,  0,  0, OFF, 
        OFF,  0,  0,  0,  0,  0,  0,  0,  0, OFF, 
        OFF, -1, -1, -1, -1, -1, -1, -1, -1, OFF, 
        OFF, -4, -2, -3, -5, -6, -3, -2, -4, OFF, 
        OFF, OFF, OFF, OFF, OFF, OFF, OFF, OFF, OFF, OFF,
        OFF, OFF, OFF, OFF, OFF, OFF, OFF, OFF, OFF, OFF
    },
    0, 0, 0, 0, 0, { {""} }, { 0 }, { 0 }, -1, 0, 0, 0, 0, 0, 0, 0, { {""} }, { 0 }, 0, ""
};

#define board g.board
#define nodes_searched g.nodes_searched
#define current_depth g.current_depth
#define current_eval g.current_eval
#define scroll_offset g.scroll_offset
#define history_count g.history_count
#define move_history g.move_history
#define history_from g.history_from
#define history_to g.history_to
#define engine_book_line g.engine_book_line
#define w_king_moved g.w_king_moved
#define w_rook_a_moved g.w_rook_a_moved
#define w_rook_h_moved g.w_rook_h_moved
#define b_king_moved g.b_king_moved
#define b_rook_a_moved g.b_rook_a_moved
#define b_rook_h_moved g.b_rook_h_moved
#define en_passant_sq g.en_passant_sq
#define top_moves g.top_moves
#define top_scores g.top_scores
#define top_count g.top_count
#define engine_last_move g.engine_last_move

unsigned char move_from[(MAX_DEPTH + 1) * MAX_MOVES];
unsigned char move_to[(MAX_DEPTH + 1) * MAX_MOVES];
int move_counts[MAX_DEPTH + 1];

const int dir_vectors[8] = {10, -10, 1, -1, 11, 9, -11, -9};
const int knight_vectors[8] = {-21, -19, -12, -8, 8, 12, 19, 21};

void init_display(void) {
    printf("\x1B[2&"); 
    putchar(12);       
}

void goto_xy(int row, int col) {
    printf("\x1B[%d;%dH", row, col);
}

void clear_row(int row) {
    goto_xy(row, 1);
    printf("                                "); 
}

void str_tolower(char *str) {
    int i;
    for (i = 0; str[i]; i++) str[i] = tolower(str[i]);
}

int find_king(int color) {
    int sq;
    for (sq = 21; sq <= 98; sq++) if (board[sq] == 6 * color) return sq;
    return 0;
}

int is_square_attacked(int sq, int attacker_color) {
    int i, t, p, ptype, p_back;
    for (i = 0; i < 8; i++) {
        t = sq + knight_vectors[i];
        if (board[t] != OFF && board[t] == 2 * attacker_color) return 1;
    }
    p_back = (attacker_color == 1) ? -10 : 10;
    if (board[sq + p_back - 1] == 1 * attacker_color) return 1;
    if (board[sq + p_back + 1] == 1 * attacker_color) return 1;
    for (i = 0; i < 4; i++) {
        t = sq;
        while (1) {
            t += dir_vectors[i];
            p = board[t];
            if (p == OFF) break;
            if (p != EMPTY) {
                ptype = abs(p);
                if (p * attacker_color > 0 && (ptype == 4 || ptype == 5)) return 1;
                break;
            }
        }
    }
    for (i = 4; i < 8; i++) {
        t = sq;
        while (1) {
            t += dir_vectors[i];
            p = board[t];
            if (p == OFF) break;
            if (p != EMPTY) {
                ptype = abs(p);
                if (p * attacker_color > 0 && (ptype == 3 || ptype == 5)) return 1;
                break;
            }
        }
    }
    for (i = 0; i < 8; i++) {
        t = sq + dir_vectors[i];
        if (board[t] != OFF && board[t] == 6 * attacker_color) return 1;
    }
    return 0;
}

void format_san(int from, int to, int piece, int captured, int is_check, int is_mate, char *out_san) {
    int ptype = abs(piece);
    int from_col = (from % 10) - 1;
    int to_col   = (to % 10) - 1;
    int to_row   = (to / 10) - 1;
    int idx = 0;

    if (ptype == 6 && abs(to - from) == 2) {
        if (to > from) strcpy(out_san, "O-O");
        else strcpy(out_san, "O-O-O");
        if (is_mate) strcat(out_san, "#");
        else if (is_check) strcat(out_san, "+");
        return;
    }
    if (ptype == 2) out_san[idx++] = 'N';
    else if (ptype == 3) out_san[idx++] = 'B';
    else if (ptype == 4) out_san[idx++] = 'R';
    else if (ptype == 5) out_san[idx++] = 'Q';
    else if (ptype == 6) out_san[idx++] = 'K';

    if (captured != 0 && captured != OFF) {
        if (ptype == 1) out_san[idx++] = 'a' + from_col;
        out_san[idx++] = 'x';
    }
    out_san[idx++] = 'a' + to_col;
    out_san[idx++] = '0' + to_row;
    if (is_mate) out_san[idx++] = '#';
    else if (is_check) out_san[idx++] = '+';
    out_san[idx] = '\0';
}

void sq_to_str(int sq, char *buf) {
    int file = (sq % 10) - 1;
    int rank = (sq / 10) - 2;
    buf[0] = 'a' + file;
    buf[1] = '1' + rank;
    buf[2] = '\0';
}

int get_alignment(int sq, int color) {
    if (board[sq] == OFF) return OFF;
    if (board[sq] == EMPTY) return 0;
    return (board[sq] * color > 0) ? 1 : -1;
}

void generate_moves(int color, int ply) {
    int move_idx = ply * MAX_MOVES;
    int sq, piece, dir, fwd, rank, double_fwd;
    int c, target, align, v, start_dir, end_dir, is_slider;
    int cap_dirs[2];
    
    move_counts[ply] = 0;
    for (sq = 21; sq <= 98; sq++) {
        if (get_alignment(sq, color) != 1) continue;
        piece = abs(board[sq]);

        if (piece == 1) {
            dir = (color == 1) ? 10 : -10;
            fwd = sq + dir;
            if (board[fwd] == EMPTY) {
                move_from[move_idx] = sq; move_to[move_idx] = fwd;
                move_idx++; move_counts[ply]++;
                rank = (sq / 10) - 2;
                double_fwd = sq + (dir * 2);
                if (((color == 1 && rank == 1) || (color == -1 && rank == 6)) && board[double_fwd] == EMPTY) {
                    move_from[move_idx] = sq; move_to[move_idx] = double_fwd;
                    move_idx++; move_counts[ply]++;
                }
            }
            cap_dirs[0] = dir - 1;
            cap_dirs[1] = dir + 1;
            for (c = 0; c < 2; c++) {
                target = sq + cap_dirs[c];
                if (get_alignment(target, color) == -1 || target == en_passant_sq) {
                    move_from[move_idx] = sq; move_to[move_idx] = target;
                    move_idx++; move_counts[ply]++;
                }
            }
            continue;
        }

        if (piece == 2) {
            for (v = 0; v < 8; v++) {
                target = sq + knight_vectors[v];
                align = get_alignment(target, color);
                if (align == 0 || align == -1) {
                    move_from[move_idx] = sq; move_to[move_idx] = target;
                    move_idx++; move_counts[ply]++;
                }
            }
            continue;
        }

        start_dir = (piece == 3) ? 4 : 0;
        end_dir   = (piece == 4) ? 4 : 8;
        is_slider = (piece != 6);

        for (v = start_dir; v < end_dir; v++) {
            target = sq;
            while (1) {
                target += dir_vectors[v];
                align = get_alignment(target, color);
                if (align == OFF || align == 1) break;
                move_from[move_idx] = sq; move_to[move_idx] = target;
                move_idx++; move_counts[ply]++;
                if (align == -1 || !is_slider) break;
            }
        }

        if (piece == 6) {
            if (color == 1 && sq == 25) {
                if (!w_king_moved && !w_rook_h_moved && board[28] == 4 && board[26] == EMPTY && board[27] == EMPTY && 
                    !is_square_attacked(25, -1) && !is_square_attacked(26, -1) && !is_square_attacked(27, -1)) {
                    move_from[move_idx] = 25; move_to[move_idx] = 27; move_idx++; move_counts[ply]++;
                }
                if (!w_king_moved && !w_rook_a_moved && board[21] == 4 && board[24] == EMPTY && board[23] == EMPTY && board[22] == EMPTY && 
                    !is_square_attacked(25, -1) && !is_square_attacked(24, -1) && !is_square_attacked(23, -1)) {
                    move_from[move_idx] = 25; move_to[move_idx] = 23; move_idx++; move_counts[ply]++;
                }
            } else if (color == -1 && sq == 95) {
                if (!b_king_moved && !b_rook_h_moved && board[98] == -4 && board[96] == EMPTY && board[97] == EMPTY && 
                    !is_square_attacked(95, 1) && !is_square_attacked(96, 1) && !is_square_attacked(97, 1)) {
                    move_from[move_idx] = 95; move_to[move_idx] = 97; move_idx++; move_counts[ply]++;
                }
                if (!b_king_moved && !b_rook_a_moved && board[91] == -4 && board[94] == EMPTY && board[93] == EMPTY && board[92] == EMPTY && 
                    !is_square_attacked(95, 1) && !is_square_attacked(94, 1) && !is_square_attacked(93, 1)) {
                    move_from[move_idx] = 95; move_to[move_idx] = 93; move_idx++; move_counts[ply]++;
                }
            }
        }
    }
}

void score_and_sort_moves(int ply) {
    int i, j, count, start_idx, cap, piece, temp_score;
    unsigned char temp_from, temp_to;
    int scores[MAX_MOVES];

    count = move_counts[ply];
    start_idx = ply * MAX_MOVES;

    for (i = 0; i < count; i++) {
        scores[i] = 0;
        cap = abs(board[move_to[start_idx + i]]);
        if (cap != EMPTY) {
            piece = abs(board[move_from[start_idx + i]]);
            scores[i] = piece_val[cap] + (1000 - piece_val[piece]);
        }
        if (abs(board[move_from[start_idx + i]]) == 1) {
            if (move_to[start_idx + i] <= 29 || move_to[start_idx + i] >= 91) scores[i] += 9000; 
        }
    }

    for (i = 1; i < count; i++) {
        temp_score = scores[i];
        temp_from = move_from[start_idx + i];
        temp_to = move_to[start_idx + i];
        j = i - 1;
        while (j >= 0 && scores[j] < temp_score) {
            scores[j + 1] = scores[j];
            move_from[start_idx + j + 1] = move_from[start_idx + j];
            move_to[start_idx + j + 1] = move_to[start_idx + j];
            j--;
        }
        scores[j + 1] = temp_score;
        move_from[start_idx + j + 1] = temp_from;
        move_to[start_idx + j + 1] = temp_to;
    }
}

int make_move_full(unsigned char from, unsigned char to, int *captured, int *ep_target_out, int *promoted) {
    int piece = board[from];
    int ptype = abs(piece);
    *captured = board[to];
    *ep_target_out = 0;
    *promoted = 0;

    if (ptype == 1 && to == en_passant_sq) {
        int cap_sq = (piece > 0) ? (to - 10) : (to + 10);
        *captured = board[cap_sq];
        board[cap_sq] = EMPTY;
    }
    if (ptype == 1 && abs(to - from) == 20) {
        *ep_target_out = (from + to) / 2;
    }
    if (ptype == 6 && abs(to - from) == 2) {
        if (to > from) {
            board[from + 1] = board[from + 3]; board[from + 3] = EMPTY;
        } else {
            board[from - 1] = board[from - 4]; board[from - 4] = EMPTY;
        }
    }
    if (ptype == 1 && (to <= 29 || to >= 91)) {
        piece = (piece > 0) ? 5 : -5;
        *promoted = 1;
    }
    board[to] = piece;
    board[from] = EMPTY;
    return *captured;
}

void unmake_move_full(unsigned char from, unsigned char to, int captured, int prev_ep, int promoted) {
    int piece = board[to];
    int ptype = abs(piece);
    if (promoted) {
        piece = (piece > 0) ? 1 : -1;
        ptype = 1;
    }
    board[from] = piece;
    board[to] = EMPTY;

    if (ptype == 6 && abs(to - from) == 2) {
        if (to > from) {
            board[from + 3] = board[from + 1]; board[from + 1] = EMPTY;
        } else {
            board[from - 4] = board[from - 1]; board[from - 1] = EMPTY;
        }
    }
    if (ptype == 1 && to == prev_ep) {
        int cap_sq = (piece > 0) ? (to - 10) : (to + 10);
        board[cap_sq] = -piece; 
        board[to] = EMPTY;
    } else {
        board[to] = captured;
    }
    en_passant_sq = prev_ep;
}

void replay_to(int target_count) {
    int i, j, k, color, move_idx, cap, prom, from, to, piece;
    char clean_input[15], san_buf[15];
    
    for (i = 0; i < 120; i++) board[i] = START_BOARD[i];
    nodes_searched = 0;
    current_depth = 0;
    current_eval = 0;
    w_king_moved = 0; w_rook_a_moved = 0; w_rook_h_moved = 0;
    b_king_moved = 0; b_rook_a_moved = 0; b_rook_h_moved = 0;
    en_passant_sq = 0;
    engine_book_line = -1;
    
    for (k = 0; k < target_count; k++) {
        color = (k % 2 == 0) ? 1 : -1;
        j = 0;
        for (i = 0; move_history[k][i] != '\0'; i++) {
            if (isalnum(move_history[k][i]) || move_history[k][i] == '-') {
                clean_input[j++] = tolower(move_history[k][i]);
            }
        }
        clean_input[j] = '\0';
        
        generate_moves(color, 0);
        move_idx = -1;
        
        for (i = 0; i < move_counts[0]; i++) {
            format_san(move_from[i], move_to[i], board[move_from[i]], board[move_to[i]], 0, 0, san_buf);
            str_tolower(san_buf);
            if (strcmp(clean_input, san_buf) == 0) {
                move_idx = i;
                break;
            }
        }
        
        if (move_idx != -1) {
            from = move_from[move_idx];
            to = move_to[move_idx];
            piece = board[from];
            
            history_from[k] = from;
            history_to[k] = to;
            
            make_move_full(from, to, &cap, &en_passant_sq, &prom);
            
            if (piece == 6) w_king_moved = 1;
            if (from == 21) w_rook_a_moved = 1;
            if (from == 28) w_rook_h_moved = 1;
            if (piece == -6) b_king_moved = 1;
            if (from == 91) b_rook_a_moved = 1;
            if (from == 98) b_rook_h_moved = 1;
        }
    }
    
    history_count = target_count;
    if (history_count > 7) scroll_offset = history_count - 7;
    else scroll_offset = 0;
    
    top_count = 0;
    for (i = 0; i < 5; i++) {
        strcpy(top_moves[i], "-");
        top_scores[i] = 0;
    }
    strcpy(engine_last_move, "-");
}

void reset_game(void) {
    int i;
    for (i = 0; i < 50; i++) {
        strcpy(move_history[i], "");
        history_from[i] = 0;
        history_to[i] = 0;
    }
    replay_to(0);
}

int evaluate_board(void) {
    int score = 0;
    int sq, p, piece_type, val, file, rank, pst_idx;
    for (sq = 21; sq <= 98; sq++) {
        p = board[sq];
        if (p == OFF || p == EMPTY) continue;
        piece_type = abs(p);
        val = piece_val[piece_type];
        file = (sq % 10) - 1;
        rank = (sq / 10) - 2;
        pst_idx = (p > 0) ? ((7 - rank) * 8 + file) : (rank * 8 + file);
        if (piece_type == 1) val += pst_pawn[pst_idx];
        else if (piece_type == 2) val += pst_knight[pst_idx];
        if (p > 0) score += val;
        else score -= val;
    }
    return score;
}

int has_legal_moves(int color) {
    int i, cap, prom, prev_ep = en_passant_sq;
    int ply = MAX_DEPTH; 
    int start_idx = ply * MAX_MOVES;
    int illegal;
    generate_moves(color, ply);
    for (i = 0; i < move_counts[ply]; i++) {
        make_move_full(move_from[start_idx + i], move_to[start_idx + i], &cap, &en_passant_sq, &prom);
        illegal = is_square_attacked(find_king(color), -color);
        unmake_move_full(move_from[start_idx + i], move_to[start_idx + i], cap, prev_ep, prom);
        if (!illegal) return 1; 
    }
    return 0; 
}

int alpha_beta(int depth, int alpha, int beta, int color, int ply) {
    int count, start_idx, i, captured, score, promoted;
    unsigned char from, to;
    int prev_ep = en_passant_sq;
    int legal_moves = 0;

    nodes_searched++;
    if (depth == 0) return evaluate_board() * color;

    generate_moves(color, ply);
    score_and_sort_moves(ply); 
    
    count = move_counts[ply];
    start_idx = ply * MAX_MOVES;

    for (i = 0; i < count; i++) {
        from = move_from[start_idx + i];
        to = move_to[start_idx + i];
        make_move_full(from, to, &captured, &en_passant_sq, &promoted);
        
        if (is_square_attacked(find_king(color), -color)) {
            unmake_move_full(from, to, captured, prev_ep, promoted);
            continue; 
        }
        legal_moves++;
        score = -alpha_beta(depth - 1, -beta, -alpha, -color, ply + 1);
        unmake_move_full(from, to, captured, prev_ep, promoted);

        if (score >= beta) return beta;
        if (score > alpha) alpha = score;
    }
    if (legal_moves == 0) {
        if (is_square_attacked(find_king(color), -color)) return -20000 + ply;
        return 0; 
    }
    return alpha;
}

int find_best_move(int color, unsigned char *best_from, unsigned char *best_to) {
    int count, best_score, i, j, k, captured, score, promoted;
    unsigned char from, to;
    int moving_piece, captured_piece;
    int prev_ep = en_passant_sq;
    int is_chk;
    int legal_moves = 0;
    int match_found, line;

    nodes_searched = 0;
    current_depth = MAX_DEPTH;
    top_count = 0;

    /* --- BINARY OPENING BOOK LOOKUP --- */
    if (history_count < BOOK_DEPTH) {
        for (line = 0; line < BOOK_LINES; line++) {
            match_found = 1;
            for (j = 0; j < history_count; j++) {
                if (history_from[j] != opening_book[line][j * 2] || 
                    history_to[j] != opening_book[line][j * 2 + 1]) {
                    match_found = 0;
                    break;
                }
            }
            if (match_found && opening_book[line][history_count * 2] != 0) {
                *best_from = opening_book[line][history_count * 2];
                *best_to = opening_book[line][history_count * 2 + 1];
                current_eval = 0;
                engine_book_line = line;
                
                format_san(*best_from, *best_to, board[*best_from], board[*best_to], 0, 0, top_moves[0]);
                top_scores[0] = 0;
                top_count = 1;
                
                return 0; 
            }
        }
    }
    engine_book_line = -1;
    /* --- END BOOK LOOKUP --- */

    generate_moves(color, 0);
    score_and_sort_moves(0);
    count = move_counts[0];

    best_score = -30000;
    *best_from = 0;
    *best_to = 0;

    for (i = 0; i < count; i++) {
        from = move_from[i];
        to = move_to[i];
        
        moving_piece = board[from];
        captured_piece = board[to];

        make_move_full(from, to, &captured_piece, &en_passant_sq, &promoted);
        
        if (is_square_attacked(find_king(color), -color)) {
            unmake_move_full(from, to, captured_piece, prev_ep, promoted);
            continue;
        }
        legal_moves++;

        score = -alpha_beta(MAX_DEPTH - 1, -30000, 30000, -color, 1);
        is_chk = is_square_attacked(find_king(-color), color);
        unmake_move_full(from, to, captured_piece, prev_ep, promoted);

        for (j = 0; j < 5; j++) {
            if (j >= top_count || score > top_scores[j]) {
                for (k = 4; k > j; k--) {
                    if (k <= top_count) {
                        top_scores[k] = top_scores[k-1];
                        strcpy(top_moves[k], top_moves[k-1]);
                    }
                }
                top_scores[j] = score;
                format_san(from, to, moving_piece, captured_piece, is_chk, 0, top_moves[j]);
                if (top_count < 5) top_count++;
                break;
            }
        }

        if (score > best_score || legal_moves == 1) {
            best_score = score;
            *best_from = from;
            *best_to = to;
        }
    }

    if (legal_moves == 0) return -20000;

    current_eval = best_score * color;
    return best_score;
}

void render_dashboard(void) {
    int i, idx, d_score;
    int n;
    
    for (i = 0; i < 7; i++) {
        goto_xy(i + 1, 1);
        idx = i + scroll_offset;
        if (idx < history_count) {
            printf("%3d:%-6.6s", idx + 1, move_history[idx]);
        } else {
            printf("          "); 
        }

        goto_xy(i + 1, 11);
        if (i == 0) {
            printf("Top:    "); 
        } else if (i >= 1 && i <= 5) {
            if ((i - 1) < top_count) {
                d_score = top_scores[i - 1];
                if (d_score > 15000) printf("%-4.4s:M  ", top_moves[i - 1]);
                else if (d_score < -15000) printf("%-4.4s:-M ", top_moves[i - 1]);
                else {
                    d_score /= 10;
                    if (d_score > 99) d_score = 99;
                    if (d_score < -99) d_score = -99;
                    printf("%-4.4s:%-3d", top_moves[i - 1], d_score);
                }
            } else {
                printf("        "); 
            }
        } else {
            printf("        "); 
        }

        goto_xy(i + 1, 23);
        if (i == 0) {
            n = nodes_searched;
            if (n > 9999) n = 9999;
            printf("N:%-4d", n);
        }
        else if (i == 1) printf("D:%-1d   ", current_depth);
        else if (i == 2) printf("Turn: ");
        else if (i == 3) printf("YOU   ");
        else if (i == 4) {
            if (engine_book_line >= 0) printf("Bk:%-3d", engine_book_line + 1);
            else printf("       ");
        }
        else printf("      "); 
    }
}

void get_user_move(void) {
    char input[15], clean_input[15], coord_buf[15], san_buf[15];
    int pos = 0, j;
    int c, from, to;
    unsigned char b_from, b_to;
    char san_str[15];
    int moving_piece, captured_piece, promoted, prev_ep = en_passant_sq;
    int is_chk, is_mat;
    long d;
    int i, move_idx = -1;

    clear_row(8);
    goto_xy(8, 1);
    printf("Move: ");

    while (1) {
        c = getch();
        if (c == 27) { clear_row(8); return; }
        if (c == '\r' || c == '\n') {
            if (pos > 0) break;
        } else if (c == 8 || c == 127) {
            if (pos > 0) {
                pos--;
                goto_xy(8, 7 + pos);
                putchar(' ');
                goto_xy(8, 7 + pos);
            }
        } else if (pos < 10 && c > 32 && c <= 126) { 
            input[pos++] = c;
            putchar(c);
        }
    }
    input[pos] = '\0';

    j = 0;
    for (i = 0; input[i] != '\0'; i++) {
        if (isalnum(input[i]) || input[i] == '-') {
            clean_input[j++] = tolower(input[i]);
        }
    }
    clean_input[j] = '\0';

    generate_moves(1, 0);
    for (i = 0; i < move_counts[0]; i++) {
        sq_to_str(move_from[i], coord_buf);
        sq_to_str(move_to[i], coord_buf + 2);
        str_tolower(coord_buf);
        if (strcmp(clean_input, coord_buf) == 0) {
            move_idx = i;
            break;
        }
        format_san(move_from[i], move_to[i], board[move_from[i]], board[move_to[i]], 0, 0, san_buf);
        str_tolower(san_buf);
        if (strcmp(clean_input, san_buf) == 0) {
            move_idx = i;
            break;
        }
    }

    if (move_idx != -1) {
        from = move_from[move_idx];
        to = move_to[move_idx];
        moving_piece = board[from];
        captured_piece = board[to];
        
        make_move_full(from, to, &captured_piece, &en_passant_sq, &promoted);
        
        if (is_square_attacked(find_king(1), -1)) {
            unmake_move_full(from, to, captured_piece, prev_ep, promoted);
            goto_xy(8, 1);
            printf("In Check!             ");
            for(d = 0; d < 30000; d++); 
            clear_row(8);
            return;
        }

        is_chk = is_square_attacked(find_king(-1), 1);
        is_mat = (is_chk && !has_legal_moves(-1));
        format_san(from, to, moving_piece, captured_piece, is_chk, is_mat, san_str);
        
        if (moving_piece == 6) { w_king_moved = 1; }
        if (from == 21) { w_rook_a_moved = 1; }
        if (from == 28) { w_rook_h_moved = 1; }

        if (history_count < 50) {
            history_from[history_count] = from;
            history_to[history_count] = to;
            strcpy(move_history[history_count++], san_str);
            if (history_count > 7) scroll_offset = history_count - 7;
        }

        render_dashboard();
        
        if (is_mat) {
            strcpy(engine_last_move, "Mate");
        } else if (!has_legal_moves(-1)) {
            strcpy(engine_last_move, "Draw");
        } else {
            goto_xy(4, 23); printf("ENG   ");
            clear_row(8);
            goto_xy(8, 1);
            printf("Engine thinking...    ");

            prev_ep = en_passant_sq;
            find_best_move(-1, &b_from, &b_to);
            
            moving_piece = board[b_from];
            captured_piece = board[b_to];
            
            make_move_full(b_from, b_to, &captured_piece, &en_passant_sq, &promoted);
            
            is_chk = is_square_attacked(find_king(1), -1);
            is_mat = (is_chk && !has_legal_moves(1));
            format_san(b_from, b_to, moving_piece, captured_piece, is_chk, is_mat, san_str);
            strcpy(engine_last_move, san_str);

            if (moving_piece == -6) { b_king_moved = 1; }
            if (b_from == 91) { b_rook_a_moved = 1; }
            if (b_from == 98) { b_rook_h_moved = 1; }

            if (history_count < 50) {
                history_from[history_count] = b_from;
                history_to[history_count] = b_to;
                strcpy(move_history[history_count++], san_str);
                if (history_count > 7) scroll_offset = history_count - 7;
            }
        }
    } else {
        goto_xy(8, 1);
        printf("Invalid Move!         ");
        for(d = 0; d < 30000; d++); 
    }
    clear_row(8);
}

int main(void) {
    int key;
    int needs_redraw = 1;
    
    init_display();
    goto_xy(1, 1);
    printf("Reset Game? (y/n): ");
    while(1) {
        key = getch();
        if (key == 27) break; 
        key = tolower(key);
        if (key == 'y') {
            reset_game();
            break;
        } else if (key == 'n') {
            if (history_count > 7) scroll_offset = history_count - 7;
            else scroll_offset = 0;
            break;
        }
    }
    putchar(12);

    while (1) {
        if (needs_redraw) {
            render_dashboard();
            needs_redraw = 0;
        }
        key = getk(); 
        if (key == 0) continue;
        key = tolower(key);

        if (key == 'w') { 
            if (scroll_offset > 0) {
                scroll_offset--;
                needs_redraw = 1;
            }
        }
        if (key == 's') { 
            if (scroll_offset < history_count - 7) {
                scroll_offset++;
                needs_redraw = 1;
            }
        }
        if (key == 'u') {
            if (history_count >= 2) {
                replay_to(history_count - 2);
                needs_redraw = 1;
            } else if (history_count == 1) {
                replay_to(0);
                needs_redraw = 1;
            }
        }
        if (key == ' ') { 
            get_user_move();
            needs_redraw = 1; 
        }

        /* Quit Key */
        if (key == 'q') {
            clear_row(8);
            goto_xy(8, 1);
            printf("Exiting engine...     ");
            break; /* Breaks the while(1) loop, returning 0 at the end of main() */
        }

    }
    return 0;
}
