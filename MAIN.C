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

void main(int argc, char *argv[])
{ 
    int result = 0;
    char *datalogtxt;

    while (argc-->1) {
	if (strcmp(argv[argc],"log")==0) {
	    buttonslog = true;
	    fp = fopen(LOGFILE,"w");
	    sprintf(datalogtxt,"RPNV %s - DATALOG\n",VERSION);
	    fprintf(fp,datalogtxt);
	}
    }

    initial_steps();    // set video mode

    init_calc_screen(); // draw the calculator layout

    init_mouse();

    show_mouse();

    double_speed_mouse();

    update_curpos(NOMOVE);   // move button cursor to defaul position - ENTER key

    update_lcd();       // update the lcd screen with starting value

    while (result >= 0) result = main_loop();        // main calc loop

    hide_mouse();

    closure_steps();    // return to default video mode

    if (buttonslog) fclose(fp);
}
