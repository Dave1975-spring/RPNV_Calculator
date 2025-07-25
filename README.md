# RPNV

### WHAT IT IS:

RPNV is an RPN calculator inspired by HP Voyager calculator series, in particular the [HP10-C](https://www.hpmuseum.org/hp10c.htm).

It is coded in C, in FreeDos, using FED text editor and Open Watcom C 1.9 compiler.

It has been made for fun, for my personal amusement in making a C code using Conio library in the FreeDos environment. 
There's no intent to make whatever a high quality RPN calculator. So only some basic functions have been implemented, other may be implemented in the future.
There could be some bugs in the code, so it should no be used for any valuable calculation work. _Of course it's released without any kind of warranty as stated in the LICENSE file._

If you have any comment or whatever feel free to send to me a note.

### Special thanks to: 
- [FreeDOS](https://freedos.org/) Development Team for this great OS
- [Jim Hall](https://freedos.org/jhall/) for his great tutorials on C and CONIO
- [Shawn Hargreaves](https://shawnhargreaves.com/fed/) for his great FED text Editor
- ["root42"](https://www.youtube.com/@root42) for his very usefull MS-DOS programming tutorials
- [Liamtoh Resu](https://www.hpmuseum.org/forum/user-13332.html) for the several tests done and bugs highlighted
- [Albert Chan](https://www.hpmuseum.org/forum/user-9024.html) for his help in understand and correct the DMS<>DD conversion bug

### WHAT HAS BEEN IMPLEMENTED AND WHAT IS NOT:

The calculator has a 4 level stack (X,Y,Z,T registers), the Last-X register and 10 memory registers (named from 0 to 9) where values can be stored and recall. The basic operation, the trigonometric function and few others are available as well. No programming capability has been implemented as well as statistical function. Maybe in the future.

### HOW IT WORKS:

Use the arrow keys to move the cursor on the desired button then hit SPACE bar to push the button. All the implemented functions can be used in this way. Mouse is also supported. Some functions has a keyboard shortcut:
- all numbers can be used
- all the four basic operation: + - * /
- the ENTER key works as the ENTER button on the calculator
- K switches on and off the show of the stack content beside the LCD display
- T rotates the stack value
- L recalls the value from the Last-X register
- M shows the content of the memory registers from 0 to 9
- S stores the current value from stack X to a memory register from 0 to 9
- C recalls the value from a memory register from 0 to 9 and place it in the stack X register
- P recalls the pi-greek value
- F toggles the F second function
- E for EEX function

A data.log file is written at every use, containing all calculator keystrokes, their meaning and the the stack & last-x registers content at each button pressed.

![Calc layout](https://github.com/user-attachments/assets/05ecbbef-787c-4fd0-afc1-cd8879086e32)
