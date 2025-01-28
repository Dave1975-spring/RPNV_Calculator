/*
 * MIT License
 *
 * Copyright (c) 2024-2025 Davide Erbetta
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 ****************************************************************************
 *
 * Special thanks to:
 * - FreeDOS development team
 * - Jim Hall for his great tutorials on C and CONIO
 * - Shawn Hargreaves for his great FED text editor
 * - "root42" for his very usefull MS-DOS programming tutorial
 *
 ****************************************************************************
 *
 * To be compiled with Open Watcom 1.9
 *
 */

#include "rpnv.h"

void show_help()   // HELP window if H is pressed
{
    char *firstrow;

    sprintf(firstrow," RPNV %s is an RPN calc inspired by HP Voyager calc\n",VERSION);

    _settextwindow(1,13,25,68);
    _setbkcolor(5);
    _settextcolor(15); 
    _clearscreen(_GWINDOW); 

    _settextposition(2,1);
    _outtext(firstrow);
    _outtext("     Made for fun by Davide Erbetta in 2024-2025      \n"); 
    _outtext(" Developed in C in FreeDos with FED and OpewWatcom 1.9\n\n");
    _outtext("        -> See README and LICENSE documents <-        \n\n"); 
    _outtext("                      INSTRUCTIONS                    \n\n");
    _outtext("   Use arrow keys and SPACE bar to select the button  \n");
    _outtext("         or use the mouse and the left button         \n\n");
    _outtext("      Some shortcuts from keyboard are available:     \n");
    _outtext("  All numbers + - / * . ENTER key can be used         \n");
    _outtext("  T --> roTaTe stack R\031    K --> toggle show stack    \n");
    _outtext("  L --> recall last X      M --> show registers       \n");
    _outtext("  S --> STOre in register  R --> ReCaLl from register \n"); 
    _outtext("  P --> \343                  F --> second function     \n");
    _outtext("  E --> EEX add 10^ exp.                              \n\n"); 
    _outtext("   A data.log file is created with all calc buttons   \n");
    _outtext("   pressed, their meaning, stack plus errors if any   \n\n");
    _outtext("      Not all functions have been implemented yet     \n");

    getch();
    init_calc_screen();
    update_curpos(NOMOVE);
    update_lcd();
}
