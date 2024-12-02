#include <stdio.h>
#include <conio.h>
#include <graph.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>

#define offset_col 7
#define offset_row 8
#define pitch_col 7
#define pitch_row 4
#define M_PI 3.14159265359
#define MAXDIGITS 12

double stack[4] = {0.0,0.0,0.0,0.0}; // stack>: X,Y,Z,T registers
double lastx = 0.0; // last x register
double memory[10] = {0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0}; // memory 
int stackx_digit = 0;  // total number of digits in stack X
int stackx_dec_digit = 0; // number of decimal digits in stack X
int stackx_exp = 0; // 10^ exponent of stackx value
bool stackx_dec = false; // true = decimal number ; false = integer
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
    char c;
    bool point = false; // track if a dot has been found in the string

    if ((fabs(stack[0])>1.0E12) || (fabs(stack[0])<1.0E-12)) sprintf(text_stackx,"%1.*E",MAXDIGITS,stack[0]);
    else sprintf(text_stackx,"%*.*f",(MAXDIGITS*2+1),MAXDIGITS,stack[0]); 

    stackx_digit = 0;
    stackx_dec_digit = 0;

    for (i=0; i<strlen(text_stackx); i++) {
	c = text_stackx[i];
	if (c>=49 && c<=57) stackx_digit++;
	if (c>=49 && c<=57 && point) stackx_dec_digit++;
	if (c=='.') point = true; 
    }

    if (stackx_digit==0) stackx_digit = 1; // in case of 0.0 at least 1 digits
}

void update_lcd()
{
    char text_lcd[25];

    // create the string to be shown in LCD

    if ((fabs(stack[0])>1.0E12) || (fabs(stack[0])<1.0E-12)) sprintf(text_lcd,"%1.*E",stackx_digit,stack[0]);
    else {
	if (stackx_dec) sprintf(text_lcd,"%#1.*Lf",stackx_dec_digit,stack[0]);
	else sprintf(text_lcd,"%1.*Lf",stackx_dec_digit,stack[0]);
    }
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

    _settextposition(3,13);
    _outtext("RAD");     // by now only rad mode implemented

    if (show_stack) show_full_stack();
}

double factorial_int(double number)
{
    int i;

    for (i=(int)(number-1); i>0; i--) number *= i;

    return number;
}

int update_curpos(char *dir,int curpos)
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

    return curpos; 
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

