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

void show_full_stack()  // show the stack values beside the calc display
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

void clear_full_stack() // hide the stack values beside the calc display
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

void show_prgm_list()  // show the program list content
{
    int i=0,j=0,k;
    char text_prgm_row[70];
    char text_pos[3][3];

    _settextwindow(2,3,24,78);
    _setbkcolor(1);
    _settextcolor(7); 
    _clearscreen(_GWINDOW); 

    _settextposition(1,4);
    sprintf(text_prgm_row,"Program index at: %2i        Last program row at: %2i",prgm_index,prgm_index_max);
    _outtext(text_prgm_row);

    _settextposition(2,4);
    sprintf(text_prgm_row,"Program title: %s",prgm_title);
    _outtext(text_prgm_row); 

    for (j=0; j<4; j++) 
	for (i=0; i<20; i++) {
	    _settextposition(i+3,3+15*j); 
	    k = i + j * 20;
	    sprintf(text_prgm_row,"%02i- %2s %2s %2s \263",k,prgm_list[k][0],prgm_list[k][1],prgm_list[k][2]);
	    _outtext(text_prgm_row);
	} 

    for (i=0; i<20; i++) {
	_settextposition(i+3,63); 
	k = i + 80;
	sprintf(text_prgm_row,"%02i- %2s %2s %2s",k,prgm_list[k][0],prgm_list[k][1],prgm_list[k][2]);
	_outtext(text_prgm_row);
    } 

    _settextposition(23,26);
    _outtext("Press any key to continue");

    getch();
    init_calc_screen();
    update_curpos(NOMOVE);
    update_lcd();
}

void clear_prgm_list() // if no state.log file is available to be read, the program list is initialized
{
    int i;

    strcpy(prgm_list[0][0],"  ");
    strcpy(prgm_list[0][1],"  ");
    strcpy(prgm_list[0][2],"  "); 

    for (i=1;i<MAXPRGMLIST;i++) {
	strcpy(prgm_list[i][0],"  ");
	strcpy(prgm_list[i][1],"22");
	strcpy(prgm_list[i][2],"00");
    } 
    prgm_index = FIRSTPRGMSTEP;
    prgm_index_max = FIRSTPRGMSTEP;
}

void show_memory()  // show the registers (memory[0..9]) content
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
    update_curpos(NOMOVE);
    update_lcd();
}

void store_memory(int mempos) // store values in memory register, applying the selected artithmetic if it's the case
{
    switch (storage_arith) {
	case 0: { memory[mempos]  = stack[0]; break; }
	case 1: { memory[mempos] += stack[0]; break; } 
	case 2: { memory[mempos] -= stack[0]; break; } 
	case 3: { memory[mempos] *= stack[0]; break; } 
	case 4: { memory[mempos] /= stack[0]; break; } 
    }
    storage_arith = 0; // reset the arithmetic value (0 = not arithmetic selected)
    store_hit = false; 
    func_hit = true;
}

void recall_memory(int mempos)
{
    push_stack();
    stack[0] = memory[mempos]; 
    recall_hit = false;
    func_hit = true;
}

void clear_memory() // clear the memory[0..9] registers and stack
{
    int i;
    for (i=0; i<10; i++) memory[i] = 0.0;
    for (i=0; i<4; i++) stack[i] = 0.0; 
    lastx = 0.0;
}
