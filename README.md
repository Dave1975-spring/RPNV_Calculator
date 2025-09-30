# RPNV

### WHAT IT IS:

RPNV is an RPN calculator inspired by HP Voyager calculator series, in particular the [HP-10C](https://www.hpmuseum.org/hp10c.htm).

It is coded in C, in FreeDos, using FED text editor and Open Watcom C 1.9 compiler.

It has been made for fun, for my personal amusement in making a C code using Conio library in the FreeDos environment. 
There's no intent to make whatever a high quality RPN calculator. There could be some bugs in the code, so it should not be used for any valuable calculation work. 

_Of course it's released without any kind of warranty as stated in the LICENSE file._

If you have any comment or whatever feel free to send to me a note.

### Special thanks to: 
- [FreeDOS](https://freedos.org/) Development Team for this great OS
- [Jim Hall](https://freedos.org/jhall/) for his great tutorials on C and CONIO
- [Shawn Hargreaves](https://shawnhargreaves.com/fed/) for his great FED text Editor
- ["root42"](https://www.youtube.com/@root42) for his very usefull MS-DOS programming tutorials
- [Liamtoh Resu](https://www.hpmuseum.org/forum/user-13332.html) for the several tests done and bugs highlighted
- [Albert Chan](https://www.hpmuseum.org/forum/user-9024.html) for his help in understand and correct the DMS<>DD conversion bug

### WHAT IS IMPLEMENTED:

All the functionality of the HP-10C calculator have been implemented, so: 
- 4 level stack (X,Y,Z,T registers)
- the Last-X register
- 10 memory registers (named from 0 to 9) where values can be stored, with or without arithmetic, and recall.
- All the basic operations, the trigonometric functions and statistical functions
- Programming capability, up to 99 steps of program

For functions behaviour and programming please refer to HP-10C onwer's handbook.

In addition:
- A "data.log" file is written at every use, containing all calculator keystrokes, their meaning, the stack & last-x registers content at each button pressed
- A "state.log" file is written at every use, containing stacks values, register values and program list. If the file is present in the same directory of the exectuable file, then it's also read at startup, so it can be used to load in the calculator memory the program or memory registers values of interest; just rename the ".log" file cointaing the data of interest as "state.log" before launching the calculator
- In PROGRAM mode, differently from the HP-10C, new row of program are inserted, not overwriting the existing row

### HOW IT WORKS:

Use the arrow keys to move the cursor on the desired button then hit SPACE bar to push the button. All the implemented functions can be used in this way. Mouse is also supported. Some functions has a keyboard shortcut:
- all numbers can be used
- all the four basic operation: + - * /
- the ENTER key works as the ENTER button on the calculator
- BACKSPACE: in EXEC mode clear the display (X register), in PROG mode delete the current row
- K switches on and off the show of the stack content beside the LCD display
- T rotates the stack value
- L recalls the value from the Last-X register
- M shows the content of the memory registers from 0 to 9
- S stores the current value from stack X to a memory register from 0 to 9
- R recalls the value from a memory register from 0 to 9 and place it in the stack X register
- C change sign
- P recalls the pi-greek value
- F toggles the F second function
- E for EEX function

![Calc layout](https://github.com/user-attachments/assets/05ecbbef-787c-4fd0-afc1-cd8879086e32)
