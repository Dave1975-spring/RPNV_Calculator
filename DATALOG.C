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

void start_datalog()
{
    char datalogtxt[80];
    struct tm time_of_day;
    time_t ltime;
    auto char buf[26];

    time(&ltime);
    _localtime(&ltime,&time_of_day);

    fp = freopen(LOGFILE,"w",stderr);

    sprintf(datalogtxt,"RPNV %s DATALOG - %s",VERSION,_asctime(&time_of_day,buf));
    fprintf(fp,datalogtxt);
    fprintf(fp,"Calc buttons pressed, meaning and stack or calc error, if any\n");
    fprintf(fp,"BUTTON\tMEAN\tX:\t\t\tY:\t\t\tZ:\t\t\tT:\t\t\tLastX:\n");
}

void update_datalog(int curpos)
{
    if (curpos!=0) {
	fprintf(fp,"%i%s\t%s\t% 1.*E\t% 1.*E\t% 1.*E\t% 1.*E\t% 1.*E\n",
		    curpos,
		    (second_f?" 2ndF":""),
		    (second_f?butt_2ndF[curpos-1]:butt_base[curpos-1]),
		    MAXDIGITS,stack[0],
		    MAXDIGITS,stack[1],
		    MAXDIGITS,stack[2],
		    MAXDIGITS,stack[3],
		    MAXDIGITS,lastx); // populate log file 
    }
}
