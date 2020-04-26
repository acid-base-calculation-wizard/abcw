/*
version : v3.1.0-alpha

MIT License

Copyright (c) 2019 Kyung-ha Lee <index_am@nulleekh.com>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
 */

#include <stdio.h>

#ifndef PH_CALCULATOR_PHCALCFUNC_H
#define PH_CALCULATOR_PHCALCFUNC_H

long SpecifyType1Solute(char *name, long name_length);
//Return index of Type 1 solution.
long SpecifyType2Solute(char *name, long name_length);
//Return index of Type 2 solution.

double PhCalculator(long num_acid, long num_base, long num_type_1, long num_type_2, double *concentration_acid,
                    double *concentration_base, double *concentration_type_1, double *mol_type_2, long *index_type_1,
                    long *index_type_2, double volume_all);
//Return pH value of mixed solution.
int GraphGenerator(long num_acid, long num_base, long num_type_1, long num_type_2, double *concentration_acid,
                   double *concentration_base, double *concentration_type_1, double *mol_type_2, long *index_type_1,
                   long *index_type_2, double volume_all, long titrant_type, long time_titrant, double volume_titrant,
                   double concentration_titrant, long index_titrant);
//Save graph of titration experimentation.
double RecipeFinder(long num_acid, long num_base, long num_type_1, long num_type_2, double *concentration_acid,
                    double *concentration_base, double *concentration_type_1, double *mol_type_2, long *index_type_1,
                    long *index_type_2, double volume_all, long titrant_type, double concentration_titrant,
                    long index_titrant, double target_ph);
//Return volume/mole of titrant to reach target pH value.

#endif //PH_CALCULATOR_PHCALCFUNC_H
