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

/*************** INCLUDE STATEMENTS ***************/

#include <stdio.h>
#include <conio.h>
#include <graph.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>
#include <i86.h>
#include <errno.h>
#include <time.h>

/*************** DEFINE STATEMENTS ***************/

#define VERSION "0.9.16"
#define LOGFILE "data.log"
#define offset_col 7
#define offset_row 8
#define pitch_col 7
#define pitch_row 4
#define MAXDIGITS 12
#define NOMOVE 0
#define UP 1
#define DOWN 2
#define LEFT 3
#define RIGHT 4
#define A 12  // used in Spouge's approximation for Gamma function
#ifndef M_PI
#define M_PI 3.14159265359
#endif

/*************** Variables definitions ***************/

double stack[4] = {0.0,0.0,0.0,0.0}; // stack>: X,Y,Z,T registers
double lastx = 0.0; // last x register
double memory[10] = {0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0}; // memory 
int ang_mode = 0;
char ang_mode_txt[3][4] = {"DEG","RAD","GRD"};
int disp_mode = 0;
int disp_mode_dec_digit = 9; // track how many decimal digits for DISP mode selected
char disp_mode_txt[3][4] = {"FIX","SCI","ENG"};
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
bool disp_mode_hit = false; // track if FIX or SCI or ENG has been just hit
bool number_hit = false; // track if a number has been just hit
bool second_f = false; // track if f second function button has been just hit
bool second_f_datalog = false; // keep track if second f button for datalog purpouse only 
bool show_stack = false; // track if the stack has to be show when LCD is updated
char *butt_base[] = {"\373x","e^x","10^x","y^x","1/x","CHS","7","8","9","\366", 
		     "%","GTO","SIN","COS","TAN","EEX","4","5","6","*",
		     "R/S","SST","R\031","x\035y","CLx","ENTER","1","2","3","-",
		     "ON","2ndF","P/R","STO","RCL","ENTER","0",".","\344+","+"};
char *butt_2ndF[] = {"x\375","LN","LOG","\032H.MS","\032H","\343","FIX","SCI","ENG","x\363y", 
		     "\032R","\032P","SIN-1","COS-1","TAN-1","x!","DEG","RAD","GRD","x=0",
		     "PSE","BST","PRGM","REG","PREFX","Lastx","x,r","y,r","L.R.","\032RAD",
		     "HELP","2ndF","MEM","INT","FRC","Lastx","x","s","\344-","\032DEG"};

FILE *fp; // pointer to buttonslog file



/*************** Functions declaration ***************/

// In main.c 
void main();

// In datalog.c
void start_datalog();
void update_datalog(int);

// In mainloop.c - main loop called by main function
// this is the main loop tracking the key pressed by the user and addressing actions
int main_loop(); 

// In layout.c
// all the functions related to the user interface, calc layout, etc.
void initial_steps(); //
void closure_steps(); //
void draw_border(); //
void draw_std_button(int posy, int posx, char *sup, char *smid, char *slow); //
void draw_f_button(); //
void draw_enter_button(); //
void init_calc_screen(); //
void print_message(int color,char *message); //
void update_lcd_badge(); //
void update_lcd(); //
void update_curpos(int dir); //

// In math.c
// all maths functions, except the simplest
double sp_gamma(double z);
double factorial(double number);
void pol_to_rec();
void rec_to_pol();
double H_to_HMS(double n);
double HMS_to_H(double h1);
double rad_to_deg(double n);
double deg_to_rad(double n);
void convert_ang(int s);
void back_convert_ang(int s);
void sigma_plus();
void sigma_minus();
void mean_x_y();
void stddev_x_y();
double int_part(double x);
double fract_part(double x);

// In mem.c
// all functions related to memory managed, stack, registers, etc 
void show_full_stack(); // show stack content on calc layout
void clear_full_stack(); // clear the screen where the stack is shown
void push_stack();
void pull_stack();
void rotate_stack();
void swap_stackxy();
void show_memory();  // show the registers content on calc layout
void store_memory(int mempos);
void recall_memory(int mempos);
void clear_memory();

// In function.c
// all function related to numbers management and modification expect strictly mathematical functions
void define_digits();
void stackx_by_exp();
void get_disp_mode_dec_digit();
void add_number_exp(int num); // move the unit to tens, tens to hundreds and add a new unit 
void add_number(double num);

// In help.c
// help function
void show_help();   // HELP window if H is pressed

// In buttons.c
// define what each calc button does
void hit_button_at_curpos(int curpos);

// In mouse.c
// all functions related to mouse management
int init_mouse();
void show_mouse();
void hide_mouse();
void double_speed_mouse();
void get_mouse( int *x, int *y, int *left, int*right );
int mouse_position(int mouse_x, int mouse_y);

