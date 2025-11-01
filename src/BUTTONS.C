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

void hit_button_at_curpos(int curpos) 
// the behaviour of each calculator's button is defined here
// for the majority of the buttons the behaviour depends if second f function has been just hit
// or, in case of numbers buttons, if STO and RCL buttons have been just hit
{
    char buffer[3];

    second_f_datalog = second_f; // store second_f value for datalog purpouse
    switch (curpos) {
	case 1:  // sqrt(x) / x^2
	    if (store_hit) store_hit = false;
	    if (recall_hit) recall_hit = false;
	    if (stackx_exp_hit==true) stackx_by_exp();
	    if (second_f==false) {
		if (calc_mode==PRGM) fill_PRGM_row(0,0,curpos_to_button[curpos]);
		if ((calc_mode==RUN) || (calc_mode==EXEC)) {
		    if (stack[0]>=0.0) {
			lastx = stack[0];
			stack[0] = sqrt(stack[0]);
			if (enter_hit) enter_hit = false;
			func_hit = true; 
		    } else {
			print_message(6,"Negative number!");
		    }
		}
		update_lcd();
	    }
	    else {
		if (calc_mode==PRGM) fill_PRGM_row(0,42,curpos_to_button[curpos]);
		if ((calc_mode==RUN) || (calc_mode==EXEC)) { 
		    lastx = stack[0];
		    stack[0] = pow(stack[0],2.0); 
		}
		if (enter_hit) enter_hit = false;
		func_hit = true;
		second_f = false;
		update_lcd(); 
	    }
	    break; 
	case 2:  // e^x / LN
	    if (store_hit) store_hit = false; 
	    if (recall_hit) recall_hit = false;
	    if (stackx_exp_hit==true) stackx_by_exp();
	    if (second_f==false) {
		if (calc_mode==PRGM) fill_PRGM_row(0,0,curpos_to_button[curpos]);
		if ((calc_mode==RUN) || (calc_mode==EXEC)) { 
		    lastx = stack[0];
		    stack[0] = exp(stack[0]);
		}
		if (enter_hit) enter_hit = false;
		func_hit = true; 
		update_lcd();
	    }
	    else {
		if (calc_mode==PRGM) fill_PRGM_row(0,42,curpos_to_button[curpos]);
		if ((calc_mode==RUN) || (calc_mode==EXEC)) { 
		    if (stack[0]>=0.0) { 
			lastx = stack[0];
			stack[0] = log(stack[0]);
			if (enter_hit) enter_hit = false;
			func_hit = true;
		    } else {
			print_message(6,"Negative number!");
		    }
		} 
		second_f = false; 
		update_lcd(); 
	    }
	    break; 
	case 3:  // 10^x / LOG10
	    if (store_hit) store_hit = false; 
	    if (recall_hit) recall_hit = false; 
	    if (stackx_exp_hit==true) stackx_by_exp();
	    if (second_f==false) {
		if (calc_mode==PRGM) fill_PRGM_row(0,0,curpos_to_button[curpos]); 
		if ((calc_mode==RUN) || (calc_mode==EXEC)) { 
		    lastx = stack[0];
		    stack[0] = pow(10.0,stack[0]);
		}
		if (enter_hit) enter_hit = false;
		func_hit = true; 
		update_lcd(); 
	    }
	    else {
		if (calc_mode==PRGM) fill_PRGM_row(0,42,curpos_to_button[curpos]);
		if ((calc_mode==RUN) || (calc_mode==EXEC)) { 
		    if (stack[0]>=0.0) { 
			lastx = stack[0];
			stack[0] = log10(stack[0]);
			if (enter_hit) enter_hit = false;
			func_hit = true;
		    } else {
			print_message(6,"Negative number!");
		    }
		}
		second_f = false; 
		update_lcd(); 
	    }
	    break; 
	case 4:  // y^x / ->HMS
	    if (store_hit) store_hit = false; 
	    if (recall_hit) recall_hit = false; 
	    if (stackx_exp_hit==true) stackx_by_exp();
	    if (second_f==false) {
		if (calc_mode==PRGM) fill_PRGM_row(0,0,curpos_to_button[curpos]); 
		if ((calc_mode==RUN) || (calc_mode==EXEC)) { 
		    lastx = stack[0];
		    stack[0] = pow(stack[1],stack[0]);
		    pull_stack();
		}
		if (enter_hit) enter_hit = false;
		func_hit = true; 
		update_lcd(); 
	    }
	    else {
		if (calc_mode==PRGM) fill_PRGM_row(0,42,curpos_to_button[curpos]); 
		if ((calc_mode==RUN) || (calc_mode==EXEC)) { 
		    lastx = stack[0];
		    stack[0] = H_to_HMS(stack[0]);
		}
		if (enter_hit) enter_hit = false;
		func_hit = true;
		second_f = false;
		update_lcd(); 
	    }
	    break;
	case 5:  // 1/x / ->H
	    if (store_hit) store_hit = false; 
	    if (recall_hit) recall_hit = false; 
	    if (stackx_exp_hit==true) stackx_by_exp();
	    if (second_f==false) {
		if (calc_mode==PRGM) fill_PRGM_row(0,0,curpos_to_button[curpos]); 
		if ((calc_mode==RUN) || (calc_mode==EXEC)) { 
		    lastx = stack[0];
		    stack[0] = 1.0 / stack[0];
		}
		if (enter_hit) enter_hit = false;
		func_hit = true; 
		update_lcd(); 
	    }
	    else {
		if (calc_mode==PRGM) fill_PRGM_row(0,42,curpos_to_button[curpos]); 
		if ((calc_mode==RUN) || (calc_mode==EXEC)) { 
		    lastx = stack[0];
		    stack[0] = HMS_to_H(stack[0]);
		}
		if (enter_hit) enter_hit = false;
		func_hit = true;
		second_f = false;
		update_lcd(); 
	    }
	    break;
	case 6:  // CHS / PI
	    if (store_hit) store_hit = false; 
	    if (recall_hit) recall_hit = false; 
	    if (second_f==false) {
		if (calc_mode==PRGM) fill_PRGM_row(0,0,curpos_to_button[curpos]); 
		if ((calc_mode==RUN) || (calc_mode==EXEC)) { 
		    if (stackx_exp_hit==true) stackx_exp *= -1;
		    else stack[0] = stack[0] * -1.0;
		}
		if (!func_hit && !enter_hit) number_hit = true;
	    } 
	    else {
		if (calc_mode==PRGM) fill_PRGM_row(0,42,curpos_to_button[curpos]); 
		if ((calc_mode==RUN) || (calc_mode==EXEC)) { 
		    push_stack();
		    stack[0] = M_PI;
		}
		if (enter_hit) enter_hit = false;
		func_hit = true;
		second_f = false;
	    }
	    update_lcd();
	    break;
	case 7:  // 7 / FIX
	    if (second_f==false) {
		if (calc_mode==PRGM) manage_number_PRGM(curpos);
		if ((calc_mode==RUN) || (calc_mode==EXEC)) manage_number_RUN_EXEC(7);
	    } else {
		disp_mode = 0;
		disp_mode_hit = true;
		second_f = false; 
	    }
	    update_lcd(); 
	    break;
	case 8:  // 8 / SCI
	    if (second_f==false) {
		if (calc_mode==PRGM) manage_number_PRGM(curpos); 
		if ((calc_mode==RUN) || (calc_mode==EXEC)) manage_number_RUN_EXEC(8);
	    } else {
		disp_mode = 1;
		disp_mode_hit = true;
		second_f = false; 
	    } 
	    update_lcd(); 
	    break;
	case 9:  // 9 / ENG
	    if (second_f==false) {
		if (calc_mode==PRGM) manage_number_PRGM(curpos); 
		if ((calc_mode==RUN) || (calc_mode==EXEC)) manage_number_RUN_EXEC(9);
	    } else {
		disp_mode = 2;
		disp_mode_hit = true;
		second_f = false; 
	    } 
	    update_lcd();
	    break;
	case 10: // / / x<=y in Program
	    if (second_f==false) {
		if (recall_hit) recall_hit = false;
		if (calc_mode==PRGM) manage_arith_PRGM(curpos,4);
		if ((calc_mode==RUN) || (calc_mode==EXEC)) { 
		    if (stackx_exp_hit==true) stackx_by_exp(); 
		    if (store_hit==true) { 
			storage_arith = 4; 
			break;
		    } else { 
			lastx = stack[0];
			stack[0] = stack[1]/stack[0];
			pull_stack();
			if (enter_hit) enter_hit = false;
			func_hit = true;
		    }
		}
		update_lcd();
	    } else {
		if (store_hit) store_hit = false;
		if (recall_hit) recall_hit = false; 
		if (calc_mode==PRGM) fill_PRGM_row(0,42,curpos_to_button[curpos]); 
		if (calc_mode==EXEC) { // if x>y skip 1 step
		    if (stack[0]>stack[1]) {
			if ((prgm_index+2)<=prgm_index_max) prgm_index++;
			else {  // reached end of program
			    prgm_index = FIRSTPRGMSTEP;
			    calc_mode = RUN;
			}
		    }
		    func_hit=true; // function stack-enable 
		} 
		second_f = false; 
		update_lcd(); 
	    } 
	    break;
	case 11: // % / ->RECT
	    if (store_hit) store_hit = false; 
	    if (recall_hit) recall_hit = false;
	    if (second_f==false) {
		if (calc_mode==PRGM) fill_PRGM_row(0,0,curpos_to_button[curpos]); 
		if ((calc_mode==RUN) || (calc_mode==EXEC)) { 
		    lastx = stack[0];
		    stack[0] = stack[1]/100.0*stack[0];
		    pull_stack();
		    if (enter_hit) enter_hit = false;
		}
		func_hit = true;
		update_lcd();
	    } else {
		if (calc_mode==PRGM) fill_PRGM_row(0,42,curpos_to_button[curpos]); 
		if ((calc_mode==RUN) || (calc_mode==EXEC)) { 
		    lastx = stack[0];
		    convert_ang(1);
		    pol_to_rec();
		    if (enter_hit) enter_hit = false;
		}
		func_hit = true;
		second_f = false;
		update_lcd(); 
	    } 
	    break;
	case 12: // GTO / ->POLAR
	    if (store_hit) store_hit = false; 
	    if (recall_hit) recall_hit = false;
	    if (second_f==false) {
		if (calc_mode==PRGM) GTO_hit = 2;    // GTO_hit works both as flag (!=0) and counter of remaing digits to input
		if (calc_mode==RUN)  GTO_to = 2; 
		if (calc_mode==EXEC) {
		    prgm_index = atoi(prgm_list[prgm_index][2]) - 1; // it'll be incremented by 1 later
		    GTO_in_use = true; // put TRUE the flag so in MAINLOOP it can go directly to next prgm_index
		}
	    } else {
		if (calc_mode==PRGM) fill_PRGM_row(0,42,curpos_to_button[curpos]); 
		if ((calc_mode==RUN) || (calc_mode==EXEC)) { 
		    lastx = stack[0];
		    rec_to_pol(1);
		    back_convert_ang(1);
		    if (enter_hit) enter_hit = false;
		}
		func_hit = true;
		second_f = false;
		update_lcd(); 
	    } 
	    break;
	case 13: // SIN / ASIN
	    if (store_hit) store_hit = false; 
	    if (recall_hit) recall_hit = false;
	    if (stackx_exp_hit==true) stackx_by_exp();
	    if (second_f == false) {
		if (calc_mode==PRGM) fill_PRGM_row(0,0,curpos_to_button[curpos]); 
		if ((calc_mode==RUN) || (calc_mode==EXEC)) { 
		    lastx = stack[0];
		    convert_ang(0);
		    stack[0] = sin(stack[0]);
		}
		if (enter_hit) enter_hit = false;
		func_hit = true;
		update_lcd();
	    }
	    else {
		if (calc_mode==PRGM) fill_PRGM_row(0,42,curpos_to_button[curpos]); 
		if ((calc_mode==RUN) || (calc_mode==EXEC)) { 
		    if ((stack[0]>=-1.0 && stack[0]<=1.0)) {
			lastx = stack[0];
			stack[0] = asin(stack[0]);
			back_convert_ang(0);
			if (enter_hit) enter_hit = false;
			func_hit = true;
		    } 
		    else print_message(6,"number outside function domain"); 

		}
		second_f = false; 
		update_lcd(); 
	    }
	    break;
	case 14: // COS / ACOS
	    if (store_hit) store_hit = false; 
	    if (recall_hit) recall_hit = false;
	    if (stackx_exp_hit==true) stackx_by_exp(); 
	    if (second_f == false) { 
		if (calc_mode==PRGM) fill_PRGM_row(0,0,curpos_to_button[curpos]); 
		if ((calc_mode==RUN) || (calc_mode==EXEC)) { 
		    lastx = stack[0];
		    convert_ang(0);
		    stack[0] = cos(stack[0]);
		}
		if (enter_hit) enter_hit = false;
		func_hit = true;
		update_lcd();
	    }
	    else {
		if (calc_mode==PRGM) fill_PRGM_row(0,42,curpos_to_button[curpos]); 
		if ((calc_mode==RUN) || (calc_mode==EXEC)) {
		    if ((stack[0]>=-1.0 && stack[0]<=1.0)) { 
			lastx = stack[0];
			stack[0] = acos(stack[0]);
			back_convert_ang(0); 
			if (enter_hit) enter_hit = false;
			func_hit = true;
		    } 
		    else print_message(6,"number outside function domain"); 
		}
		second_f = false; 
		update_lcd(); 
	    }
	    break;
	case 15: // TAN / ATAN
	    if (store_hit) store_hit = false; 
	    if (recall_hit) recall_hit = false; 
	    if (stackx_exp_hit==true) stackx_by_exp(); 
	    if (second_f == false) {
		if (calc_mode==PRGM) fill_PRGM_row(0,0,curpos_to_button[curpos]); 
		if ((calc_mode==RUN) || (calc_mode==EXEC)) {
		    lastx = stack[0];
		    convert_ang(0);
		    stack[0] = tan(stack[0]);
		}
	    }
	    else {
		if (calc_mode==PRGM) fill_PRGM_row(0,42,curpos_to_button[curpos]); 
		if ((calc_mode==RUN) || (calc_mode==EXEC)) { 
		    lastx = stack[0];
		    stack[0] = atan(stack[0]);
		    back_convert_ang(0); 
		}
	    }
	    if (enter_hit) enter_hit = false;
	    second_f = false;
	    func_hit = true;
	    update_lcd();
	    break;
	case 16: // EEX / N!
	    if (store_hit) store_hit = false; 
	    if (recall_hit) recall_hit = false; 
	    if (second_f==false) {
		if (calc_mode==PRGM) fill_PRGM_row(0,0,curpos_to_button[curpos]); 
		if ((calc_mode==RUN) || (calc_mode==EXEC)) {
		    if (stackx_exp_hit==false) {
			if ((func_hit) || (stack[0]==0.0)) { 
			    lastx=stack[0];
			    stack[0] = 1.0;
			}
			stackx_exp_hit = true;
		    }
		}
		update_lcd();
	    } 
	    else {
		if (calc_mode==PRGM) fill_PRGM_row(0,42,curpos_to_button[curpos]); 
		if ((calc_mode==RUN) || (calc_mode==EXEC)) { 
		    if (stackx_exp_hit==true) stackx_by_exp();
		    if (stack[0]>=0.0) {
			if (stack[0]>0.0) stack[0] = factorial(stack[0]);
			else stack[0]=1.0;
			if (enter_hit) enter_hit = false;
			func_hit = true;
		    } else print_message(6,"Negative number!");
		}
		second_f = false; 
		update_lcd();
	    }
	    break; 
	case 17: // 4 / DEG
	    if (second_f==false) {
		if (calc_mode==PRGM) manage_number_PRGM(curpos);
		if ((calc_mode==RUN) || (calc_mode==EXEC)) manage_number_RUN_EXEC(4);
		update_lcd();
	    } else {
		if (calc_mode==PRGM) fill_PRGM_row(0,42,curpos_to_button[curpos]); 
		if ((calc_mode==RUN) || (calc_mode==EXEC)) ang_mode = 0;
		second_f = false;
		update_lcd(); 
	    } 
	    break;
	case 18: // 5 / RAD
	    if (second_f==false) {
		if (calc_mode==PRGM) manage_number_PRGM(curpos);
		if ((calc_mode==RUN) || (calc_mode==EXEC)) manage_number_RUN_EXEC(5);
		update_lcd(); 
	    } else {
		if (calc_mode==PRGM) fill_PRGM_row(0,42,curpos_to_button[curpos]);
		if ((calc_mode==RUN) || (calc_mode==EXEC)) ang_mode = 1;
		second_f = false;
		update_lcd();
	    }
	    break;
	case 19: // 6 / GRD
	    if (second_f==false) {
		if (calc_mode==PRGM) manage_number_PRGM(curpos);
		if ((calc_mode==RUN) || (calc_mode==EXEC)) manage_number_RUN_EXEC(6);
		update_lcd();
	    } else {
		if (calc_mode==PRGM) fill_PRGM_row(0,42,curpos_to_button[curpos]);
		if ((calc_mode==RUN) || (calc_mode==EXEC)) ang_mode = 2;
		second_f = false;
		update_lcd(); 
	    } 
	    break;
	case 20: // * / x=0 in Program
	    if (second_f==false) {
		if (recall_hit) recall_hit = false; 
		if (calc_mode==PRGM) manage_arith_PRGM(curpos,3);
		if ((calc_mode==RUN) || (calc_mode==EXEC)) { 
		    if (stackx_exp_hit==true) stackx_by_exp();
		    if (store_hit==true) { 
			storage_arith = 3; 
			break;
		    } else {
			lastx = stack[0]; 
			stack[0] = stack[1]*stack[0];
			pull_stack();
			if (enter_hit) enter_hit = false;
			func_hit = true; 
		    }
		}
		update_lcd();
	    } else {
		if (store_hit) store_hit = false;
		if (recall_hit) recall_hit = false; 
		if (calc_mode==PRGM) fill_PRGM_row(0,42,curpos_to_button[curpos]);
		if (calc_mode==EXEC) { 
		    if (stack[0]!=0) { // if x!=0 skip 1 step
			if ((prgm_index+2)<=prgm_index_max) prgm_index++;
			else {  // reached end of program
			    prgm_index = FIRSTPRGMSTEP;
			    calc_mode = RUN;
			}
		    }
		    func_hit=true; // function stack-enable
		} 
		second_f = false; 
		update_lcd(); 
	    } 
	    break;
	case 21: // R/S / PSE
	    if (store_hit) store_hit = false; 
	    if (recall_hit) recall_hit = false; 
	    if (stackx_exp_hit==true) stackx_by_exp(); 
	    if (second_f==false) {
		if (calc_mode==RUN) calc_mode = EXEC;
		    // prgm_index = FIRSTPRGMSTEP;
		else if (calc_mode==PRGM) fill_PRGM_row(0,0,curpos_to_button[curpos]);
		else if (calc_mode==EXEC) calc_mode = RUN; 
		update_lcd();
	    } else {
		if (calc_mode==PRGM) fill_PRGM_row(0,42,curpos_to_button[curpos]);
		second_f = false;
		update_lcd();
		if (calc_mode==EXEC) {
		    EXEC_PSE = true;
		    exec_pse_initial = time(NULL); // store the time when pause has started
		}
	    }
	    break;
	case 22: // SST / BST / the pointer prgm_index is pointing to the first empty step 
	    if (store_hit) store_hit = false; 
	    if (recall_hit) recall_hit = false; 
	    if (stackx_exp_hit==true) stackx_by_exp(); 
	    if (second_f==false) {
		if (calc_mode==RUN) {
		    calc_mode=EXEC; // required to get correct execution for some buttons as GTO
		    exec_PRGM_row();
		    calc_mode=RUN; // back to RUN mode
		}
		else if (calc_mode==PRGM) {
		    if (prgm_index==prgm_index_max) prgm_index = FIRSTPRGMSTEP;
		    else prgm_index += 1;
		}
	    } else {
		if (prgm_index==0) prgm_index = prgm_index_max;
		else prgm_index -= 1; 
		second_f = false;
	    }
	    update_lcd();
	    break;
	case 23: // rotate stack / clear program list
	    if (store_hit) store_hit = false; 
	    if (recall_hit) recall_hit = false; 
	    if (second_f==false) {
		if (calc_mode==PRGM) fill_PRGM_row(0,0,curpos_to_button[curpos]); 
		if ((calc_mode==RUN) || (calc_mode==EXEC)) { 
		    if (stackx_exp_hit==true) stackx_by_exp();
		    rotate_stack();
		}
	    }
	    else {
		if (calc_mode==PRGM) clear_prgm_list();
		if (calc_mode==RUN) prgm_index = FIRSTPRGMSTEP;
		second_f = false; 
	    } 
	    update_lcd(); 
	    break;
	case 24: // x<>y / clear Registers 0-9, stack, lastx
	    if (store_hit) store_hit = false; 
	    if (recall_hit) recall_hit = false; 
	    if (second_f==false) {
		if (calc_mode==PRGM) fill_PRGM_row(0,0,curpos_to_button[curpos]); 
		if ((calc_mode==RUN) || (calc_mode==EXEC)) { 
		    if (stackx_exp_hit==true) stackx_by_exp();
		    swap_stackxy();
		}
	    }
	    else {
		if (calc_mode==PRGM) fill_PRGM_row(0,42,curpos_to_button[curpos]); 
		if ((calc_mode==RUN) || (calc_mode==EXEC)) clear_memory();
		second_f = false;
	    }
	    update_lcd();
	    break; 
	case 25: // CLx / PREFIX 
	    if (store_hit) store_hit = false; 
	    if (recall_hit) recall_hit = false; 
	    if (second_f==false) {
		if (calc_mode==PRGM) fill_PRGM_row(0,0,curpos_to_button[curpos]); 
		if ((calc_mode==RUN) || (calc_mode==EXEC)) { 
		    stack[0] = 0.0;
		    stackx_dec = false;
		    enter_hit = true; // CLx is "stack-disabling" as ENTER key
		    func_hit = false;
		}
	    }
	    else {
		store_hit = false; 
		recall_hit = false; 
		disp_mode_hit = false;
		second_f = false;
		if (calc_mode==RUN) prefix_hit = true; 
	    }
	    update_lcd();
	    break;
	case 26: case 36: // ENTER / LastX --> ensure identical code for case 36, still ENTER
	    if (store_hit) store_hit = false; 
	    if (recall_hit) recall_hit = false; 
	    if (second_f==false) {
		if (calc_mode==PRGM) fill_PRGM_row(0,0,curpos_to_button[curpos]); 
		if ((calc_mode==RUN) || (calc_mode==EXEC)) { 
		    if (stackx_exp_hit==true) stackx_by_exp();
		    if (func_hit) func_hit = false; 
		    push_stack();
		}
		enter_hit = true;
	    }
	    else {
		if (calc_mode==PRGM) fill_PRGM_row(0,42,curpos_to_button[curpos]); 
		if ((calc_mode==RUN) || (calc_mode==EXEC)) { 
		    push_stack();
		    stack[0] = lastx;
		    if (enter_hit) enter_hit = false;
		}
		second_f = false;
		func_hit = true;
	    }
	    update_lcd();
	    break;
	case 27: // 1 / Estimation of X and Correlation Coefficient
	    if (second_f==false) {
		if (calc_mode==PRGM) manage_number_PRGM(curpos);
		if ((calc_mode==RUN) || (calc_mode==EXEC)) manage_number_RUN_EXEC(1);
		update_lcd();
	    } else {
		if (calc_mode==PRGM) {
		    fill_PRGM_row(0,42,curpos_to_button[curpos]); 
		    second_f = false; 
		    update_lcd(); 
		} 
		if ((calc_mode==RUN) || (calc_mode==EXEC)) { 
		    if (memory[0]!=0.0) { 
			lastx = stack[0];
			estimation_correlation_x();
			enter_hit = true; // stack behaves as when enter is just it
			func_hit = false;
			second_f = false; 
			update_lcd(); 
		    } else print_message(6,"Error 2: n=0 "); 
		}
	    } 
	    break;
	case 28: // 2 / Estimation of Y and Correlation Coefficient
	    if (second_f==false) {
		if (calc_mode==PRGM) manage_number_PRGM(curpos);
		if ((calc_mode==RUN) || (calc_mode==EXEC)) manage_number_RUN_EXEC(2);
		update_lcd();
	    } else {
		if (calc_mode==PRGM) {
		    fill_PRGM_row(0,42,curpos_to_button[curpos]); 
		    second_f = false; 
		    update_lcd(); 
		} 
		if ((calc_mode==RUN) || (calc_mode==EXEC)) { 
		    if (memory[0]!=0.0) { 
			lastx = stack[0];
			estimation_correlation_y();
			enter_hit = true; // stack behaves as when enter is just it
			func_hit = false;
			second_f = false; 
			update_lcd(); 
		    } else print_message(6,"Error 2: n=0 "); 
		}
	    } 
	    break;
	case 29: // 3 / Linear Regression
	    if (second_f==false) { 
		if (calc_mode==PRGM) manage_number_PRGM(curpos);
		if ((calc_mode==RUN) || (calc_mode==EXEC)) manage_number_RUN_EXEC(3);
		update_lcd();
	    } else {
		if (calc_mode==PRGM) {
		    fill_PRGM_row(0,42,curpos_to_button[curpos]); 
		    second_f = false; 
		    update_lcd(); 
		} 
		if ((calc_mode==RUN) || (calc_mode==EXEC)) { 
		    if (memory[0]!=0.0) { 
			lastx = stack[0];
			push_stack();
			linear_regression();
			enter_hit = true; // stack behaves as when enter is just it
			func_hit = false;
			second_f = false; 
			update_lcd(); 
		    } else print_message(6,"Error 2: n=0 "); 
		}
	    } 
	    break;
	case 30: // - / ->RAD
	    if (second_f==false) {
		if (recall_hit) recall_hit = false; 
		if (calc_mode==PRGM) manage_arith_PRGM(curpos,2);
		if ((calc_mode==RUN) || (calc_mode==EXEC)) { 
		    if (stackx_exp_hit==true) stackx_by_exp(); 
		    if (store_hit==true) { 
			storage_arith = 2; 
			break;
		    } else {
			lastx = stack[0]; 
			stack[0] = stack[1]-stack[0];
			pull_stack();
			if (enter_hit) enter_hit = false;
			func_hit = true;
		    }
		}
		update_lcd();
	    } else {
		if (calc_mode==PRGM) fill_PRGM_row(0,42,curpos_to_button[curpos]); 
		if ((calc_mode==RUN) || (calc_mode==EXEC)) { 
		    lastx = stack[0];
		    stack[0] = deg_to_rad(stack[0]);
		    if (enter_hit) enter_hit = false;
		}
		func_hit = true;
		second_f = false; 
		update_lcd();
	    } 
	    break;
	case 31: // ON / HELP
	    if (second_f==false) {
		// ON button, do nothing
	    } else {
		if (calc_mode==RUN) {
		    update_lcd_badge();
		    show_help();
		}
		second_f = false;
	    } 
	    break;
	case 32: // f - 2nd function / RAN# random number
	    if (store_hit) store_hit = false;
	    if (recall_hit) recall_hit = false; 
	    if (second_f==false) second_f = true;
	    else {
		if (calc_mode==PRGM) fill_PRGM_row(0,42,curpos_to_button[curpos]); 
		if ((calc_mode==RUN) || (calc_mode==EXEC)) { 
		    push_stack();
		    stack[0] = ((double)rand())/RAND_MAX;
		}
		if (enter_hit) enter_hit = false;
		func_hit = true;
		second_f = false;
	    }
	    update_lcd();
	    break;
	case 33: // P/R / MEM program list
	    if (second_f==false) { 
		if (calc_mode==RUN) calc_mode = PRGM;
		else if (calc_mode==PRGM) {
		    calc_mode = RUN;
		    prgm_index = FIRSTPRGMSTEP;
		}
	    } else {
		show_prgm_list();
		second_f = false;
	    }
	    update_lcd();
	    break; 
	case 34: // STO / INT
	    if (second_f==false) { 
		if (stackx_exp_hit==true) stackx_by_exp();
		storage_arith = 0;
		if (store_hit==false) {
		    store_hit = true;
		    if (recall_hit) recall_hit = false;
		    if (second_f) second_f = false;
		} else store_hit = false;
		update_lcd_badge();
	    } else {
		if (calc_mode==PRGM) fill_PRGM_row(0,42,curpos_to_button[curpos]); 
		if ((calc_mode==RUN) || (calc_mode==EXEC)) { 
		    lastx = stack[0];
		    stack[0] = int_part(stack[0]);
		    if (enter_hit) enter_hit = false;
		}
		second_f = false;
		func_hit = true;
		update_lcd();
	    } 
	    break;
	case 35: // RCL / FRAC
	    if (second_f==false) { 
		if (stackx_exp_hit==true) stackx_by_exp();
		if (recall_hit==false) {
		    recall_hit = true;
		    if (store_hit) store_hit = false;
		    if (second_f) second_f = false;
		} else recall_hit = false;
		update_lcd_badge();
	    } else {
		if (calc_mode==PRGM) fill_PRGM_row(0,42,curpos_to_button[curpos]); 
		if ((calc_mode==RUN) || (calc_mode==EXEC)) { 
		    lastx = stack[0];
		    stack[0] = fract_part(stack[0]);
		    if (enter_hit) enter_hit = false;
		}
		second_f = false;
		func_hit = true; 
		update_lcd(); 
	    } 
	    break; 
	case 37: // 0 / mean(x) and mean(y)
	    if (second_f==false) { 
		if (calc_mode==PRGM) manage_number_PRGM(curpos);
		if ((calc_mode==RUN) || (calc_mode==EXEC)) manage_number_RUN_EXEC(0);
		update_lcd(); 
	    } else {
		if (calc_mode==PRGM) { 
		    fill_PRGM_row(0,42,curpos_to_button[curpos]); 
		    second_f = false; 
		    update_lcd(); 
		} 
		if ((calc_mode==RUN) || (calc_mode==EXEC)) { 
		    if (memory[0]!=0.0) {
			lastx = stack[0];
			mean_x_y();
			enter_hit = true; // stack behaves as when enter is just it
			func_hit = false;
			second_f = false; 
			update_lcd(); 
		    } 
		    else print_message(6,"Error 2: n=0 "); 
		}
	    } 
	    break;
	case 38: // . / stddev(x) and stddev(y)
	    if (store_hit) store_hit = false; 
	    if (recall_hit) recall_hit = false; 
	    if (second_f==false) { 
		if (calc_mode==PRGM) {
		    if (GTO_hit==2) {
			GTO_hit=0; // GTO used to move the pointer, not to store GTO XX instruction
			GTO_to=2;
		    } else fill_PRGM_row(0,0,curpos_to_button[curpos]); 
		}
		if ((calc_mode==RUN) || (calc_mode==EXEC)) { 
		    if (stackx_exp_hit==true) stackx_exp_hit = false;
		    if (enter_hit) {
			stack[0] = 0.0;
			stackx_digit = 1;
			stackx_dec_digit = 0;
			if (enter_hit) enter_hit = false;
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
		    number_hit = true;
		}
		update_lcd();
	    } else {
		if (calc_mode==PRGM) {
		    fill_PRGM_row(0,42,curpos_to_button[curpos]); 
		    second_f = false; 
		    update_lcd(); 
		} 
		if ((calc_mode==RUN) || (calc_mode==EXEC)) { 
		    if (memory[0]!=0.0) { 
			lastx = stack[0];
			stddev_x_y();
			enter_hit = true; // stack behaves as when enter is just it
			func_hit = false;
			second_f = false; 
			update_lcd(); 
		    } else print_message(6,"Error 2: n=0 "); 
		}
	    } 
	    break;
	case 39: // SIGMA+ / SIGMA-
	    if (store_hit) store_hit = false; 
	    if (recall_hit) recall_hit = false; 
	    if (second_f==false) {
		if (calc_mode==PRGM) fill_PRGM_row(0,0,curpos_to_button[curpos]); 
		if ((calc_mode==RUN) || (calc_mode==EXEC)) { 
		    lastx = stack[0]; 
		    sigma_plus();
		}
		enter_hit = true; // stack behaves as when enter is just it
		func_hit = false; 
		update_lcd();
	    } else {
		if (calc_mode==PRGM) fill_PRGM_row(0,42,curpos_to_button[curpos]); 
		if ((calc_mode==RUN) || (calc_mode==EXEC)) { 
		    lastx = stack[0];
		    sigma_minus();
		}
		enter_hit = true; // stack behaves as when enter is just it
		func_hit = false;
		second_f = false; 
		update_lcd(); 
	    } 
	    break; 
	case 40: // + / ->DEG
	    if (recall_hit) recall_hit = false; 
	    if (second_f==false) { 
		if (recall_hit) recall_hit = false; 
		if (calc_mode==PRGM) manage_arith_PRGM(curpos,1); 
		if ((calc_mode==RUN) || (calc_mode==EXEC)) {
		    if (stackx_exp_hit==true) stackx_by_exp();
		    if (store_hit==true) { 
			storage_arith = 1; 
			break;
		    } else {
			lastx = stack[0]; 
			stack[0] = stack[1]+stack[0];
			pull_stack();
			if (enter_hit) enter_hit = false;
			func_hit = true;
		    }
		}
		update_lcd();
	    } else {
		if (calc_mode==PRGM) fill_PRGM_row(0,42,curpos_to_button[curpos]); 
		if ((calc_mode==RUN) || (calc_mode==EXEC)) { 
		    lastx = stack[0];
		    stack[0] = rad_to_deg(stack[0]);
		    if (enter_hit) enter_hit = false;
		}
		func_hit = true;
		second_f = false; 
		update_lcd(); 
	    } 
	    break;
    }
    update_datalog(curpos);
}
