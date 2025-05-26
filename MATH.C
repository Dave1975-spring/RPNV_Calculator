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

double factorial(double z)
// factorial based on gamma function according to Spouge's approximation
// adapted from as implemented @ rosettacode.org
{
    const int a = A;  // see #define statement 
    static double c_space[A];
    static double *c = NULL;
    int k;
    double accm;

    if ( c == NULL ) {
	double k1_factrl = 1.0; // (k-1)*(-1)^k with 0!==1
	c = c_space;
	c[0] = sqrt(2.0 * M_PI);
	for (k=1; k<a; k++) {
	    c[k] = exp(a-k)*pow(a-k,k-0.5) / k1_factrl;
	    k1_factrl *= -k;
	}
    }
    accm = c[0];
    for (k=1; k<a; k++) {
	accm += c[k] / (z+k);
    }
    accm *= exp(-(z+a)) * pow(z+a, z+0.5); // gamma(z+1)
    return accm;
}

double rad_to_deg(double n)
{
    return n / M_PI * 180.0;
}

double deg_to_rad(double n)
{
    return n * M_PI / 180.0;
}

void convert_ang()
{
    switch (ang_mode) {
	case 0: stack[0] = deg_to_rad(stack[0]); break;
	case 1: break;
	case 2: stack[0] = stack[0] * M_PI / 200.0; break;
    }
}

void back_convert_ang()
{
    switch (ang_mode) {
	case 0: stack[0] = rad_to_deg(stack[0]); break;
	case 1: break;
	case 2: stack[0] = stack[0] / M_PI * 200.0; break;
    }
}

