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
 * - Liamtoh Resu for the several tests done and bugs highlighted
 * - Albert Chan for his help in understand & correct the DMS<>DD bug
 *
 ****************************************************************************
 *
 * To be compiled with Open Watcom 1.9
 *
 */

#include "rpnv.h"

void initial_steps()
{
    _setvideomode(_TEXTC80);
    _displaycursor(_GCURSOROFF);
    _clearscreen(_GCLEARSCREEN);
}

void closure_steps()
{
    _displaycursor(_GCURSORON);
    _setbkcolor(0);
    _settextcolor(7);
    _clearscreen(_GCLEARSCREEN); 
    _setvideomode(_DEFAULTMODE);
}

void draw_border()
{
    int col;
    int row;

    _settextcolor(7);
    _settextposition(7,3);
    _outtext("\332");
    _settextposition(24,3);
    _outtext("\324");
    _settextposition(7,78);
    _outtext("\277");
    _settextposition(24,78);
    _outtext("\276");

    for (row=8; row<24; row++)
    {
	_settextposition(row,3);
	_outtext("\263");
	_settextposition(row,78);
	_outtext("\263");
    } 

    for (col=4; col<78; col++)
    {
	_settextposition(7,col);
	_outtext("\304");
    }

    for (col=4; col<78; col++)
    {
	_settextposition(24,col);
	_outtext("\315");
    }

    _settextposition(24,8);
    _outtext(" R P N - V O Y A G E R ");
}

void draw_std_button(int posy, int posx, char *sup, char *smid, char *slow)
{
    int row;
    int col;

    // calculate button position

    row = offset_row + (posy-1)*pitch_row;
    col = offset_col + (posx-1)*pitch_col;

    // create upper part and text

    _setbkcolor(0);
    _settextwindow(row,col,row,col+5);
    _clearscreen(_GWINDOW);

    _settextcolor(6);
    _settextposition(1,1);
    _outtext(sup);

    // create mid part and text

    _setbkcolor(7);
    _settextwindow(row+1,col,row+2,col+4);
    _clearscreen(_GWINDOW);

    _settextcolor(0);
    _settextposition(1,1);
    _outtext(smid);

    // create bottom text

    _settextcolor(0);
    _settextposition(2,1);
    _outtext(slow);
}

void draw_f_button()
{
    int row = 4;
    int col = 2;

    // calculate button position

    row = offset_row + (4-1)*pitch_row;
    col = offset_col + (2-1)*pitch_col;

    // create mid part and text

    _setbkcolor(6);
    _settextwindow(row+1,col,row+2,col+4);
    _clearscreen(_GWINDOW);

    _settextcolor(0);
    _settextposition(1,1);
    _outtext("  f"); 
}

void draw_enter_button()
{
    int row = 0;
    int col = 0;

    // calculate button position

    row = offset_row + (3-1)*pitch_row;
    col = offset_col + (6-1)*pitch_col;

    // create upper part and text

    _setbkcolor(0);
    _settextwindow(row,col,row,col+5);
    _clearscreen(_GWINDOW);

    _settextcolor(6);
    _settextposition(1,1);
    _outtext("LASTx");

    // create mid part and text

    _setbkcolor(7);
    _settextwindow(row+1,col,row+6,col+4);
    _clearscreen(_GWINDOW);

    _settextcolor(0);
    _settextposition(1,1);
    _outtext("  E"); 
    _settextposition(2,1);
    _outtext("  N"); 
    _settextposition(3,1);
    _outtext("  T"); 
    _settextposition(4,1);
    _outtext("  E"); 
    _settextposition(5,1);
    _outtext("  R");
}

