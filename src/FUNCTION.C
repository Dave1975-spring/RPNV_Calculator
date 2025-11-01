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

void stackx_by_exp()
{
    int i;

    if (stackx_exp>0) for (i=1 ; i<= stackx_exp; i++) stack[0] = stack[0] * 10.0;
    if (stackx_exp<0) for (i=-1 ; i>= stackx_exp; i--) stack[0] = stack[0] / 10.0;

    stackx_exp = 0;
    stackx_exp_hit = false;
}

void get_disp_mode_dec_digit() // used when selecting FIX, ENG, SCI display modes
{
    char c = 0;
    while (c<48 || c>57) c = getch();
    disp_mode_dec_digit = (int)c - 48; // to store the number of digits to be used when setting a new display mode
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
    int sign;

    if (enter_hit) {    // ENTER has been hit, start a new number entry
	stack[0] = num;
	stackx_digit = 1;
	stackx_dec_digit = 0;
	stackx_dec = false; 
	enter_hit = false;
    } 
    else if (func_hit) { // a function has been hit, start a new number entry
	push_stack();   // several function request the stack to be pushed before a new number entry
	stack[0] = num; 
	stackx_digit = 1;
	stackx_dec_digit = 0;
	stackx_dec = false; 
	func_hit = false;
    }
    else if (stackx_digit < MAXDIGITS) { // add the digit pressed to compose the new number under entering
	sign = (stack[0]<0.0) ? -1.0 : 1.0;
	if (stackx_dec==false) stack[0] = stack[0]*10.0 + sign * num;
	else {
	    stackx_dec_digit = stackx_dec_digit + 1;
	    for (i=1; i<=stackx_dec_digit; i++) num = num / 10.0;
	    stack[0] = stack[0] + sign * num;
	}
	stackx_digit = stackx_digit + 1; // take track of the number of digits already input
    } else print_message(14,"Too many digits"); // the max number of digits have been already input
}

void delete_PRGM_row() // delete the row at prgm_index, when backspace is pressed
{
    int i,j;
    if (prgm_index>=1) {
	for (i=prgm_index;i<99;i++)  // shift down the row values after the current line at prgm_index
	    for (j=0;j<3;j++) 
		strcpy(prgm_list[i][j],prgm_list[i+1][j]);

	strcpy(prgm_list[99][0],"  "); // insert the default values in the last row at index 99
	strcpy(prgm_list[99][1],"22");
	strcpy(prgm_list[99][2],"00");

	if (prgm_index==prgm_index_max) prgm_index--;
	prgm_index_max--; 
    }
}

void insert_PRGM_row() // insert a row in the program list, the default behaviour, different from real HP10C
{
    int i,j;
    for (i=99;i>prgm_index;i--) // push the rows content one row upward
	for (j=0;j<3;j++) 
	    strcpy(prgm_list[i][j],prgm_list[i-1][j]);
}

void fill_PRGM_row(int pos0, int pos1, int pos2)
{
    char buffer[3];
    if ((prgm_index_max==(MAXPRGMLIST-1)) && (GTO_hit==2)) GTO_hit=0; // if already at last line, avoid to enter in GTO loop
    if ( (prgm_index_max<(MAXPRGMLIST-1)) ||  // if there's room for another step ...
	((prgm_index_max==(MAXPRGMLIST-1)) && (GTO_hit==1))) { 
	if (GTO_hit!=1) { // if GTO_hit == 1 still creating the entry for the GTO function, no new line has to be created
			  // if GTO line under composition (GTO_hit==1), then override the row
	    insert_PRGM_row(); // new line to be created, make space for it 
	    prgm_index++;      // increase the index by one
	    prgm_index_max++;  // increase the last program row by one
	    if (prgm_index>prgm_index_max) prgm_index_max = prgm_index; // prgm_index can be moved beyond prgm_index_max with GTO at any time 
	}
	strcpy(prgm_list[prgm_index][0],(pos0?itoa(pos0,buffer,10):"  ")); // if input is 0, store "  " (NULL)
	strcpy(prgm_list[prgm_index][1],(pos1?itoa(pos1,buffer,10):"  ")); // if input is 0, store "  " (NULL)
	strcpy(prgm_list[prgm_index][2],itoa(pos2,buffer,10)); // if input is 0, write "0" since it's the 0 digit
    }
}

