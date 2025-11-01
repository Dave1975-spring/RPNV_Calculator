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

int main_loop()  // this is the main loop tracking the key pressed by the user
{
    int c = 0,i = 0;
    int mouse_x,mouse_y,mouse_left,mouse_right;

    if (calc_mode==EXEC) { // if in EXECution mode, run the program stored in prgm_list at prgm_index
	if (!EXEC_PSE) { 
	    exec_PRGM_row();  // if not in pause, exec the next row of program list
	    if (prgm_index==FIRSTPRGMSTEP) { 
		calc_mode = RUN;  // back to 00 or step finished, so end of the program
		update_lcd_badge();
	    }
	}
	else { // if in pause, check if it's time to continue to exec the program
	    exec_pse_actual = time(NULL);
	    if ((exec_pse_actual-exec_pse_initial)>=2) EXEC_PSE = false; // if at least 1 second has passed, pause is finished
	}
    }

    get_mouse(&mouse_x,&mouse_y,&mouse_left,&mouse_right); // get mouse position and click, if the case

    if (mouse_left) {  // if mouse left button pressed, hit the corresponding calc button
	hit_button_at_curpos(mouse_position(mouse_x,mouse_y));
	delay(500);  // give sum time to unpress the mouse button and avoid undesired multiple click ...
    }

    if (kbhit()) { // check if PC keyboard has been pressed and in case hit the corresponding calc button
	c = getch();
	if ( c ==(char)0 ) {    // if special keys are pressed, as arrows
	    c = getch();
	    switch (c) {
		case 72: // UP
		    update_curpos(UP);
		    break;
		case 80: // DOWN
		    update_curpos(DOWN);
		    break;
		case 75: // LEFT
		    update_curpos(LEFT);
		    break;
		case 77: // RIGHT
		    update_curpos(RIGHT);
		    break; 
	    } 
	} else switch (c) {
	    case  8: // BACKSPACE
		if (calc_mode==RUN) hit_button_at_curpos(25);
		if (calc_mode==PRGM) {
		    delete_PRGM_row();
		    update_lcd();
		}
		break; 
	    case 13: // ENTER
		hit_button_at_curpos(26);
		break; 
	    case 27: // !!! ESC !!! ==>> RETURN -1, which stops the MAINLOOP and exit the calculator
		return -1;
	    case 32: // SPACE
		hit_button_at_curpos(curpos);
		break;
	    case 42: // *
		hit_button_at_curpos(20);
		break;
	    case 43: // +
		hit_button_at_curpos(40);
		break;
	    case 45: // -
		hit_button_at_curpos(30);
		break;
	    case 46: // .
		hit_button_at_curpos(38);
		break;
	    case 47: // /
		hit_button_at_curpos(10);
		break;
	    case 48: // 0
		hit_button_at_curpos(37);
		break;
	    case 49: // 1
		hit_button_at_curpos(27);
		break;
	    case 50: // 2
		hit_button_at_curpos(28);
		break;
	    case 51: // 3
		hit_button_at_curpos(29);
		break;
	    case 52: // 4
		hit_button_at_curpos(17);
		break;
	    case 53: // 5
		hit_button_at_curpos(18);
		break;
	    case 54: // 6
		hit_button_at_curpos(19);
		break;
	    case 55: // 7
		hit_button_at_curpos(7);
		break;
	    case 56: // 8
		hit_button_at_curpos(8); 
		break;
	    case 57: // 9
		hit_button_at_curpos(9); 
		break;
	    case 66: case 98: // B for calculator backside
		show_backside();
		break;
	    case 67: case 99: // c or C for CHS
		hit_button_at_curpos(6);
		break; 
	    case 69: case 101: // e or E for EEX
		second_f = false;
		hit_button_at_curpos(16);
		break;
	    case 70: case 102: // f or F for SECOND FUNCTION
		hit_button_at_curpos(32);
		break;
	    case 71: case 103: // g or G for GOTO
		hit_button_at_curpos(12);
		break; 
	    case 72: case 104: // h or H for HELP
		show_help();
		break;
	    case 75: case 107: // k or K for show/hide STACK
		if (show_stack==false) {
		    show_stack = true;
		    show_full_stack();
		} else {
		    show_stack = false;
		    clear_full_stack();
		}
		break;
	    case 76: case 108: // l or L for LAST X
		second_f = true;
		hit_button_at_curpos(26);
		break;
	    case 77: case 109: // m or M for show memory registers
		show_memory(); 
		break;
	    case 80: case 112: // p or P for PI
		second_f = true;
		hit_button_at_curpos(6);
		break;
	    case 82: case 114: // r or R for RCL 
		second_f = false;
		hit_button_at_curpos(35);
		break;
	    case 83: case 115: // s or S for STO
		second_f = false;
		hit_button_at_curpos(34);
		break;
	    case 84: case 116: // t or T for R|v , roTaTe stack
		second_f = false;
		hit_button_at_curpos(23);
		break;
	    default:
		return 0; // return 0, so continue the MAINLOOP loop
	}
    }
    return 0; // return 0, so continue the MAINLOOP loop
}
