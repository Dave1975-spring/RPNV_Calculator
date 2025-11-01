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

void read_statelog() // to read all values in stack, registers and program list, if "state.log" file is available
{
    char sbuffer[80],s_row[80];
    int i,s_len;

    if ((fp_state = freopen(STATEFILE,"r",stderr)) != NULL) {
	// header 
	fscanf(fp_state,"%[^\n]s",sbuffer); fscanf(fp_state,"%[\n]s",sbuffer);

	// Stack registers values - stack[0..3] + LastX
	fscanf(fp_state,"%[^\n]s",sbuffer); fscanf(fp_state,"%[\n]s",sbuffer); 
	for (i=0;i<4;i++) {
	    fscanf(fp_state,"%s %s",sbuffer, sbuffer);
	    stack[i]=atof(sbuffer); 
	    fscanf(fp_state,"%[\n]s",sbuffer);
	} 
	fscanf(fp_state,"%s %s",sbuffer,sbuffer);
	lastx=atof(sbuffer); 
	fscanf(fp_state,"%[\n]s",sbuffer);

	// memory registers values - memory[0..9] section
	fscanf(fp_state,"%[^\n]s",sbuffer); fscanf(fp_state,"%[\n]s",sbuffer); 
	for (i=0;i<10;i++) {
	    fscanf(fp_state,"%s %s",sbuffer,sbuffer); 
	    memory[i]=atof(sbuffer); 
	    fscanf(fp_state,"%[\n]s",sbuffer);
	}

	// program list - prgm_list[0..MAXPRGMLIST] section
	fscanf(fp_state,"%s %s %[^\n]s",sbuffer,sbuffer,prgm_title);  fscanf(fp_state,"%[\n]s",sbuffer); // program title row
	fscanf(fp_state,"%s %s %s %s %s",sbuffer,sbuffer,sbuffer,sbuffer,sbuffer); // program index at ...
	prgm_index=atoi(sbuffer);
	fscanf(fp_state,"%[\n]s",sbuffer); 
	fscanf(fp_state,"%s %s %s %s %s",sbuffer,sbuffer,sbuffer,sbuffer,sbuffer); // last program row at ...
	prgm_index_max=atoi(sbuffer);
	fscanf(fp_state,"%[\n]s",sbuffer); 
	fscanf(fp_state,"%[^\n]s",sbuffer);  fscanf(fp_state,"%[\n]s",sbuffer); // header row
	fscanf(fp_state,"%[^\n]s",sbuffer);  fscanf(fp_state,"%[\n]s",sbuffer); // first row of program

	// initialize first row (row 0) of the programm which is blank
	strcpy(prgm_list[0][2],"  ");
	strcpy(prgm_list[0][1],"  ");
	strcpy(prgm_list[0][0],"  "); 

	// read program lines from row 1 to row 99
	for (i=1;i<MAXPRGMLIST;i++) {
	    fscanf(fp_state,"%s %[^\n]s %[\n]s",sbuffer,s_row,sbuffer);
	    s_len = strlen(s_row);
	    strcpy(prgm_list[i][2],"  ");
	    strcpy(prgm_list[i][1],"  ");
	    strcpy(prgm_list[i][0],"  ");
	    if (s_len==1) { 
		memcpy(prgm_list[i][2],s_row+(s_len-1),1); // single digit 
		prgm_list[i][2][1]='\0';
	    }
	    if (s_len>=2) {
		memcpy(prgm_list[i][2],s_row+(s_len-2),2); // two digits
		prgm_list[i][2][2]='\0';
	    }
	    if (s_len>=5) {
		memcpy(prgm_list[i][1],s_row+(s_len-5),2); // 4 digits
		prgm_list[i][1][2]='\0';
	    }
	    if (s_len>=8) {
		memcpy(prgm_list[i][0],s_row+(s_len-8),2); // 6 digits
		prgm_list[i][0][2]='\0';
	    } 
	}
    } else clear_prgm_list();

    fclose(fp_state); 
}

void save_statelog() // to store all values in stack, registers and program list, to be read at the next ON of the calc
{
    char statelogtxt[80];
    char label[4][3]={"X:","Y:","Z:","T:"};
    struct tm time_of_day;
    time_t ltime;
    auto char buf[26];
    int i,j;

    time(&ltime);
    _localtime(&ltime,&time_of_day);

    fp_state = freopen(STATEFILE,"w",stderr);

    // File header
    sprintf(statelogtxt,"RPNV %s CALC STATE LOG - %s",VERSION,_asctime(&time_of_day,buf));
    fprintf(fp_state,statelogtxt);

    // Stack registers values - stack[0..3] + LastX
    fprintf(fp_state,"STACK REGISTERS VALUES:\n");
    for (i=0;i<4;i++) fprintf(fp_state,"%s     % 1.*E\n",label[i],MAXDIGITS,stack[i]);
    fprintf(fp_state,"LastX: % 1.*E\n",MAXDIGITS,lastx);

    // Store memory registers values - memory[0..9]
    fprintf(fp_state,"MEMORY REGISTERS VALUES:\n");
    for (i=0;i<10;i++) fprintf(fp_state,"%i: % 1.*E\n",i,MAXDIGITS,memory[i]);

    // Store program list - prgm_list[0..MAXPRGMLIST]
    fprintf(fp_state,"PROGRAM TITLE: %s\n",prgm_title);
    fprintf(fp_state,"PROGRAM INDEX AT ROW: %i\n",prgm_index);
    fprintf(fp_state,"PROGRAM LAST ROW AT:  %i\n",prgm_index_max);
    fprintf(fp_state,"PROGRAM LIST: (keep the format RR- XX YY ZZ to avoid error in loading)\n");
    for (i=0;i<MAXPRGMLIST;i++) 
	fprintf(fp_state,"%02i- %2s %2s %2s\n",i,prgm_list[i][0],prgm_list[i][1],prgm_list[i][2]);

    fprintf(fp,"END STATE FILE"); 
    fclose(fp_state);
}

