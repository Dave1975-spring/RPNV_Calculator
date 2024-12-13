/*
 * MIT License
 *
 * Copyright (c) 2024 Davide Erbetta
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


#include <stdio.h>
#include <conio.h>
#include <graph.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>
#include <i86.h>

#define offset_col 7
#define offset_row 8
#define pitch_col 7
#define pitch_row 4
#define M_PI 3.14159265359
#define MAXDIGITS 12

double stack[4] = {0.0,0.0,0.0,0.0}; // stack>: X,Y,Z,T registers
double lastx = 0.0; // last x register
double memory[10] = {0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0}; // memory 
int ang_mode = 0;
char ang_mode_txt[3][4] = {"DEG","RAD","GRD"};
int curpos = 26;  // track the cursor position, moved by arrow keys
int stackx_digit = 0;  // total number of digits in stack X
int stackx_dec_digit = 0; // number of decimal digits in stack X
int stackx_exp = 0; // 10^ exponent of stackx value
int stackx_exp_digit = 0; // numer of digit inserted in stackx_exp
bool stackx_dec = false; // true = decimal number ; false = integer
bool stackx_exp_hit = false; // track if EEX button has been just pressed
bool enter_hit = false; // track if ENTER has been just hit
bool func_hit = false; // track if a standard function key has been just hit
bool store_hit = false; // track if the STO button has been just hit
bool recall_hit = false; // track if the RCL button has been just hit
bool second_f = false; // track if f second function button has been just hit
bool show_stack = false; // track if the stack has to be show when LCD is updated

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

    draw_std_button(4,1,""," ON","");
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

void show_full_stack()
{
    char text_stack[35];
    _setbkcolor(7);
    _settextcolor(0);
    _settextwindow(2,45,6,77);
    _clearscreen(_GWINDOW);
    _settextposition(1,1);

    if ((fabs(stack[3])>1.0E12) || (fabs(stack[3])<1.0E-12)) {
	sprintf(text_stack,"     T:%1.*E\n",(MAXDIGITS-1),stack[3]);
    }
    else sprintf(text_stack,"     T:%0*.*Lf\n",(MAXDIGITS*2+1),MAXDIGITS,stack[3]);
    _outtext(text_stack);

    if ((fabs(stack[2])>1.0E12) || (fabs(stack[2])<1.0E-12)) {
	sprintf(text_stack,"     Z:%1.*E\n",(MAXDIGITS-1),stack[2]);
    }
    else sprintf(text_stack,"     Z:%0*.*Lf\n",(MAXDIGITS*2+1),MAXDIGITS,stack[2]);
    _outtext(text_stack);

    if ((fabs(stack[1])>1.0E12) || (fabs(stack[1])<1.0E-12)) {
	sprintf(text_stack,"     Y:%1.*E\n",(MAXDIGITS-1),stack[1]);
    }
    else sprintf(text_stack,"     Y:%0*.*Lf\n",(MAXDIGITS*2+1),MAXDIGITS,stack[1]);
    _outtext(text_stack);

    if ((fabs(stack[0])>1.0E12) || (fabs(stack[0])<1.0E-12)) {
	sprintf(text_stack,"     X:%1.*E\n",(MAXDIGITS-1),stack[0]);
    }
    else sprintf(text_stack,"     X:%0*.*Lf\n",(MAXDIGITS*2+1),MAXDIGITS,stack[0]);
    _outtext(text_stack);

    if ((fabs(lastx)>1.0E12) || (fabs(lastx)<1.0E-12)) {
	sprintf(text_stack,"Last X:%1.*E",(MAXDIGITS-1),lastx);
    }
    else sprintf(text_stack,"Last X:%0*.*Lf",(MAXDIGITS*2+1),MAXDIGITS,lastx);
    _outtext(text_stack);
}

void define_digits()
{
    char text_stackx[35];
    int i;
    int tmp_dec_digit = 0; // tmp_dec_digit track potential digits, to be confirmed
    char c;
    bool point = false; // track if a dot has been found in the string
    bool dec_digit = false; // track if decimal digit <> 0 has been found

    if ((fabs(stack[0])>1.0E12) || (fabs(stack[0])<1.0E-12)) sprintf(text_stackx,"%1.*E",MAXDIGITS,stack[0]);
    else sprintf(text_stackx,"%*.*f",(MAXDIGITS*2+1),MAXDIGITS,stack[0]); 

    stackx_digit = 0;
    stackx_dec_digit = 0;

    for (i=0; i<strlen(text_stackx); i++) {
	c = text_stackx[i];
	if (point==false) {     // counts the digits before the dot
	    if (c>=49 && c<=57) stackx_digit++;
	    if (c=='.') point = true;
	    if (stackx_digit==MAXDIGITS) break; 
	} else {        // counts the digits after the dot, excluding the last zeros
	    if (c>=48 && c<=57) tmp_dec_digit++;
	    if (c>=49 && c<=57) stackx_dec_digit = tmp_dec_digit;
	    if (c=='E') break;
	    if ((stackx_digit+tmp_dec_digit)==MAXDIGITS) break;
	}
    }
    stackx_digit = stackx_digit + stackx_dec_digit;

    if (stackx_digit==0) stackx_digit = 1; // in case of 0.0 at least 1 digits
}

void update_lcd()
{
    char text_lcd[25];
    char text_exp[5];

    // create the string to be shown in LCD

    if ( ((fabs(stack[0])<1.0E12) && (fabs(stack[0])>1.0E-12)) || (stack[0]==0.0) ) {
	if (stackx_dec) sprintf(text_lcd,"%#1.*Lf",stackx_dec_digit,stack[0]);
	else sprintf(text_lcd,"%1.*Lf",stackx_dec_digit,stack[0]);
    }
    else sprintf(text_lcd,"%1.*E",stackx_dec_digit,stack[0]);

    _setbkcolor(3);
    _settextcolor(0);
    _settextwindow(3,15,5,42);
    _clearscreen(_GWINDOW);
    _settextposition(2,4); 
    _outtext(text_lcd);

    // show badges as f-function, STO, RCL, RAD, etc.

    _settextposition(3,3);
    if (second_f==false) _outtext(" ");
    else _outtext("f"); 

    _settextposition(3,5);
    if (store_hit==false) _outtext("   ");
    else _outtext("STO"); 

    _settextposition(3,9);
    if (recall_hit==false) _outtext("   ");
    else _outtext("RCL"); 

    // display angular mode

    _settextposition(3,13);
    _outtext(ang_mode_txt[ang_mode]); 

    if (stackx_exp_hit==true) {
	_settextposition(2,21);
	sprintf(text_exp,"E%+04i",stackx_exp);
	_outtext(text_exp);
    }

    if (show_stack) show_full_stack();
}

double factorial_int(double number)
{
    int i;

    for (i=(int)(number-1); i>0; i--) number *= i;

    return number;
}

void update_curpos(char *dir)
{
    char *testo;
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

    if (dir=="up") {
	if (curposy==1) curposy = 4;
	else curposy = curposy - 1; 
    }
    if (dir=="down") {
	if (curposy==4) curposy = 1;
	else curposy = curposy + 1;
    }
    if (dir=="left") {
	if (curposx==1) curposx = 10;
	else curposx = curposx - 1;
    }
    if (dir=="right") {
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

    //return curpos; 
}

void clear_full_stack() // clear the screen where the stack is shown
{
    _setbkcolor(7);
    _settextwindow(2,45,6,77);
    _clearscreen(_GWINDOW); 
}

void push_stack()
{
    stack[3] = stack[2];
    stack[2] = stack[1];
    stack[1] = stack[0];
}

void pull_stack()
{
    stack[1] = stack[2];
    stack[2] = stack[3];
    stack[3] = 0.0;
}

void rotate_stack()
{
    double buf;
    buf = stack[0];
    stack[0] = stack[1];
    stack[1] = stack[2];
    stack[2] = stack[3];
    stack[3] = buf; 
}

void swap_stackxy()
{
    double buf;
    buf = stack[0];
    stack[0] = stack[1];
    stack[1] = buf;
}

void show_memory()  // show the registers content
{
    int i=0;
    char text_memory[28];

    _settextwindow(8,21,23,60);
    _setbkcolor(2);
    _settextcolor(7); 
    _clearscreen(_GWINDOW); 

    _settextposition(2,12);
    _outtext("Registers content:");

    for (i=0; i<10; i++) {
	_settextposition(i+4,7);
	if ((fabs(memory[i])>1.0E12) || (fabs(memory[i])<1.0E-12)) {
	    sprintf(text_memory,"%1i: %1.*E\n",i,MAXDIGITS,memory[i]);
	}
	else sprintf(text_memory,"%1i: %0*.*Lf\n",i,(MAXDIGITS*2+1),MAXDIGITS,memory[i]);
	_outtext(text_memory);
    } 

    _outtext("\n       Press any key to continue");

    getch();
    init_calc_screen();
    update_curpos("");
    update_lcd();
}

void store_memory(int mempos)
{
    memory[mempos] = stack[0]; 
    store_hit = false;
    func_hit = true;
}

void recall_memory(int mempos)
{
    push_stack();
    stack[0] = memory[mempos]; 
    define_digits();
    recall_hit = false;
    func_hit = true;
}

void clear_memory()
{
    int i;
    for (i=0; i<10; i++) memory[i] = 0.0;
}

void stackx_by_exp()
{
    int i;

    if (stackx_exp>0) for (i=1 ; i<= stackx_exp; i++) stack[0] = stack[0] * 10.0;
    if (stackx_exp<0) for (i=-1 ; i>= stackx_exp; i--) stack[0] = stack[0] / 10.0;

    stackx_exp = 0;
    stackx_exp_hit = false;
}


void convert_ang()
{
    switch (ang_mode) {
	case 0: stack[0] = stack[0] * M_PI / 180.0; break;
	case 1: break;
	case 2: stack[0] = stack[0] * M_PI / 200.0; break;
    }
}

void back_convert_ang()
{
    switch (ang_mode) {
	case 0: stack[0] = stack[0] / M_PI * 180.0; break;
	case 1: break;
	case 2: stack[0] = stack[0] / M_PI * 200.0; break;
    }
}

void add_number_exp(int num) // move the unit to tens, tens to hundreds and add a new unit 
{
    int buf;
    buf = (int)(stackx_exp/100); // get the hundreds
    stackx_exp = (stackx_exp - buf*100) * 10 + num; // subtract the hundreds and add the unit
}

void add_number(double num)
{
    int i;

    if (enter_hit) {
	stack[0] = num;
	stackx_digit = 1;
	stackx_dec_digit = 0;
	stackx_dec = false; 
	enter_hit = false;
    } 
    else if (func_hit) {
	push_stack();
	stack[0] = num; 
	stackx_digit = 1;
	stackx_dec_digit = 0;
	stackx_dec = false; 
	func_hit = false;
    }
    else if (stackx_digit < MAXDIGITS) {
	if (stackx_dec==false) {
	    stack[0] = stack[0]*10.0 + num;
	}
	else {
	    stackx_dec_digit = stackx_dec_digit + 1;
	    for (i=1; i<=stackx_dec_digit; i++) {
		num = num / 10.0;
	    }
	    stack[0] = stack[0] + num;

	}
	stackx_digit = stackx_digit + 1;
    }
    else {
	print_message(14,"Too many digits");
    }
}

void hit_button_at_curpos(int curpos) 
// the behaviour of each button is defined here
// for most of the button the behaviour depends if second f function has been just hit
// or, in case of numbers buttons, if STO and RCL buttons have been just hit
{
    switch (curpos) {
	case 1:  // sqrt(x) / x^2
	    if (store_hit) store_hit = false;
	    if (recall_hit) recall_hit = false;
	    if (stackx_exp_hit==true) stackx_by_exp();
	    if (second_f==false) {
		lastx = stack[0];
		stack[0] = sqrt(stack[0]);
		pull_stack();
		func_hit = true; 
	    }
	    else {
		lastx = stack[0];
		stack[0] = pow(stack[0],2.0);
		pull_stack();
		func_hit = true;
		second_f = false; 
	    }
	    define_digits();
	    update_lcd();
	    break; 
	case 2:  // e^x / LN
	    if (store_hit) store_hit = false; 
	    if (recall_hit) recall_hit = false;
	    if (stackx_exp_hit==true) stackx_by_exp();
	    if (second_f==false) {
		lastx = stack[0];
		stack[0] = exp(stack[0]);
		pull_stack();
		func_hit = true; 
	    }
	    else {
		lastx = stack[0];
		stack[0] = log(stack[0]);
		pull_stack();
		func_hit = true;
		second_f = false; 
	    }
	    define_digits();
	    update_lcd();
	    break; 
	case 3:  // 10^x / LOG10
	    if (store_hit) store_hit = false; 
	    if (recall_hit) recall_hit = false; 
	    if (stackx_exp_hit==true) stackx_by_exp();
	    if (second_f==false) {
		lastx = stack[0];
		stack[0] = pow(10.0,stack[0]);
		pull_stack();
		func_hit = true; 
	    }
	    else {
		lastx = stack[0];
		stack[0] = log10(stack[0]);
		pull_stack();
		func_hit = true;
		second_f = false; 
	    }
	    define_digits();
	    update_lcd();
	    break; 
	case 4:  // y^x /
	    if (store_hit) store_hit = false; 
	    if (recall_hit) recall_hit = false; 
	    if (second_f==false) {
		if (stackx_exp_hit==true) stackx_by_exp();
		lastx = stack[0];
		stack[0] = pow(stack[1],stack[0]);
		pull_stack();
		func_hit = true; 
	    }
	    else {
		print_message(14,"Not yet implemented");
	    }
	    define_digits();
	    update_lcd();
	    break;
	case 5:  // 1/x / 
	    if (store_hit) store_hit = false; 
	    if (recall_hit) recall_hit = false; 
	    if (second_f==false) {
		if (stackx_exp_hit==true) stackx_by_exp();
		lastx = stack[0];
		stack[0] = 1.0 / stack[0];
		pull_stack();
		func_hit = true; 
	    }
	    else {
		print_message(14,"Not yet implemented");
	    }
	    define_digits();
	    update_lcd();
	    break;
	case 6:  // CHS / PI
	    if (store_hit) store_hit = false; 
	    if (recall_hit) recall_hit = false; 
	    if (second_f==false) {
		if (stackx_exp_hit==true) stackx_exp *= -1;
		else stack[0] = stack[0] * -1.0;
	    }
	    else {
		push_stack();
		stack[0] = M_PI;
		func_hit = true;
		second_f = false;
	    }
	    define_digits();
	    update_lcd();
	    break;
	case 7:  // 7
	    if (second_f==false) {
		if (store_hit==true) store_memory(7);
		else if (recall_hit==true) recall_memory(7);
		else if (stackx_exp_hit==true) add_number_exp(7);
		else add_number(7.0);
	    } else {
		print_message(14,"Not yet implemented");
	    }
	    update_lcd();
	    break;
	case 8:  // 8
	    if (second_f==false) {
		if (store_hit==true) store_memory(8);
		else if (recall_hit==true) recall_memory(8);
		else if (stackx_exp_hit==true) add_number_exp(8);
		else add_number(8.0);
	    } else {
		print_message(14,"Not yet implemented");
	    } 
	    update_lcd();
	    break;
	case 9:  // 9
	    if (second_f==false) {
		if (store_hit==true) store_memory(9);
		else if (recall_hit==true) recall_memory(9);
		else if (stackx_exp_hit==true) add_number_exp(9);
		else add_number(9.0);
	    } else {
		print_message(14,"Not yet implemented");
	    } 
	    update_lcd();
	    break;
	case 10: // /
	    if (store_hit) store_hit = false; 
	    if (recall_hit) recall_hit = false;
	    if (second_f==false) {
		if (stackx_exp_hit==true) stackx_by_exp();
		if (store_hit) store_hit = false; 
		lastx = stack[0];
		stack[0] = stack[1]/stack[0];
		pull_stack();
		func_hit = true;
	    } else {
		print_message(14,"Not yet implemented");
	    } 
	    define_digits(); 
	    update_lcd(); 
	    break;
	case 11: // % / 
	    if (store_hit) store_hit = false; 
	    if (recall_hit) recall_hit = false;
	    if (second_f==false) {
		lastx = stack[0];
		stack[0] = stack[1]/100.0*stack[0];
		pull_stack();
		func_hit = true;
	    } else {
		print_message(14,"Not yet implemented");
	    } 
	    define_digits(); 
	    update_lcd(); 
	    break;
	case 13: // SIN / ASIN
	    if (store_hit) store_hit = false; 
	    if (recall_hit) recall_hit = false; 
	    if (stackx_exp_hit==true) stackx_by_exp();
	    if (second_f == false) {
		lastx = stack[0];
		convert_ang();
		stack[0] = sin(stack[0]);
		pull_stack();
		func_hit = true;
	    }
	    else {
		if ((stack[0]>=-1.0 && stack[0]<=1.0)) {
		    lastx = stack[0];
		    stack[0] = asin(stack[0]);
		    back_convert_ang();
		    pull_stack();
		    func_hit = true;
		    second_f = false;
		} else print_message(6,"number outside function domain");
	    }
	    define_digits();
	    update_lcd();
	    break;
	case 14: // COS / ACOS
	    if (store_hit) store_hit = false; 
	    if (recall_hit) recall_hit = false;
	    if (stackx_exp_hit==true) stackx_by_exp();
	    if (second_f == false) {
		lastx = stack[0];
		convert_ang();
		stack[0] = cos(stack[0]);
		pull_stack();
		func_hit = true;
	    }
	    else {
		if ((stack[0]>=-1.0 && stack[0]<=1.0)) { 
		lastx = stack[0];
		    stack[0] = acos(stack[0]);
		    back_convert_ang(); 
		    pull_stack();
		    func_hit = true;
		    second_f = false;
		} else print_message(6,"number outside function domain"); 
	    }
	    define_digits();
	    update_lcd();
	    break;
	case 15: // TAN / ATAN
	    if (store_hit) store_hit = false; 
	    if (recall_hit) recall_hit = false; 
	    if (stackx_exp_hit==true) stackx_by_exp();
	    lastx = stack[0];
	    if (second_f == false) {
		convert_ang();
		stack[0] = tan(stack[0]);
	    }
	    else {
		stack[0] = atan(stack[0]);
		back_convert_ang(); 
		second_f = false;
	    }
	    pull_stack();
	    func_hit = true;
	    define_digits();
	    update_lcd();
	    break;
	case 16: // EEX / FACTORIAL
	    if (store_hit) store_hit = false; 
	    if (recall_hit) recall_hit = false; 
	    if (second_f==false) {
		if (stackx_exp_hit==false) {
		    if ((func_hit) || (stack[0]==0.0)) { 
			lastx=stack[0];
			stack[0] = 1.0;
		    }
		    stackx_exp_hit = true;
		}
	    } 
	    else {
		if (stackx_dec_digit==0) {
		    if (stackx_exp_hit==true) stackx_by_exp();
		    stack[0] = factorial_int(stack[0]);
		    func_hit = true;
		    second_f = false;
		    define_digits();
		} else {
		    print_message(14,"Factorial implemented only for integer"); 
		}
	    }
	    define_digits();
	    update_lcd();
	    break; 
	case 17: // 4 / DEG
	    if (second_f==false) {
		if (store_hit==true) store_memory(4); 
		else if (recall_hit==true) recall_memory(4); 
		else if (stackx_exp_hit==true) add_number_exp(4);
		else add_number(4.0);
	    } else {
		ang_mode = 0;
		second_f = false;
	    } 
	    update_lcd();
	    break;
	case 18: // 5 / RAD
	    if (second_f==false) {
		if (store_hit==true) store_memory(5); 
		else if (recall_hit==true) recall_memory(5); 
		else if (stackx_exp_hit==true) add_number_exp(5);
		else add_number(5.0);
	    } else {
		ang_mode = 1;
		second_f = false;
	    } 
	    update_lcd();
	    break;
	case 19: // 6 / GRD
	    if (second_f==false) {
		if (store_hit==true) store_memory(6); 
		else if (recall_hit==true) recall_memory(6); 
		else if (stackx_exp_hit==true) add_number_exp(6);
		else add_number(6.0);
	    } else {
		ang_mode = 2;
		second_f = false;
	    } 
	    update_lcd();
	    break;
	case 20: // * 
	    if (store_hit) store_hit = false; 
	    if (recall_hit) recall_hit = false; 
	    if (second_f==false) {
		if (stackx_exp_hit==true) stackx_by_exp();
		lastx = stack[0]; 
		stack[0] = stack[1]*stack[0];
		pull_stack();
		func_hit = true;
	    } else {
		print_message(14,"Not yet implemented");
	    } 
	    define_digits();
	    update_lcd();
	    break;
	case 23: // rotate stack
	    if (store_hit) store_hit = false; 
	    if (recall_hit) recall_hit = false; 
	    if (second_f==false) {
		if (stackx_exp_hit==true) stackx_by_exp();
		rotate_stack();
	    }
	    else {
		print_message(14,"Not yet implemented");
	    } 
	    define_digits();
	    update_lcd();
	    break;
	case 24: // x<>y / clear Registers 0-9
	    if (store_hit) store_hit = false; 
	    if (recall_hit) recall_hit = false; 
	    if (second_f==false) {
		if (stackx_exp_hit==true) stackx_by_exp();
		swap_stackxy();
	    }
	    else {
		clear_memory();
		second_f = false;
	    }
	    define_digits();
	    update_lcd();
	    break; 
	case 25: // CLx / 
	    if (store_hit) store_hit = false; 
	    if (recall_hit) recall_hit = false; 
	    if (second_f==false) {
		stack[0] = 0.0;
		stackx_dec = false;
	    }
	    else {
		print_message(14,"Not yet implemented");
	    }
	    define_digits();
	    update_lcd();
	    break;
	case 26: // ENTER / LastX --> ensure identical code for case 36, still ENTER
	    if (store_hit) store_hit = false; 
	    if (recall_hit) recall_hit = false; 
	    if (second_f==false) {
		if (stackx_exp_hit==true) stackx_by_exp();
		push_stack();
		enter_hit = true;
	    }
	    else {
		push_stack();
		stack[0] = lastx;
		second_f = false;
		func_hit = true;
	    }
	    define_digits();
	    update_lcd();
	    break;
	case 27: // 1
	    if (second_f==false) {
		if (store_hit==true) store_memory(1); 
		else if (recall_hit==true) recall_memory(1); 
		else if (stackx_exp_hit==true) add_number_exp(1);
		else add_number(1.0);
	    } else {
		print_message(14,"Not yet implemented");
	    } 
	    update_lcd();
	    break;
	case 28: // 2
	    if (second_f==false) {
		if (store_hit==true) store_memory(2); 
		else if (recall_hit==true) recall_memory(2); 
		else if (stackx_exp_hit==true) add_number_exp(2);
		else add_number(2.0);
	    } else {
		print_message(14,"Not yet implemented");
	    } 
	    update_lcd(); 
	    break;
	case 29: // 3
	    if (second_f==false) { 
		if (store_hit==true) store_memory(3); 
		else if (recall_hit==true) recall_memory(3); 
		else if (stackx_exp_hit==true) add_number_exp(3);
		else add_number(3.0);
	    } else {
		print_message(14,"Not yet implemented");
	    } 
	    update_lcd();
	    break;
	case 30: // -
	    if (store_hit) store_hit = false; 
	    if (recall_hit) recall_hit = false; 
	    if (second_f==false) {
		if (stackx_exp_hit==true) stackx_by_exp(); 
		lastx = stack[0]; 
		stack[0] = stack[1]-stack[0];
		pull_stack();
		func_hit = true;
	    } else {
		print_message(14,"Not yet implemented");
	    } 
	    define_digits();
	    update_lcd(); 
	    break;
	case 32: // f - 2nd function
	    if (store_hit) store_hit = false;
	    if (recall_hit) recall_hit = false; 
	    if (second_f==false) second_f = true;
	    else second_f = false;
	    update_lcd();
	    break;
	case 34: // STO / INT
	    if (second_f==false) { 
		if (stackx_exp_hit==true) stackx_by_exp();
		if (store_hit==false) {
		    store_hit = true;
		    if (recall_hit) recall_hit = false;
		    if (second_f) second_f = false;
		} else store_hit = false;
	    } else {
		lastx = stack[0];
		stack[0] = (int)stack[0];
		second_f = false;
		func_hit = true;
		define_digits(); 
	    } 
	    update_lcd();
	    break;
	case 35: // RCL / FRAC
	    if (second_f==false) { 
		if (stackx_exp_hit==true) stackx_by_exp();
		if (recall_hit==false) {
		    recall_hit = true;
		    if (store_hit) store_hit = false;
		    if (second_f) second_f = false;
		} else recall_hit = false;
	    } else {
		lastx = stack[0];
		stack[0] = stack[0] - (int)stack[0];
		second_f = false;
		func_hit = true;
		define_digits(); 
	    } 
	    update_lcd();
	    break;
	case 36: // ENTER / LastX --> ensure identical code for case 26, still ENTER
	    if (store_hit) store_hit = false; 
	    if (recall_hit) recall_hit = false; 
	    if (second_f==false) {
		if (stackx_exp_hit==true) stackx_by_exp();
		push_stack();
		enter_hit = true;
	    }
	    else {
		push_stack();
		stack[0] = lastx;
		second_f = false;
		func_hit = true;
	    }
	    define_digits();
	    update_lcd();
	    break;
	case 37: // 0
	    if (second_f==false) { 
		if (store_hit==true) store_memory(0); 
		else if (recall_hit==true) recall_memory(0); 
		else if (stackx_exp_hit==true) add_number_exp(0);
		else add_number(0.0);
	    } else {
		print_message(14,"Not yet implemented");
	    } 
	    update_lcd(); 
	    break;
	case 38: // .
	    if (store_hit) store_hit = false; 
	    if (recall_hit) recall_hit = false; 
	    if (second_f==false) { 
		if (stackx_exp_hit==true) stackx_exp_hit = false;
		if (enter_hit) {
		    stack[0] = 0.0;
		    stackx_digit = 1;
		    stackx_dec_digit = 0;
		    enter_hit = false;
		    stackx_dec = true;
		} 
		else if (func_hit) {
		    push_stack();
		    stack[0] = 0.0; 
		    stackx_digit = 1;
		    stackx_dec_digit = 0; 
		    func_hit = false;
		    stackx_dec = true;
		} else if (stackx_dec==false) stackx_dec = true;
	    } else {
		print_message(14,"Not yet implemented");
	    } 
	    update_lcd();
	    break;
	case 40: // +
	    if (store_hit) store_hit = false; 
	    if (recall_hit) recall_hit = false; 
	    if (second_f==false) { 
		if (stackx_exp_hit==true) stackx_by_exp();
		lastx = stack[0]; 
		stack[0] = stack[1]+stack[0];
		pull_stack();
		func_hit = true;
	    } else {
		print_message(14,"Not yet implemented");
	    } 
	    define_digits();
	    update_lcd(); 
	    break;
	default:
	    print_message(14,"Not yet implemented");
	    break;
    }
}

void show_help()   // HELP window if H is pressed
{
    _settextwindow(2,13,23,68);
    _setbkcolor(5);
    _settextcolor(15); 
    _clearscreen(_GWINDOW); 

    _settextposition(2,1);
    _outtext(" RPNV 0.5.2 is an RPN calc inspired by HP Voyager calc\n");
    _outtext("        Made for fun by Davide Erbetta in 2024        \n"); 
    _outtext(" Developed in C in FreeDos with FED and OpewWatcom 1.9\n\n");
    _outtext("    ----------------------------------------------    \n");
    _outtext("    See README and LICENSE documents before use it    \n");
    _outtext("    ----------------------------------------------    \n\n"); 
    _outtext("                      INSTRUCTIONS                    \n\n");
    _outtext("   Use arrow keys and SPACE bar to select the button  \n\n");
    _outtext("      Some shortcuts from keyboard are available:     \n");
    _outtext("  All numbers + - / * . ENTER key can be used         \n");
    _outtext("  T --> roTaTe stack R\031    K --> toggle show stack    \n");
    _outtext("  L --> recall last X      M --> show registers       \n");
    _outtext("  S --> STOre in register  R --> ReCaLl from register \n"); 
    _outtext("  P --> \343                  F --> second function     \n");
    _outtext("  E --> EEX add 10^ exp.                              \n"); 
    _outtext("     Not all functions have been implemented yet      \n");

    getch();
    init_calc_screen();
    update_curpos("");
    update_lcd();
}

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

    if (mouse_pos < 0) mouse_pos = 0; // if negative, so no button, then set to zero;
    if (mouse_pos == 36) second_f = false; // no second function for buttons 36

    return mouse_pos;
}

int main_loop()  // this is the main loop tracking the key pressed by the user
{
    int c = 0;
    int mouse_x,mouse_y,mouse_left,mouse_right;
    char mouse_text[30];

    get_mouse(&mouse_x,&mouse_y,&mouse_left,&mouse_right);

    if (mouse_left) {
	hit_button_at_curpos(mouse_position(mouse_x,mouse_y));
	delay(250);  // give sum time to unpress the mouse button ...
    }

    if (kbhit()) {
	c = getch();
	if ( c ==(char)0 ) {    // if special keys are pressed, as arrows
	    c = getch();
	    switch (c) {
		case 72: // UP
		    update_curpos("up");
		    break;
		case 80: // DOWN
		    update_curpos("down");
		    break;
		case 75: // LEFT
		    update_curpos("left");
		    break;
		case 77: // RIGHT
		    update_curpos("right");
		    break; 
	    } 
	} else switch (c) {
	    case 13: // ENTER
		hit_button_at_curpos(26);
		break; 
	    case 27: // !!! ESC !!!
		return -1;
	    case 32: // SPACE
		hit_button_at_curpos(curpos);
		break;
	    case 42: // *
		hit_button_at_curpos(20);
		break;
	    case 43: // +
		hit_button_at_curpos(40);
		break;
	    case 45: // -
		hit_button_at_curpos(30);
		break;
	    case 46: // .
		hit_button_at_curpos(38);
		break;
	    case 47: // /
		hit_button_at_curpos(10);
		break;
	    case 48: // 0
		hit_button_at_curpos(37);
		break;
	    case 49: // 1
		hit_button_at_curpos(27);
		break;
	    case 50: // 2
		hit_button_at_curpos(28);
		break;
	    case 51: // 3
		hit_button_at_curpos(29);
		break;
	    case 52: // 4
		hit_button_at_curpos(17);
		break;
	    case 53: // 5
		hit_button_at_curpos(18);
		break;
	    case 54: // 6
		hit_button_at_curpos(19);
		break;
	    case 55: // 7
		hit_button_at_curpos(7);
		break;
	    case 56: // 8
		hit_button_at_curpos(8); 
		break;
	    case 57: // 9
		hit_button_at_curpos(9); 
		break;
	    case 101: // e for EEX
		second_f = false;
		hit_button_at_curpos(16);
		break;
	    case 102: // f for SECOND FUNCTION
		hit_button_at_curpos(32);
		break;
	    case 104: // h for HELP
		show_help();
		break;
	    case 107: // k for STACK
		if (show_stack==false) {
		    show_stack = true;
		    show_full_stack();
		} else {
		    show_stack = false;
		    clear_full_stack();
		}
		break;
	    case 108: // l for LAST X
		second_f = true;
		hit_button_at_curpos(26);
		break;
	    case 109: // m for show memory registers
		show_memory(); 
		break;
	    case 112: // p for PI
		second_f = true;
		hit_button_at_curpos(6);
		break;
	    case 114: // R for RCL 
		second_f = false;
		hit_button_at_curpos(35);
		break;
	    case 115: // S for STO
		second_f = false;
		hit_button_at_curpos(34);
		break;
	    case 116: // T for R|v , roTaTe stack
		second_f = false;
		hit_button_at_curpos(23);
		break;
	    default:
		return 0;
	}
    }
    return 0;
}

void main()
{
    int result = 0;

    initial_steps();    // set video mode

    init_calc_screen(); // draw the calculator layout

    init_mouse();

    show_mouse();

    update_curpos("");   // move button cursor to defaul position - ENTER key

    update_lcd();       // update the lcd screen with starting value

    print_message(10,"ESC to exit - H for HELP"); 

    while (result >= 0) result = main_loop();        // main calc loop

    closure_steps();    // return to default video mode
}
