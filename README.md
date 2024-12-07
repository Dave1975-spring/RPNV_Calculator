# RPNV

### WHAT IT IS

RPNV is an RPN calculator inspired by HP Voyager calculator series.
It is coded in C, in FreeDos, using FED text editor and Open Watcom 1.9 

It has been made for fun, for my personal amusement in making a C code using Conio library in the FreeDos environment. 
There's no intent to make whatever a high quality RPN calculator. So only some basic functions have been implemented, other may be implemented in the future.
There could be some bugs in the code, so it should no be used for any valuable calculation work. _Of course it's released without any kind of warranty as stated in the LICENSE file._

If you have any comment or whatever feel free to send to me a note.

### WHAT HAS BEEN IMPLEMENTED AND WHAT IS NOT:

The calculator has a 4 level stack (X,Y,Z,T registers), the Last-X register and 10 memory registers (named from 0 to 9) where values can be stored and recall. The basic operation, the trigonometric function and few others are available as well. No programming capability has been implemented as well as statistical function. Maybe in the future.

### HOW IT WORKS:

Use the arrow keys to move the cursor on the desired button then hit SPACE bar to push the button. All the implemented functions can be used in this way.
Some functions has a keyboard shortcut:
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

![image](https://github.com/user-attachments/assets/9dd779b8-923e-40fd-a5bf-e9af1f1ca8d8)

