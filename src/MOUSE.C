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

int init_mouse()
{
    union REGS reg;
    reg.x.ax = 0x00;
    int86( 0x33, &reg, &reg);

    return 0xFFFF == reg.x.ax;
}

void show_mouse()
{
    union REGS reg;
    reg.x.ax = 0x01;
    int86( 0x33, &reg, &reg);
}

void hide_mouse()
{
    union REGS reg;
    reg.x.ax = 0x02;
    int86( 0x33, &reg, &reg);
}

void double_speed_mouse()
{
    union REGS reg;
    reg.x.ax = 0x13;
    int86( 0x33, &reg, &reg);
}


void get_mouse( int *x, int *y, int *left, int*right )
{
    union REGS reg;
    reg.x.ax = 0x03;
    int86( 0x33, &reg, &reg);
    *x = reg.x.cx / 8 + 1;
    *y = reg.x.dx / 8 + 1;
    *left = reg.x.bx & 0x1;
    *right = reg.x.bx & 0x2;
}

int mouse_position(int mouse_x, int mouse_y)
{
    int mouse_pos;
    int mouse_row = -100;  // if position does not match with any button row or 
    int mouse_col = -100;  // column then mouse_pos will be negative

    // translate mouse coordinates (1 to 25 rows) in buttons coordinates
    // if directly the 2nd function is clicked, then set true the second_f flag

    if ((mouse_y >=  8) && (mouse_y <= 10)) mouse_row = 0;
    if  (mouse_y == 8) second_f = true;

    if ((mouse_y >= 12) && (mouse_y <= 14)) mouse_row = 1;
    if  (mouse_y == 12) second_f = true;

    if ((mouse_y >= 16) && (mouse_y <= 18)) mouse_row = 2;
    if  (mouse_y == 16) second_f = true;

    if ((mouse_y >= 20) && (mouse_y <= 22)) mouse_row = 3;
    if  (mouse_y == 20) second_f = true;

    // translate mouse coordinates (1 to 80 columns) in buttons coordinates

    if ((mouse_x >=  7) && (mouse_x <= 11)) mouse_col = 1; 
    if ((mouse_x >= 14) && (mouse_x <= 18)) mouse_col = 2; 
    if ((mouse_x >= 21) && (mouse_x <= 25)) mouse_col = 3; 
    if ((mouse_x >= 28) && (mouse_x <= 32)) mouse_col = 4; 
    if ((mouse_x >= 35) && (mouse_x <= 39)) mouse_col = 5; 
    if ((mouse_x >= 42) && (mouse_x <= 46)) mouse_col = 6; 
    if ((mouse_x >= 49) && (mouse_x <= 53)) mouse_col = 7; 
    if ((mouse_x >= 56) && (mouse_x <= 60)) mouse_col = 8; 
    if ((mouse_x >= 63) && (mouse_x <= 67)) mouse_col = 9; 
    if ((mouse_x >= 70) && (mouse_x <= 74)) mouse_col = 10; 

    mouse_pos = mouse_row * 10 + mouse_col;

    // special case for ENTER button, occupying also space between rows 3 and 4
    if ((mouse_x >= 42) && (mouse_x <= 46) && (mouse_y == 19)) mouse_pos = 26; 

    if (mouse_pos < 0) mouse_pos = 0; // if negative, so no button, then set to zero;
    if (mouse_pos == 36) second_f = false; // no second function for buttons 36
    if ((mouse_pos == 32) && (mouse_y == 20)) mouse_pos = 0; // no second function for 2ndF button

    return mouse_pos;
}