void manage_number_PRGM(int curpos)
// dedicated function to manage the keypress of a digit button in programming mode
// When in programming mode, as well as during execution, the result depends on what has 
// been pressed before the digit button
{ 
    char buffer[3];
    int buffer_int;
    if (disp_mode_hit) {  // disp mode selection called
	fill_PRGM_row(42,(7+disp_mode),curpos_to_button[curpos]);
	disp_mode_hit = false;
    } else if (store_hit) {  // STOre function called
	if (storage_arith!=0) { // STOrage upon math calculation to be performed
	    fill_PRGM_row(44,(50-storage_arith*10),curpos_to_button[curpos]);
	    storage_arith = 0;
	} else fill_PRGM_row(0,44,curpos_to_button[curpos]);
	store_hit = false;
	func_hit = true; 
    } else if (recall_hit) {  // STOre function called
	fill_PRGM_row(0,45,curpos_to_button[curpos]);
	recall_hit = false;
	func_hit = true; 
    } else if (GTO_to==2) { // when GTO + . just pressed in PRGM mode
	GTO_to_index = curpos_to_button[curpos];
	GTO_to=1;
    } else if (GTO_to==1) { // when GTO + . pressed in PRGM mode
	prgm_index = GTO_to_index * 10 + curpos_to_button[curpos];
	GTO_to=0;
    } else if (GTO_hit==0) { // non function called, just number hit
	fill_PRGM_row(0,0,curpos_to_button[curpos]);
    } else if (GTO_hit==1) {  // GTO function called
	buffer_int = atoi(prgm_list[prgm_index][2]) * 10 + curpos_to_button[curpos]; // convert string to int to manipulate
	fill_PRGM_row(0,22,buffer_int);
	GTO_hit--; 
    } else if (GTO_hit==2) {  // GTO function called
	fill_PRGM_row(0,22,curpos_to_button[curpos]);
	GTO_hit--; 
    } 
}

void manage_arith_PRGM(int curpos, int arith)
{
    char buffer[3];
    if (store_hit==true) storage_arith = arith; 
    else fill_PRGM_row(0,0,curpos_to_button[curpos]);
}

void exec_PRGM_row()
{
    int i;

    if (prgm_index>=FIRSTPRGMSTEP) {
	if (prgm_index==0) prgm_index++; // if at first row (0) goes directly to prgm row 1
	for (i=0;i<3;i++) {
	    if (strcmp(prgm_list[prgm_index][i],"  ")) // hit only the not NULL value stored
		hit_button_at_curpos(button_to_curpos[atoi(prgm_list[prgm_index][i])]);
		if (GTO_in_use) {
		    GTO_in_use = false;
		    break; // GTO function in use, skip directly to next prgm_index;
		}
	}
	prgm_index++;
	update_lcd();
    }
    if (prgm_index==MAXPRGMLIST) prgm_index = FIRSTPRGMSTEP; // if reached over the last position, back to first position
    update_lcd();
}

void manage_number_RUN_EXEC(int num) // dedicated function to manage number hit when in RUN or EXEC mode
{
    if (store_hit==true) store_memory(num);
    else if (recall_hit==true) recall_memory(num);
    else if (GTO_to==2) { 
	GTO_to_index = num;
	GTO_to=1;
    }
    else if (GTO_to==1) {
	prgm_index = GTO_to_index * 10 + num;
	GTO_to=0;
    }
    else if (stackx_exp_hit==true) add_number_exp(num);
    else if (disp_mode_hit==true) {
	disp_mode_dec_digit = num;
	disp_mode_hit = false;
    } 
    else {
	add_number(num); 
	number_hit = true;
    }
}
