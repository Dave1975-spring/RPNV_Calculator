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

void rec_to_pol()
{
    double x,y;
    x = stack[0];
    y = stack[1];
    stack[0] = sqrt(pow(x,2) + pow(y,2));
    stack[1] = atan(y/x);
}

void pol_to_rec()
{
    double r,t;
    r = stack[0];
    t = stack[1];
    stack[0] = r * cos(t);
    stack[1] = r * sin(t);
}

double H_to_HMS(double h1)
{
    double h2,m1,m2,s; // h = hours, m = minutes, s = seconds
    h2 = (int)h1;
    m1 = (h1 - h2) * 60.0;
    m2 = (int)m1;
    s  = (m1 - m2) * 60.0;
    return h2 + m2 / 100.0 + s / 10000.0;
}

double HMS_to_H(double h1)
{
    double h2,ms1,ms2,m,s;
    h2 = (int)h1;
    ms1 = (h1 - h2) * 100.0;
    m = (int)ms1;
    s = ms1 - m;
    ms2 = m + s * 100.0 / 60.0;
    return h2 + ms2 / 60.0;
}

double rad_to_deg(double n)
{
    return n / M_PI * 180.0;
}

double deg_to_rad(double n)
{
    return n * M_PI / 180.0;
}

void convert_ang(int s)
{
    switch (ang_mode) {
	case 0: stack[s] = deg_to_rad(stack[s]); break;
	case 1: break;
	case 2: stack[s] = stack[s] * M_PI / 200.0; break;
    }
}

void back_convert_ang(int s)
{
    switch (ang_mode) {
	case 0: stack[s] = rad_to_deg(stack[s]); break;
	case 1: break;
	case 2: stack[s] = stack[s] / M_PI * 200.0; break;
    }
}

void sigma_plus()
{ // according to HP-10C owner's handbook
    memory[0] = memory[0] + 1.0;
    memory[1] = memory[1] + stack[0];
    memory[2] = memory[2] + pow(stack[0],2);
    memory[3] = memory[3] + stack[1];
    memory[4] = memory[4] + pow(stack[1],2);
    stack[0] = memory[0];
}

void sigma_minus()
{ // according to HP-10C owner's handbook
    memory[0] = memory[0] - 1.0;
    memory[1] = memory[1] - stack[0];
    memory[2] = memory[2] - pow(stack[0],2);
    memory[3] = memory[3] - stack[1];
    memory[4] = memory[4] - pow(stack[1],2);
    stack[0] = memory[0];
}

void mean_x_y()
{ // according to HP-10C owner's handbook
    push_stack();
    push_stack();
    stack[0] = memory[1] / memory[0];
    stack[1] = memory[3] / memory[0];
}

void stddev_x_y()
{ // according to HP-10C owner's handbook
    push_stack();
    push_stack();
    stack[0] = sqrt( (memory[0]*memory[2]-pow(memory[1],2)) / (memory[0]*(memory[0]-1.0)) );
    stack[1] = sqrt( (memory[0]*memory[4]-pow(memory[3],2)) / (memory[0]*(memory[0]-1.0)) ); 
}

double int_part(double x)
{
    double integer,fract;
    fract = modf(x,&integer);
    return integer;
}

double fract_part(double x)
{
    double integer,fract;
    fract = modf(x,&integer);
    return fract;
}
