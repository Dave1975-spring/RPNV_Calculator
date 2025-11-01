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
 */

#include "rpnv.h"

void show_help()   // HELP window if H is pressed
{
    char firstrow[70];

    sprintf(firstrow,"            RPNV %s is an RPN calculator inspired by HP-10C\n",VERSION);

    _settextwindow(2,3,24,78);
    _setbkcolor(5);
    _settextcolor(15); 
    _clearscreen(_GWINDOW); 

    _settextposition(2,1);
    _outtext(firstrow);
    _outtext("                Made for fun by Davide Erbetta in 2024-2025              \n"); 
    _outtext("       Developed in C in FreeDOS with FED editor and OpewWatcom 1.9      \n");
    _outtext("                                                                         \n");
    _outtext("                   -> See README and LICENSE documents <-                \n");
    _outtext("                                                                         \n"); 
    _outtext("       For fuctions behaviour and programming refer to HP-10C manual     \n");
    _outtext("      Use arrow and SPACE to select the button or mouse or shortcuts:    \n");
    _outtext("                                                                         \n");
    _outtext("                All numbers + - / * . ENTER key can be used              \n");
    _outtext("  T -> roTaTe stack R\031       L -> recall last X    F -> second function  \n");
    _outtext("  S -> STOre in register     R -> ReCaLl register  G --> GOTO function   \n"); 
    _outtext("  E -> EEX add 10^ exp.      P -> \343                C -> CHS              \n");
    _outtext("  BACKSPACE -> CLx in EXEC mode, delete row in PRGM mode   H -> HELP     \n"); 
    _outtext("                                                                         \n");
    _outtext("                          Stand-alone functions:                         \n"); 
    _outtext("  M -> show memory content   K -> show/hide stack  ESC -> Quit RPNV calc \n");
    _outtext("  B -> show calc backside                                                                       \n");
    _outtext("   A 'data.log' file is created with buttons pressed and stack values    \n");
    _outtext(" A 'state.log' is created & read with stack, registers and program list  \n");
    _outtext(" In PROGRAM mode, new lines are instered in the list instead of overriden");

    getch();
    init_calc_screen();
    update_curpos(NOMOVE);
    update_lcd();
}