void init_calc_screen()
{
    // create black background

    _setbkcolor(0);
    _settextwindow(1,1,25,80);
    _clearscreen(_GWINDOW);

    // draw border

    draw_border();

    // print inner label

    _settextcolor(6);
    _settextposition(15,21);
    _outtext("\332\304\304\304\304\304\304CLEAR\304\304\304\304\304\304\277");

    // create the upper display area

    // create grey upper area
    _setbkcolor(7);
    _settextwindow(2,3,6,78);
    _clearscreen(_GWINDOW);

    // draw standard buttons

    // draw 1st row

    draw_std_button(1,1," x\375"," \373x","");
    draw_std_button(1,2," LN"," e^x",""); 
    draw_std_button(1,3," LOG"," 10^x","");
    draw_std_button(1,4,"\032H.MS"," y^x","");
    draw_std_button(1,5," \032H"," 1/x","");
    draw_std_button(1,6,"  \343"," CHS","");
    draw_std_button(1,7," FIX","  7","");
    draw_std_button(1,8," SCI","  8","");
    draw_std_button(1,9," ENG","  9","");
    draw_std_button(1,10," x\363y","  \366",""); 

    // draw 2nd row

    draw_std_button(2,1," \032R","  %","");
    draw_std_button(2,2," \032P"," GTO",""); 
    draw_std_button(2,3,"SIN-1"," SIN","");
    draw_std_button(2,4,"COS-1"," COS","");
    draw_std_button(2,5,"TAN-1"," TAN","");
    draw_std_button(2,6," x!"," EEX","");
    draw_std_button(2,7," DEG","  4","");
    draw_std_button(2,8," RAD","  5","");
    draw_std_button(2,9," GRD","  6","");
    draw_std_button(2,10," x=0","  *",""); 

    // draw 3rd row

    draw_std_button(3,1," PSE"," R/S","");
    draw_std_button(3,2," BST"," SST",""); 
    draw_std_button(3,3," PRGM"," R\031","");
    draw_std_button(3,4," REG"," x\035y","");
    draw_std_button(3,5,"PREFX"," CLx","");
    draw_enter_button();
    draw_std_button(3,7," x,r","  1","");
    draw_std_button(3,8," y,r","  2","");
    draw_std_button(3,9," L.R.","  3","");
    draw_std_button(3,10," \032RAD","  -",""); 

    // draw 4th row

    draw_std_button(4,1," HELP"," ON","");
    draw_f_button();
    draw_std_button(4,3," MEM"," P/R","");
    draw_std_button(4,4," INT"," STO","");
    draw_std_button(4,5," FRC"," RCL","");

    draw_std_button(4,7,"  x","  0","");
    draw_std_button(4,8,"  s","  .","");
    draw_std_button(4,9," \344-"," \344+","");
    draw_std_button(4,10," \032DEG","  +",""); 
}

void print_message(int color,char *message)
{
    // print message at the bottom of the screen, last row

    _settextwindow(25,1,25,80);
    _setbkcolor(0);
    _settextcolor(color);
    _clearscreen(_GWINDOW);
    _settextposition(1,40-strlen(message)/2-14);
    _outtext(message);
    _outtext(" - press any key to continue");
    getch();
    _clearscreen(_GWINDOW); 
}

void update_lcd_badge()
{
    _setbkcolor(3);
    _settextcolor(0);
    _settextwindow(5,15,5,42);
    _clearscreen(_GWINDOW);

    // show badges as f-function, STO, RCL, RAD, etc. in LCD area

    _settextposition(1,3);
    if (second_f==false) _outtext(" ");
    else _outtext("f"); 

    _settextposition(1,5);
    if (store_hit==false) _outtext("   ");
    else _outtext("STO"); 

    _settextposition(1,9);
    if (recall_hit==false) _outtext("   ");
    else _outtext("RCL"); 

    // display angular mode

    _settextposition(1,13);
    _outtext(ang_mode_txt[ang_mode]); 

    // display DISP mode

    _settextposition(1,19);
    _outtext(disp_mode_txt[disp_mode]); 
}

