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

void show_backside()   // show calc backside if B is pressed
{
    _settextwindow(1,1,25,80);
    _setbkcolor(0);
    _settextcolor(7); 
    _clearscreen(_GWINDOW); 

    _settextposition(1,1);
    _outtext("                                                                               \n"); 
    _outtext("      \332\304\277  \332\304\277  \332\304\277                  \332\304\304\304\304\304\304\304\304\304\304\304\304\304\304\304\304\277\n");
    _outtext("      \263 \303\277 \263 \303\277 \263 \303\277       RPNV      \263         STACK  \263       \030    x\n");
    _outtext("    + \263 \263\263 \263 \263\263 \263 \263\263 -   Calculator  \263 FUNC   \304\304\304\304\304\304\304 \263       \263........\n");
    _outtext("      \263 \303\331 \263 \303\331 \263 \303\331                 \263         X   Y  \263       \263      /.\n"); 
    _outtext("      \300\304\331  \300\304\331  \300\304\331                  \303\304\304\304\304\304\304\304\304\304\304\304\304\304\304\304\304\264       \263     / .\n");
    _outtext("   \332\304\304\304\304\304\304\304\304\304\304\304\304\304\304\304\304\304\304\304\304\304\304\304\304\304\277       \263  \032P     r   \351  \263 \270     \263  r /  .\n");
    _outtext("   \263 cm \366 2.54         \032 in  \263       \303\304\304\304\304\304\304\304\304\304\304\304\304\304\304\304\304\264 \263\032    \263   /   . y\n");
    _outtext("   \263 kg * 2.204622622  \032 lbm \263       \263  \032R     x   y  \263 \276     \263  /    .\n");
    _outtext("   \263 l \366 3.785411784   \032 gal \263       \303\304\304\304\304\304\304\304\304\304\304\304\304\304\304\304\304\264       \263 /     .\n"); 
    _outtext("   \263 \370C * 1.8 + 32     \032 F   \263       \263   x     x   y  \263       \263/ \351    .\n");
    _outtext("   \300\304\304\304\304\304\304\304\304\304\304\304\304\304\304\304\304\304\304\304\304\304\304\304\304\304\331       \303\304\304\304\304\304\304\304\304\304\304\304\304\304\304\304\304\264       \305\304\304\304\304\304\304\304\304\304\304\304\032\n"); 
    _outtext("                                     \263   s     sx  sy \263\n"); 
    _outtext("       \332\304\304\304\304\304\304\304\304\304\304\304\304\304\304\304\304\304\277           \303\304\304\304\304\304\304\304\304\304\304\304\304\304\304\304\304\264\n");
    _outtext("       \263     \344+ / \344-     \263           \263  y,r    y   r  \263       \030      /\n"); 
    _outtext("       \303\304\304\304\304\304\304\304\304\304\304\304\304\304\304\304\304\304\264           \303\304\304\304\304\304\304\304\304\304\304\304\304\304\304\304\304\264       \263     /\n");
    _outtext("       \263          Memory \263           \263  x,y    x   r  \263       \263    /.\n");
    _outtext("       \263  n     \032   0    \263           \303\304\304\304\304\304\304\304\304\304\304\304\304\304\304\304\304\264       \263   / .\n");
    _outtext("       \263  \344x    \032   1    \263           \263  L.R.   B   A  \263 \304\032    \263  /  . y\n");
    _outtext("       \263  \344x^2  \032   2    \263           \303\304\304\304\304\304\304\304\304\304\304\304\304\304\304\304\304\264       \263 /....\n");
    _outtext("       \263  \344y    \032   3    \263           \263        x*y     \263       \263/  x\n");
    _outtext("       \263  \344y^2  \032   4    \263           \263   %    \304\304\304  y  \263     B /\n");
    _outtext("       \263  \344xy   \032   5    \263           \263        100     \263      /\263    A=y/x\n");
    _outtext("       \300\304\304\304\304\304\304\304\304\304\304\304\304\304\304\304\304\304\331           \300\304\304\304\304\304\304\304\304\304\304\304\304\304\304\304\304\331     / \305\304\304\304\304\304\304\304\304\304\304\304\032\n"); 
    getch();
    init_calc_screen();
    update_curpos(NOMOVE);
    update_lcd();
}