void show_memory(int curpos)  // show the registers content
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
    update_curpos("",curpos);
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
	    if (second_f==false) stack[0] = stack[0] * -1.0;
	    else {
		stack[0] = M_PI;
		//stackx_digit = 12;
		//stackx_dec_digit = 11;
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
	case 13: // SIN / ASIN
	    if (store_hit) store_hit = false; 
	    if (recall_hit) recall_hit = false; 
	    lastx = stack[0];
	    if (second_f == false) stack[0] = sin(stack[0]);
	    else {
		stack[0] = asin(stack[0]);
		second_f = false;
	    }
	    pull_stack();
	    func_hit = true;
	    define_digits();
	    update_lcd();
	    break;
	case 14: // COS / ACOS
	    if (store_hit) store_hit = false; 
	    if (recall_hit) recall_hit = false;
	    lastx = stack[0];
	    if (second_f == false) stack[0] = cos(stack[0]);
	    else {
		stack[0] = acos(stack[0]);
		second_f = false;
	    }
	    pull_stack();
	    func_hit = true;
	    define_digits();
	    update_lcd();
	    break;
	case 15: // TAN / ATAN
	    if (store_hit) store_hit = false; 
	    if (recall_hit) recall_hit = false; 
	    lastx = stack[0];
	    if (second_f == false) stack[0] = tan(stack[0]);
	    else {
		stack[0] = atan(stack[0]);
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
		print_message(14,"Not yet implemented"); 
	    } 
	    else {
		if (stackx_dec_digit==0) {
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
	case 17: // 4
	    if (second_f==false) {
		if (store_hit==true) store_memory(4); 
		else if (recall_hit==true) recall_memory(4); 
		else add_number(4.0);
	    } else {
		print_message(14,"Not yet implemented");
	    } 
	    update_lcd();
	    break;
	case 18: // 5
	    if (second_f==false) {
		if (store_hit==true) store_memory(5); 
		else if (recall_hit==true) recall_memory(5); 
		else add_number(5.0);
	    } else {
		print_message(14,"Not yet implemented");
	    } 
	    update_lcd();
	    break;
	case 19: // 6
	    if (second_f==false) {
		if (store_hit==true) store_memory(6); 
		else if (recall_hit==true) recall_memory(6); 
		else add_number(6.0);
	    } else {
		print_message(14,"Not yet implemented");
	    } 
	    update_lcd();
	    break;
	case 20: // * 
	    if (store_hit) store_hit = false; 
	    if (recall_hit) recall_hit = false; 
	    if (second_f==false) {
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
	    if (second_f==false) rotate_stack();
	    else {
		print_message(14,"Not yet implemented");
	    } 
	    define_digits();
	    update_lcd();
	    break;
	case 24: // x<>y /
	    if (store_hit) store_hit = false; 
	    if (recall_hit) recall_hit = false; 
	    if (second_f==false) swap_stackxy();
	    else {
		print_message(14,"Not yet implemented");
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
	case 26: // ENTER
	    if (store_hit) store_hit = false; 
	    if (recall_hit) recall_hit = false; 
	    push_stack();
	    if (second_f==false) enter_hit = true;
	    else {
		stack[0] = lastx;
		second_f = false;
		func_hit = true;
	    }
	    update_lcd();
	    break;
	case 27: // 1
	    if (second_f==false) {
		if (store_hit==true) store_memory(1); 
		else if (recall_hit==true) recall_memory(1); 
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
	case 34: // STO
	    if (second_f==false) { 
		if (store_hit==false) {
		    store_hit = true;
		    if (recall_hit) recall_hit = false;
		    if (second_f) second_f = false;
		} else store_hit = false;
	    } else {
		print_message(14,"Not yet implemented");
	    } 
	    update_lcd();
	    break;
	case 35: // RCL
	    if (second_f==false) { 
		if (recall_hit==false) {
		    recall_hit = true;
		    if (store_hit) store_hit = false;
		    if (second_f) second_f = false;
		} else recall_hit = false;
	    } else {
		print_message(14,"Not yet implemented");
	    } 
	    update_lcd();
	    break;
	case 36: // ENTER
	    if (store_hit) store_hit = false; 
	    if (recall_hit) recall_hit = false; 
	    push_stack();
	    if (second_f==false) enter_hit = true;
	    else {
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

void show_help(int curpos)   // HELP window if H is pressed
{
    _settextwindow(2,13,23,68);
    _setbkcolor(5);
    _settextcolor(15); 
    _clearscreen(_GWINDOW); 

    _settextposition(2,1);
    _outtext(" RPNV 0.1.0 is an RPN calc inspired by HP Voyager calc\n");
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
    _outtext("  P --> \343                  F --> second function     \n\n");
    _outtext("     Not all functions have been implemented yet      \n");

    getch();
    init_calc_screen();
    update_curpos("",curpos);
    update_lcd();
}

void main_loop()  // this is the main loop tracking the key pressed by the user
{
    int c = 0;
    int curpos = 26;

    while (c!=27) {
	c = getch();

	switch (c) {
	    case 13: // ENTER
		hit_button_at_curpos(26);
		continue; 
	    case 32: // SPACE
		hit_button_at_curpos(curpos);
		continue;
	    case 42: // *
		hit_button_at_curpos(20);
		continue;
	    case 43: // +
		hit_button_at_curpos(40);
		continue;
	    case 45: // -
		hit_button_at_curpos(30);
		continue;
	    case 46: // .
		hit_button_at_curpos(38);
		continue;
	    case 47: // /
		hit_button_at_curpos(10);
		continue;
	    case 48: // 0
		hit_button_at_curpos(37);
		continue; 
	    case 49: // 1
		hit_button_at_curpos(27);
		continue; 
	    case 50: // 2
		hit_button_at_curpos(28);
		continue; 
	    case 51: // 3
		hit_button_at_curpos(29);
		continue;
	    case 52: // 4
		hit_button_at_curpos(17);
		continue;
	    case 53: // 5
		hit_button_at_curpos(18);
		continue;
	    case 54: // 6
		hit_button_at_curpos(19);
		continue; 
	    case 55: // 7
		hit_button_at_curpos(7);
		continue; 
	    case 56: // 8
		hit_button_at_curpos(8); 
		continue; 
	    case 57: // 9
		hit_button_at_curpos(9); 
		continue; 
	    case 72: // UP
		curpos = update_curpos("up",curpos);
		continue;
	    case 80: // DOWN
		curpos = update_curpos("down",curpos);
		continue;
	    case 75: // LEFT
		curpos = update_curpos("left",curpos);
		continue;
	    case 77: // RIGHT
		curpos = update_curpos("right",curpos);
		continue;
	    case 102: // f for SECOND FUNCTION
		hit_button_at_curpos(32);
		continue;
	    case 104: // h for HELP
		show_help(curpos);
		continue;
	    case 107: // k for STACK
		if (show_stack==false) {
		    show_stack = true;
		    show_full_stack();
		} else {
		    show_stack = false;
		    clear_full_stack();
		}
		continue;
	    case 108: // l for LAST X
		second_f = true;
		hit_button_at_curpos(26);
		continue;
	    case 109: // m for show memory registers
		show_memory(curpos); 
		continue;
	    case 112: // p for PI
		second_f = true;
		hit_button_at_curpos(6);
		continue; 
	    case 114: // R for RCL 
		second_f = false;
		hit_button_at_curpos(35);
		continue;
	    case 115: // S for STO
		second_f = false;
		hit_button_at_curpos(34);
		continue;
	    case 116: // T for R|v , roTaTe stack
		second_f = false;
		hit_button_at_curpos(23);
		continue;
	}
    }
}

void main()
{
    initial_steps();    // set video mode

    init_calc_screen(); // draw the calculator layout

    update_curpos("",26);   // move button cursor to defaul position - ENTER key

    update_lcd();       // update the lcd screen with starting value

    print_message(10,"ESC to exit - H for HELP"); 

    main_loop();        // main calc loop

    closure_steps();    // return to default video mode
}