void update_lcd()
{
    char text_lcd[25];  // text to display the number
    char text_exp[5];   // text to display the exponential entry if EEX is pressed
    float mantissa;
    int counter = 0;

    // set the LCD area to be filled

    _setbkcolor(3);
    _settextcolor(0);
    _settextwindow(3,15,4,42);
    _clearscreen(_GWINDOW);
    _settextposition(2,4); 

    // create the string to be shown in LCD

    if ((number_hit) || (stackx_exp_hit)) {
	if (stackx_dec) sprintf(text_lcd,"% #1.*Lf",stackx_dec_digit,stack[0]);
	else sprintf(text_lcd,"% 1.*Lf",stackx_dec_digit,stack[0]);
	number_hit = false;
	_outtext(text_lcd);

	// display entry for exponenent if EEX has been pressed

	if (stackx_exp_hit) {
	    _settextposition(2,21);
	    sprintf(text_exp,"E%+04i",stackx_exp);
	    _outtext(text_exp);
	}
    } 
    else {
	switch (disp_mode) {
	    case 0:     // FIX
		if ( ((fabs(stack[0])<1.0E12) && (fabs(stack[0])>1.0E-12)) || (stack[0]==0.0) ) {
		    if (stackx_dec) sprintf(text_lcd,"% #1.*Lf",disp_mode_dec_digit,stack[0]);
		    else sprintf(text_lcd,"% 1.*Lf",disp_mode_dec_digit,stack[0]);
		}
		else sprintf(text_lcd,"% 1.*E",disp_mode_dec_digit,stack[0]);
		break;
	    case 1:     // SCI
		sprintf(text_lcd,"% 1.*E",disp_mode_dec_digit,stack[0]);
		break;
	    case 2:     // ENG
		mantissa = stack[0];
		if (fabs(mantissa) > 1.0) {
		    while (mantissa > 1.0E3) {
			mantissa = mantissa / 1.0E3;
			counter++;
		    }
		} else {
		    while (mantissa < 1.0 && mantissa!=0.0) {
			mantissa = mantissa * 1E3;
			counter--;
		    }
		}
		sprintf(text_lcd,"% 1.*LfE%03i",disp_mode_dec_digit,mantissa,(counter*3));
		break;
	    default:    // Standard
		if ( ((fabs(stack[0])<1.0E12) && (fabs(stack[0])>1.0E-12)) || (stack[0]==0.0) ) {
		    if (stackx_dec) sprintf(text_lcd,"% #1.*Lf",stackx_dec_digit,stack[0]);
		    else sprintf(text_lcd,"% 1.*Lf",stackx_dec_digit,stack[0]);
		}
		else sprintf(text_lcd,"% 1.*E",stackx_dec_digit,stack[0]);
		break;
	}
	_outtext(text_lcd);
    }

    update_lcd_badge(); // call function to update badge area

    if (show_stack) show_full_stack(); // show the stack + Last-x stack if requested
}

void update_curpos(int dir)
{
    int cur_row,cur_col;
    int curposx,curposy;

    curposy = (curpos-1) / 10 + 1;
    curposx = curpos - (curposy-1) * 10;

    // calculate previous cursor coordinates position

    cur_row = offset_row + (curposy-1)*pitch_row +1;
    cur_col = offset_col + (curposx-1)*pitch_col +5;

    // erase cursor at previous position

    _setbkcolor(0);
    _settextwindow(cur_row,cur_col,cur_row+1,cur_col+1);
    _clearscreen(_GWINDOW);
    _settextposition(1,1);
    _outtext(" ");
    _settextposition(2,1);
    _outtext(" ");

    // update cursor position

    if (dir==UP) {
	if (curposy==1) curposy = 4;
	else curposy = curposy - 1; 
    }
    if (dir==DOWN) {
	if (curposy==4) curposy = 1;
	else curposy = curposy + 1;
    }
    if (dir==LEFT) {
	if (curposx==1) curposx = 10;
	else curposx = curposx - 1;
    }
    if (dir==RIGHT) {
	if (curposx==10) curposx = 1;
	else curposx = curposx + 1;
    }

    curpos = (curposy-1) * 10 + curposx;

    // calculate new cursor coordinates position

    cur_row = offset_row + (curposy-1)*pitch_row +1;
    cur_col = offset_col + (curposx-1)*pitch_col +5;

    // draw cursor at new position

    _setbkcolor(0);
    _settextwindow(cur_row,cur_col,cur_row+1,cur_col+1);
    _settextcolor(15);
    _clearscreen(_GWINDOW);
    _settextposition(1,1);
    _outtext("\021"); 
    _settextposition(2,1);
    _outtext("\021"); 
}
