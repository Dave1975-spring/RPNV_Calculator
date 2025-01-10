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
/*
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
*/
void stackx_by_exp()
{
    int i;

    if (stackx_exp>0) for (i=1 ; i<= stackx_exp; i++) stack[0] = stack[0] * 10.0;
    if (stackx_exp<0) for (i=-1 ; i>= stackx_exp; i--) stack[0] = stack[0] / 10.0;

    stackx_exp = 0;
    stackx_exp_hit = false;
}

void get_disp_mode_dec_digit()
{
    char c = 0;
    while (c<48 || c>57) c = getch();
    disp_mode_dec_digit = (int)c - 48;
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
	sign = (stack[0]<0.0) ? -1.0 : 1.0;
	if (stackx_dec==false) {
	    stack[0] = stack[0]*10.0 + sign * num;
	}
	else {
	    stackx_dec_digit = stackx_dec_digit + 1;
	    for (i=1; i<=stackx_dec_digit; i++) {
		num = num / 10.0;
	    }
	    stack[0] = stack[0] + sign * num;

	}
	stackx_digit = stackx_digit + 1;
    }
    else {
	print_message(14,"Too many digits");
    }
}
