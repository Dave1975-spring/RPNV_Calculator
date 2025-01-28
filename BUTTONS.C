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

void hit_button_at_curpos(int curpos) 
// the behaviour of each calculator's button is defined here
// for the majority of the buttons the behaviour depends if second f function has been just hit
// or, in case of numbers buttons, if STO and RCL buttons have been just hit
{
    switch (curpos) {
	case 1:  // sqrt(x) / x^2
	    if (store_hit) store_hit = false;
	    if (recall_hit) recall_hit = false;
	    if (stackx_exp_hit==true) stackx_by_exp();
	    if (second_f==false) {
		if (stack[0]>=0.0) {
		    lastx = stack[0];
		    stack[0] = sqrt(stack[0]);
		    //pull_stack();
		    if (enter_hit) enter_hit = false;
		    func_hit = true; 
		    update_lcd();
		} else {
		    print_message(6,"Negative number!");
		}
	    }
	    else {
		lastx = stack[0];
		stack[0] = pow(stack[0],2.0);
		//pull_stack();
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
		lastx = stack[0];
		stack[0] = exp(stack[0]);
		//pull_stack();
		if (enter_hit) enter_hit = false;
		func_hit = true; 
		update_lcd(); 
	    }
	    else {
		if (stack[0]>=0.0) { 
		    lastx = stack[0];
		    stack[0] = log(stack[0]);
		    //pull_stack();
		    if (enter_hit) enter_hit = false;
		    func_hit = true;
		    second_f = false; 
		    update_lcd(); 
		} else {
		    print_message(6,"Negative number!");
		    second_f = false; 
		} 
	    }
	    break; 
	case 3:  // 10^x / LOG10
	    if (store_hit) store_hit = false; 
	    if (recall_hit) recall_hit = false; 
	    if (stackx_exp_hit==true) stackx_by_exp();
	    if (second_f==false) {
		lastx = stack[0];
		stack[0] = pow(10.0,stack[0]);
		//pull_stack();
		if (enter_hit) enter_hit = false;
		func_hit = true; 
		update_lcd(); 
	    }
	    else {
		if (stack[0]>=0.0) { 
		    lastx = stack[0];
		    stack[0] = log10(stack[0]);
		    //pull_stack();
		    if (enter_hit) enter_hit = false;
		    func_hit = true;
		    second_f = false; 
		    update_lcd(); 
		} else {
		    print_message(6,"Negative number!");
		    second_f = false; 
		}
	    }
	    break; 
	case 4:  // y^x /
	    if (store_hit) store_hit = false; 
	    if (recall_hit) recall_hit = false; 
	    if (second_f==false) {
		if (stackx_exp_hit==true) stackx_by_exp();
		lastx = stack[0];
		stack[0] = pow(stack[1],stack[0]);
		pull_stack();
		if (enter_hit) enter_hit = false;
		func_hit = true; 
		update_lcd(); 
	    }
	    else {
		print_message(14,"Not yet implemented");
		second_f = false; 
	    }
	    break;
	case 5:  // 1/x / 
	    if (store_hit) store_hit = false; 
	    if (recall_hit) recall_hit = false; 
	    if (second_f==false) {
		if (stackx_exp_hit==true) stackx_by_exp();
		lastx = stack[0];
		stack[0] = 1.0 / stack[0];
		//pull_stack();
		if (enter_hit) enter_hit = false;
		func_hit = true; 
		update_lcd(); 
	    }
	    else {
		print_message(14,"Not yet implemented");
		second_f = false; 
	    }
	    break;
	case 6:  // CHS / PI
	    if (store_hit) store_hit = false; 
	    if (recall_hit) recall_hit = false; 
	    if (second_f==false) {
		if (stackx_exp_hit==true) stackx_exp *= -1;
		else stack[0] = stack[0] * -1.0;
		if (!func_hit && !enter_hit) number_hit = true;
	    }
	    else {
		push_stack();
		stack[0] = M_PI;
		if (enter_hit) enter_hit = false;
		func_hit = true;
		second_f = false;
	    }
	    update_lcd();
	    break;
	case 7:  // 7 / FIX
	    if (second_f==false) {
		if (store_hit==true) store_memory(7);
		else if (recall_hit==true) recall_memory(7);
		else if (stackx_exp_hit==true) add_number_exp(7);
		else if (disp_mode_hit==true) {
		    disp_mode_dec_digit = 7;
		    disp_mode_hit = false;
		}
		else {
		    add_number(7.0);
		    number_hit = true;
		}
		update_lcd();
	    } else {
		second_f = false; 
		disp_mode = 0;
		disp_mode_hit = true;
		update_lcd_badge(); 
		func_hit = true; 
	    }
	    break;
	case 8:  // 8 / SCI
	    if (second_f==false) {
		if (store_hit==true) store_memory(8);
		else if (recall_hit==true) recall_memory(8);
		else if (stackx_exp_hit==true) add_number_exp(8);
		else if (disp_mode_hit==true) {
		    disp_mode_dec_digit = 8;
		    disp_mode_hit = false;
		}
		else {
		    add_number(8.0);
		    number_hit = true;
		}
		update_lcd();
	    } else {
		second_f = false; 
		disp_mode = 1;
		disp_mode_hit = true;
		update_lcd_badge(); 
		func_hit = true;
	    } 
	    break;
	case 9:  // 9 / ENG
	    if (second_f==false) {
		if (store_hit==true) store_memory(9);
		else if (recall_hit==true) recall_memory(9);
		else if (stackx_exp_hit==true) add_number_exp(9);
		else if (disp_mode_hit==true) {
		    disp_mode_dec_digit = 9;
		    disp_mode_hit = false;
		} 
		else {
		    add_number(9.0); 
		    number_hit = true;
		}
		update_lcd();
	    } else {
		second_f = false; 
		disp_mode = 2;
		disp_mode_hit = true;
		update_lcd_badge(); 
		func_hit = true; 
	    } 
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
		if (enter_hit) enter_hit = false;
		func_hit = true;
		update_lcd();
	    } else {
		print_message(14,"Not yet implemented");
		second_f = false; 
	    } 
	    break;
	case 11: // % / 
	    if (store_hit) store_hit = false; 
	    if (recall_hit) recall_hit = false;
	    if (second_f==false) {
		lastx = stack[0];
		stack[0] = stack[1]/100.0*stack[0];
		pull_stack();
		if (enter_hit) enter_hit = false;
		func_hit = true;
		update_lcd();
	    } else {
		print_message(14,"Not yet implemented");
		second_f = false; 
	    } 
	    break;
	case 12: 
	    print_message(14,"Not yet implemented");
	    break;
	case 13: // SIN / ASIN
	    if (store_hit) store_hit = false; 
	    if (recall_hit) recall_hit = false; 
	    if (stackx_exp_hit==true) stackx_by_exp();
	    if (second_f == false) {
		lastx = stack[0];
		convert_ang();
		stack[0] = sin(stack[0]);
		//pull_stack();
		if (enter_hit) enter_hit = false;
		func_hit = true;
		update_lcd();
	    }
	    else {
		if ((stack[0]>=-1.0 && stack[0]<=1.0)) {
		    lastx = stack[0];
		    stack[0] = asin(stack[0]);
		    back_convert_ang();
		    //pull_stack();
		    if (enter_hit) enter_hit = false;
		    func_hit = true;
		    second_f = false; 
		    update_lcd();
		} else {
		    print_message(6,"number outside function domain");
		    second_f = false; 
		}
	    }
	    break;
	case 14: // COS / ACOS
	    if (store_hit) store_hit = false; 
	    if (recall_hit) recall_hit = false;
	    if (stackx_exp_hit==true) stackx_by_exp();
	    if (second_f == false) {
		lastx = stack[0];
		convert_ang();
		stack[0] = cos(stack[0]);
		//pull_stack();
		if (enter_hit) enter_hit = false;
		func_hit = true;
		update_lcd();
	    }
	    else {
		if ((stack[0]>=-1.0 && stack[0]<=1.0)) { 
		lastx = stack[0];
		    stack[0] = acos(stack[0]);
		    back_convert_ang(); 
		    //pull_stack();
		    if (enter_hit) enter_hit = false;
		    func_hit = true;
		    second_f = false; 
		    update_lcd();
		} else { 
		    print_message(6,"number outside function domain"); 
		    second_f = false; 
		}
	    }
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
	    //pull_stack();
	    if (enter_hit) enter_hit = false;
	    func_hit = true;
	    update_lcd();
	    break;
	case 16: // EEX / N!
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
		update_lcd();
	    } 
	    else {
		if (stackx_exp_hit==true) stackx_by_exp();
		if (stack[0]>=0.0) {
		    if (stack[0]>0.0) stack[0] = factorial(stack[0]);
		    else stack[0]=1.0;
		    if (enter_hit) enter_hit = false;
		    func_hit = true;
		    second_f = false; 
		    update_lcd();
		} else { 
		    print_message(6,"Negative number!");
		    second_f = false; 
		}
	    }
	    break; 
	case 17: // 4 / DEG
	    if (second_f==false) {
		if (store_hit==true) store_memory(4); 
		else if (recall_hit==true) recall_memory(4); 
		else if (stackx_exp_hit==true) add_number_exp(4);
		else if (disp_mode_hit==true) {
		    disp_mode_dec_digit = 4;
		    disp_mode_hit = false;
		}
		else {
		    add_number(4.0);
		    number_hit = true;
		}
		update_lcd();
	    } else {
		ang_mode = 0;
		second_f = false;
		update_lcd_badge();
	    } 
	    break;
	case 18: // 5 / RAD
	    if (second_f==false) {
		if (store_hit==true) store_memory(5); 
		else if (recall_hit==true) recall_memory(5); 
		else if (stackx_exp_hit==true) add_number_exp(5);
		else if (disp_mode_hit==true) {
		    disp_mode_dec_digit = 5;
		    disp_mode_hit = false;
		} 
		else {
		    add_number(5.0);
		    number_hit = true;
		}
		update_lcd(); 
	    } else {
		ang_mode = 1;
		second_f = false;
		update_lcd_badge();
	    } 
	    break;
	case 19: // 6 / GRD
	    if (second_f==false) {
		if (store_hit==true) store_memory(6); 
		else if (recall_hit==true) recall_memory(6); 
		else if (stackx_exp_hit==true) add_number_exp(6);
		else if (disp_mode_hit==true) {
		    disp_mode_dec_digit = 6;
		    disp_mode_hit = false;
		} 
		else {
		    add_number(6.0);
		    number_hit = true;
		}
		update_lcd();
	    } else {
		ang_mode = 2;
		second_f = false;
		update_lcd_badge();
	    } 
	    break;
	case 20: // * 
	    if (store_hit) store_hit = false; 
	    if (recall_hit) recall_hit = false; 
	    if (second_f==false) {
		if (stackx_exp_hit==true) stackx_by_exp();
		lastx = stack[0]; 
		stack[0] = stack[1]*stack[0];
		pull_stack();
		if (enter_hit) enter_hit = false;
		func_hit = true;
		update_lcd();
	    } else {
		print_message(14,"Not yet implemented");
		second_f = false; 
	    } 
	    break;
	case 21:
	    print_message(14,"Not yet implemented");
	    break;
	case 22:
	    print_message(14,"Not yet implemented");
	    break;
	case 23: // rotate stack
	    if (store_hit) store_hit = false; 
	    if (recall_hit) recall_hit = false; 
	    if (second_f==false) {
		if (stackx_exp_hit==true) stackx_by_exp();
		rotate_stack();
		update_lcd();
	    }
	    else {
		print_message(14,"Not yet implemented");
		second_f = false; 
	    } 
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
	    update_lcd();
	    break; 
	case 25: // CLx / 
	    if (store_hit) store_hit = false; 
	    if (recall_hit) recall_hit = false; 
	    if (second_f==false) {
		stack[0] = 0.0;
		stackx_dec = false;
		if (enter_hit) enter_hit = false;
		func_hit = true;
		update_lcd();
	    }
	    else {
		print_message(14,"Not yet implemented");
		second_f = false; 
	    }
	    break;
	case 26: case 36: // ENTER / LastX --> ensure identical code for case 36, still ENTER
	    if (store_hit) store_hit = false; 
	    if (recall_hit) recall_hit = false; 
	    if (second_f==false) {
		if (stackx_exp_hit==true) stackx_by_exp();
		if (func_hit) func_hit = false; 
		push_stack();
		enter_hit = true;
	    }
	    else {
		push_stack();
		stack[0] = lastx;
		if (enter_hit) enter_hit = false;
		second_f = false;
		func_hit = true;
	    }
	    update_lcd();
	    break;
	case 27: // 1
	    if (second_f==false) {
		if (store_hit==true) store_memory(1); 
		else if (recall_hit==true) recall_memory(1); 
		else if (stackx_exp_hit==true) add_number_exp(1);
		else if (disp_mode_hit==true) {
		    disp_mode_dec_digit = 1;
		    disp_mode_hit = false;
		} 
		else {
		    add_number(1.0);
		    number_hit = true;
		}
		update_lcd();
	    } else {
		print_message(14,"Not yet implemented");
		second_f = false; 
	    } 
	    break;
	case 28: // 2
	    if (second_f==false) {
		if (store_hit==true) store_memory(2); 
		else if (recall_hit==true) recall_memory(2); 
		else if (stackx_exp_hit==true) add_number_exp(2);
		else if (disp_mode_hit==true) {
		    disp_mode_dec_digit = 2;
		    disp_mode_hit = false;
		} 
		else {
		    add_number(2.0);
		    number_hit = true;
		}
		update_lcd();
	    } else {
		print_message(14,"Not yet implemented");
		second_f = false; 
	    } 
	    break;
	case 29: // 3
	    if (second_f==false) { 
		if (store_hit==true) store_memory(3); 
		else if (recall_hit==true) recall_memory(3); 
		else if (stackx_exp_hit==true) add_number_exp(3);
		else if (disp_mode_hit==true) {
		    disp_mode_dec_digit = 3;
		    disp_mode_hit = false;
		} 
		else {
		    add_number(3.0);
		    number_hit = true;
		}
		update_lcd();
	    } else {
		print_message(14,"Not yet implemented");
		second_f = false; 
	    } 
	    break;
	case 30: // -
	    if (store_hit) store_hit = false; 
	    if (recall_hit) recall_hit = false; 
	    if (second_f==false) {
		if (stackx_exp_hit==true) stackx_by_exp(); 
		lastx = stack[0]; 
		stack[0] = stack[1]-stack[0];
		pull_stack();
		if (enter_hit) enter_hit = false;
		func_hit = true;
		update_lcd();
	    } else {
		print_message(14,"Not yet implemented");
		second_f = false; 
	    } 
	    break;
	case 31: // ON / HELP
	    if (second_f==false) {
		// ON button, do nothing
	    } else {
		second_f = false;
		update_lcd_badge();
		show_help();
	    } 
	    break;
	case 32: // f - 2nd function
	    if (store_hit) store_hit = false;
	    if (recall_hit) recall_hit = false; 
	    if (second_f==false) second_f = true;
	    else second_f = false;
	    update_lcd_badge();
	    break;
	case 33:
	    print_message(14,"Not yet implemented");
	    break; 
	case 34: // STO / INT
	    if (second_f==false) { 
		if (stackx_exp_hit==true) stackx_by_exp();
		if (store_hit==false) {
		    store_hit = true;
		    if (recall_hit) recall_hit = false;
		    if (second_f) second_f = false;
		} else store_hit = false;
		update_lcd_badge();
	    } else {
		lastx = stack[0];
		stack[0] = (int)stack[0];
		if (enter_hit) enter_hit = false;
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
		lastx = stack[0];
		stack[0] = stack[0] - (int)stack[0];
		if (enter_hit) enter_hit = false;
		second_f = false;
		func_hit = true; 
		update_lcd(); 
	    } 
	    break; 
	case 37: // 0
	    if (second_f==false) { 
		if (store_hit==true) store_memory(0); 
		else if (recall_hit==true) recall_memory(0); 
		else if (stackx_exp_hit==true) add_number_exp(0);
		else if (disp_mode_hit==true) {
		    disp_mode_dec_digit = 0;
		    disp_mode_hit = false;
		} 
		else {
		    add_number(0.0);
		    number_hit = true;
		}
		update_lcd(); 
	    } else {
		print_message(14,"Not yet implemented");
		second_f = false; 
	    } 
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
		update_lcd();
	    } else {
		print_message(14,"Not yet implemented");
		second_f = false; 
	    } 
	    break;
	case 39:
	    print_message(14,"Not yet implemented");
	    break; 
	case 40: // +
	    if (store_hit) store_hit = false; 
	    if (recall_hit) recall_hit = false; 
	    if (second_f==false) { 
		if (stackx_exp_hit==true) stackx_by_exp();
		lastx = stack[0]; 
		stack[0] = stack[1]+stack[0];
		pull_stack();
		if (enter_hit) enter_hit = false;
		func_hit = true;
		update_lcd();
	    } else {
		print_message(14,"Not yet implemented");
		second_f = false; 
	    } 
	    break;
    }
    update_datalog(curpos);
}
